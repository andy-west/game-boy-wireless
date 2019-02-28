#define DEBUG
#define GAME_BOY_ID "0"

#include <ArduinoJson.h>
#include "wifi.h"
#include "http.h"
#include "gpio.h"

const int NO_DATA                       = 0x00;
const int REQUEST_GAME_BOY_STATUS       = 0x01;
const int GAME_BOY_JOINING              = 0x02;
const int GAME_BOY_WAITING_FOR_PLAYER_2 = 0x03;
const int PLAYER_2_JOINED               = 0x04;
const int PLAYER_2_NOT_JOINED           = 0x05;
const int SET_COLOR_BLACK               = 0x06;
const int SET_COLOR_WHITE               = 0x07;
// 0x08 - 0x48 represent disc indexes.
const int MOVE_IS_VALID                 = 0x49;
const int MOVE_IS_NOT_VALID             = 0x4A;
const int GET_NEXT_DISC_TO_FLIP         = 0x4B;
const int GET_OTHER_PLAYER_MOVE         = 0x4C;
const int NO_MORE_DISCS_TO_FLIP         = 0x4D;
const int WAITING_FOR_OTHER_PLAYER_TURN = 0x4E;
const int OTHER_PLAYER_IS_DONE          = 0x4F;
const int OTHER_PLAYER_IS_NOT_DONE      = 0x50;

const int COLOR_NONE  = 0;
const int COLOR_BLACK = 1;
const int COLOR_WHITE = 2;

char httpResponse[256] = "";
byte serialDataIn = 0;
byte serialDataOut = REQUEST_GAME_BOY_STATUS;
char requestUrl[32] = "";
char discIndex[4];

int playerColor;

int moveColor;
int moveX;
int moveY;
bool isMoveValid;

int discsToFlip[18][2] =
  {{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},
  {0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0},
  {0, 0},{0, 0},{0, 0},{0, 0},{0, 0},{0, 0}};

int discsToFlipCount = 0;

bool parsePlaceDiscResult(char* json) {
    StaticJsonBuffer<1024> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(json);
    moveColor = root["playerColor"];
    moveX = root["x"];
    moveY = root["y"];
    isMoveValid = root["isMoveValid"];

    if (isMoveValid) {
      discsToFlipCount = root["discsToFlip"].size();
  
      for (int i = 0; i < discsToFlipCount; i++) {
        for (int j = 0; j < 2; j++) {
          discsToFlip[i][j] = root["discsToFlip"][i][j];
        }
      }
    } else {
      discsToFlipCount = 0;
    }
    
    return root.success();
}

void setup() {
  #ifdef DEBUG
    Serial.begin(9600);
    while (!Serial);
  #endif
  
  wifiSetup();
  setupGpio();
}

void loop() {
  delay(50);

  byte gameBoySerialData = gpioTransferByte(serialDataOut);
  int playerCount;
  int* currentDisc;

  switch(gameBoySerialData) {
    case GAME_BOY_JOINING:
      strcpy(httpResponse, sendHttpRequest("join/" GAME_BOY_ID));

      #ifdef DEBUG
        Serial.println("join/" GAME_BOY_ID);
        Serial.println(httpResponse);
      #endif

      if (strcmp(httpResponse, "Black") == 0) {
        playerColor = COLOR_BLACK;
        serialDataOut = SET_COLOR_BLACK;
      }

      if (strcmp(httpResponse, "White") == 0) {
        playerColor = COLOR_WHITE;
        serialDataOut = SET_COLOR_WHITE;
      }

      break;

    case GAME_BOY_WAITING_FOR_PLAYER_2:
      strcpy(httpResponse, sendHttpRequest("getplayercount"));
      playerCount = atoi(httpResponse);

      #ifdef DEBUG
        Serial.println("getplayercount");
        Serial.println(playerCount);
      #endif
      
      serialDataOut = (playerCount == 2) ? PLAYER_2_JOINED : PLAYER_2_NOT_JOINED;
      
      break;

    case GET_NEXT_DISC_TO_FLIP:

      if (discsToFlipCount == 0) {
        serialDataOut = NO_MORE_DISCS_TO_FLIP;

        #ifdef DEBUG
          Serial.println("No more discs to flip.");
        #endif
      } else {      
        currentDisc = discsToFlip[discsToFlipCount - 1];
        serialDataOut = currentDisc[1] * 8 + currentDisc[0] + 0x08;
        discsToFlipCount--;

        #ifdef DEBUG
          Serial.print("Serial data out: ");
          Serial.println(serialDataOut);
        #endif
      }

      break;

    case GET_OTHER_PLAYER_MOVE:
      serialDataOut = moveY * 8 + moveX + 0x08;

      #ifdef DEBUG
        Serial.print("Serial data out: ");
        Serial.println(serialDataOut);
      #endif

      break;

    case WAITING_FOR_OTHER_PLAYER_TURN:
      strcpy(httpResponse, sendHttpRequest("getlastmove"));
      parsePlaceDiscResult(httpResponse);

      serialDataOut = (moveColor != COLOR_NONE && moveColor != playerColor) ? OTHER_PLAYER_IS_DONE : OTHER_PLAYER_IS_NOT_DONE;

      break;

    default:
      serialDataOut = REQUEST_GAME_BOY_STATUS;
  }

  if (gameBoySerialData >= 0x08 && gameBoySerialData <= 0x48) {
    strcpy(requestUrl, "placedisc/" GAME_BOY_ID "/");
    sprintf(discIndex, "%d", gameBoySerialData - 0x08);
    strcat(requestUrl, discIndex);

    #ifdef DEBUG
      Serial.print("Request URL: ");
      Serial.println(requestUrl);
    #endif
    
    strcpy(httpResponse, sendHttpRequest(requestUrl));

    #ifdef DEBUG
      Serial.print("Response: ");
      Serial.println(httpResponse);
    #endif

    parsePlaceDiscResult(httpResponse);

    #ifdef DEBUG
      Serial.print("isMoveValid: ");
      Serial.println(isMoveValid);

      for (int i = 0; i < discsToFlipCount; i++) {
        Serial.print("discsToFlip: [[");
        Serial.print(discsToFlip[i][0]);
        Serial.print(", ");
        Serial.print(discsToFlip[i][1]);
        Serial.println("]]");
      }
    #endif

    serialDataOut = isMoveValid ? MOVE_IS_VALID : MOVE_IS_NOT_VALID;
  }
}
