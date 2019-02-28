char httpBuffer[256] = "";

const int HTTP_PARSE_GET_STATUS = 0;
const int HTTP_PARSE_STATUS_NOT_OK = 1;
const int HTTP_PARSE_FIND_BODY = 2;
const int HTTP_PARSE_GET_CHUNK_SIZE = 3;
const int HTTP_PARSE_GET_CHUNK_DATA = 4;

bool startsWith(const char *string, const char *prefix) {
  return strncmp(prefix, string, strlen(prefix)) == 0;
}

void appendChar(char *string, char c) {
  size_t len = strlen(string);
  string[len] = c;
  string[len + 1] = '\0';
}

char* sendHttpRequest(const char *url) {
  #ifdef DEBUG
    Serial.println("\nStarting connection to server... ");
  #endif

  char httpLineBuffer[256] = "";

  strcpy(httpLineBuffer, "");
  strcpy(httpBuffer, "");

  if (client.connect(server, 64655)) {
    #ifdef DEBUG
      Serial.println("Connected to server.");
    #endif

    client.print("GET /api/game/");
    client.print(url);
    client.println(" HTTP/1.1");
    client.println("Host: **REPLACE WITH SERVER IP ADDRESS**");
    client.println("Connection: close");
    client.println();
  } else {
    #ifdef DEBUG
      Serial.println("An error occured while connecting to the server.");
    #endif

    return httpBuffer;
  }

  int httpParseState = HTTP_PARSE_GET_STATUS;
  int chunkSize = 0;

  while (client.connected()){
    while (client.available()) {
      char c = client.read();
  
      switch(httpParseState) {
  
        case HTTP_PARSE_GET_STATUS:
          if (c == '\r' && startsWith(httpLineBuffer, "HTTP/1.1")) {
            if (strcmp(httpLineBuffer, "HTTP/1.1 200 OK") == 0) {
              httpParseState = HTTP_PARSE_FIND_BODY;
              strcpy(httpLineBuffer, "");
  
              #ifdef DEBUG
                Serial.println("Status OK.");
              #endif
            } else {
              httpParseState = HTTP_PARSE_STATUS_NOT_OK;
              strcpy(httpLineBuffer, "");
  
              #ifdef DEBUG
                Serial.println("Status not OK.");
              #endif
            }
          } else if (c == '\r' || c == '\n') {
            strcpy(httpLineBuffer, "");
          } else {
            appendChar(httpLineBuffer, c);
          }
  
          break;
  
        case HTTP_PARSE_STATUS_NOT_OK:
          client.flush();
          break;
  
        case HTTP_PARSE_FIND_BODY:
          if (c == '\r' && strlen(httpLineBuffer) == 0) {
            httpParseState = HTTP_PARSE_GET_CHUNK_SIZE;
          } else if (c == '\r' || c == '\n') {
            strcpy(httpLineBuffer, "");
          } else {
            appendChar(httpLineBuffer, c);
          }
  
          break;
  
        case HTTP_PARSE_GET_CHUNK_SIZE:
          if (c == '\r') {
            httpParseState = HTTP_PARSE_GET_CHUNK_DATA;
            chunkSize = strtol(httpLineBuffer, NULL, 16);
            strcpy(httpLineBuffer, "");
  
            #ifdef DEBUG
              Serial.print("Chunk size: ");
              Serial.println(chunkSize);
            #endif
          } else if (c != '\n') {
            appendChar(httpLineBuffer, c);
          }
  
          break;
  
        case HTTP_PARSE_GET_CHUNK_DATA:
          if (c == '\r' && strlen(httpLineBuffer) == chunkSize) {
            strcat(httpBuffer, httpLineBuffer);

            #ifdef DEBUG
              Serial.print("Chunk data: ");
              Serial.println(httpLineBuffer);
            #endif

            strcpy(httpLineBuffer, "");
            httpParseState = HTTP_PARSE_GET_CHUNK_SIZE;
          } else if (c != '\r' && c != '\n' && strlen(httpLineBuffer) < chunkSize) {
            appendChar(httpLineBuffer, c);
          }
  
          break;
      }
    }
  }

  #ifdef DEBUG
    Serial.println("Disconnecting from server.");
  #endif

  client.stop();

  return httpBuffer;
}
