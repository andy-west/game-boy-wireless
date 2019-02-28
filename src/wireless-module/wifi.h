#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino-secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;

int status = WL_IDLE_STATUS;
IPAddress server(192, 168, 0, 1);  // Replace with server IP address.

WiFiClient client;

void printWifiStatus() {
  #ifdef DEBUG
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
  #endif

  IPAddress ip = WiFi.localIP();

  #ifdef DEBUG
    Serial.print("IP Address: ");
    Serial.println(ip);
  #endif

  long rssi = WiFi.RSSI();

  #ifdef DEBUG
    Serial.print("Signal strength (RSSI): ");
    Serial.print(rssi);
    Serial.println(" dBm");
  #endif
}

void wifiSetup() {
  if (WiFi.status() == WL_NO_MODULE) {
    #ifdef DEBUG
      Serial.println("Communication with WiFi module failed. Program halted.");
    #endif

    while (true);
  }

  if (WiFi.firmwareVersion() < "1.0.0") {
    #ifdef DEBUG
      Serial.println("Please upgrade the WiFi firmware version.");
    #endif
  }

  while (status != WL_CONNECTED) {
    #ifdef DEBUG
      Serial.print("Attempting to connect to SSID: ");
      Serial.println(ssid);
    #endif

    status = WiFi.begin(ssid, pass);

    delay(10000);
  }

  digitalWrite(LED_BUILTIN, HIGH);

  #ifdef DEBUG
    Serial.println("Connected to WiFi.");
  #endif

  printWifiStatus();
}
