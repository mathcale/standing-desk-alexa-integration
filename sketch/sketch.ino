#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "config.h"

WiFiClient client;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe standingDeskControl = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/smart-standing-desk", MQTT_QOS_1);

const int downOptoPin = 2;
const int upOptoPin = 3;

void setup() {
  Serial.begin(115200);

  pinMode(downOptoPin, OUTPUT);
  pinMode(upOptoPin, OUTPUT);

  setupWifi();
  randomSeed(micros());

  standingDeskControl.setCallback(&onEvent);
  mqtt.subscribe(&standingDeskControl);
}

void loop() {
  setupMQTTBroker();

  mqtt.processPackets(10000);

  if (!mqtt.ping()) {
    mqtt.disconnect();
  }
}

void setupWifi() {
  Serial.print(F("Connecting to "));
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }

  Serial.println();
  Serial.println(F("Wifi connected!"));
  Serial.print(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void setupMQTTBroker() {
  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t returnCode;

  while ((returnCode = mqtt.connect()) != 0) {
    switch (returnCode) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }
 
    if (returnCode >= 0) {
      mqtt.disconnect();
    }

    Serial.println(F("Retrying connection..."));
    delay(5000);
  }

  Serial.println(F("Connected!"));
}

void onEvent(char *message, uint16_t len) {
  char messageBuffer[40];
  snprintf(messageBuffer, sizeof(messageBuffer), "Desk status is [%s], len: [%u]", message, len);

  Serial.println(messageBuffer);

  if (strcmp(message, "UP") == 0) {
    Serial.println("Moving desk up...");
    moveDesk(upOptoPin);
  } else if (strcmp(message, "DOWN") == 0) {
    Serial.println("Moving desk down...");
    moveDesk(downOptoPin);
  }
}

void moveDesk(int pin) {
  digitalWrite(pin, HIGH);
  delay(2000);
  digitalWrite(pin, LOW);
}
