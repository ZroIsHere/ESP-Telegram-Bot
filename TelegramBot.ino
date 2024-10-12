#include "Config.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

WiFiClientSecure client;

UniversalTelegramBot bot(botToken, client);

int botRequestDelay = 1000;
unsigned long lastTimeBotRan;

void handleCommand(String text, String chat_id) {
  if (text == "/start") {
    bot.sendMessage(chat_id, "Hiii", "");
  } else {
    bot.sendMessage(chat_id, "Not handled command: " + text, "");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to: " + String(ssid));
  }
  Serial.println("Connected to: " + String(ssid));
  client.setInsecure();
}

void loop() {
  if (millis() - lastTimeBotRan > botRequestDelay) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      for (int i = 0; i < numNewMessages; i++) {
        String chat_id = String(bot.messages[i].chat_id);
        String text = bot.messages[i].text;
        Serial.println("Mensaje recibido: " + text);
        handleCommand(text, chat_id);
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastTimeBotRan = millis();
  }
}
