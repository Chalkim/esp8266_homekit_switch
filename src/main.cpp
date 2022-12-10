#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <LittleFS.h>
#include <Ticker.h>
#include <WebSerial.h>
#include <WiFiUdp.h>

#include "applications/config.h"
#include "applications/logger.h"
#include "applications/system_time.h"
#include "drivers/relay.h"
#include "networks/web_server.h"

ConfigManager configManager("/config.json");
Relay relay(configManager.config_.relay.pin,
            configManager.config_.relay.state);
WiFiUDP ntpUDP;
SystemTime systemTime(ntpUDP, 8 * 3600); // UTC+8
Ticker tickerSystemTime;

void recvMsg(uint8_t *data, size_t len)
{
    String d = "";
    for (size_t i = 0; i < len; i++)
    {
        d += char(data[i]);
    }

    if (d == "relay.reverse")
    {
        relay.Reverse();
        auto state = relay.GetState();
        WebSerial.println(state);
        configManager.config_.relay.state = state;
        configManager.SaveConfig();
    }
    else if (d == "relay.high")
    {
        relay.SetState(HIGH);
        auto state = relay.GetState();
        WebSerial.println(state);
        configManager.config_.relay.state = state;
        configManager.SaveConfig();
    }
    else if (d == "relay.low")
    {
        relay.SetState(LOW);
        auto state = relay.GetState();
        WebSerial.println(state);
        configManager.config_.relay.state = state;
        configManager.SaveConfig();
    }
    else if (d == "config.print")
    {
        WebSerial.println(configManager.toString());
    }
    else
    {
        WebSerial.println("Received Data...");
    }
}

void updateTime()
{
    systemTime.update();
}

void setup()
{
    LittleFS.begin();

    AsyncWiFiManager wifiManager(&server, &dns);

    if (!wifiManager.autoConnect("HomeKit_Switch"))
    {
        delay(3000);
        ESP.reset();
        delay(5000);
    }

    systemTime.begin();

    tickerSystemTime.attach(1, updateTime);

    MDNS.begin("homekit");
    MDNS.addService("homekit_web_ui", "http", "tcp", 80);
    MDNS.addService("homekit_control", "websocket", "tcp", 80);

    WebSerial.begin(&server);
    WebSerial.msgCallback(recvMsg);

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/html/index.html"); });

    server.on("/site.webmanifest", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/html/site.webmanifest"); });

    server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/html/favicon.ico"); });

    server.on("/favicon-16x16.png", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/html/favicon-16x16.png"); });

    server.on("/favicon-32x32.png", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/html/favicon-32x32.png"); });

    server.on("/android-chrome-192x192.png", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/html/android-chrome-192x192.png"); });

    server.on("/android-chrome-512x512.png", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/html/android-chrome-512x512.png"); });

    server.on("/apple-touch-icon.png", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(LittleFS, "/html/apple-touch-icon.png"); });

    server.begin();
}

void loop()
{
    MDNS.update();
}
