#include "config.h"

ConfigManager::ConfigManager(const char *configFilePath)
{
    configFilePath_ = configFilePath;
    if (LittleFS.begin())
    {
        LoadConfig();
    }
}

void ConfigManager::setConfig(Config_t config)
{
    config_ = config;
    SyncToDoc();
}

Config_t ConfigManager::getConfig()
{
    return config_;
}

bool ConfigManager::LoadConfig()
{
    File file = LittleFS.open(configFilePath_, "r");
    DeserializationError error = deserializeJson(doc_, file);
    file.close();

    if (error)
    {
        return false;
        // WebSerial.println(F("Failed to read file, using default configuration"));
    }

    SyncToConfig();

    return true;
}

bool ConfigManager::SaveConfig()
{
    SyncToDoc();
    LittleFS.remove(configFilePath_);

    File file = LittleFS.open(configFilePath_, "w");
    if (!file) {
      // Serial.println(F("Failed to create file"));
      return false;
    }

    // Serialize JSON to file
    if (serializeJson(doc_, file) == 0) {
      // Serial.println(F("Failed to write to file"));
      return false;
    }

    // Close the file
    file.close();
    return true;
}

void ConfigManager::SyncToConfig()
{
    config_.relay.pin = doc_["relay"]["pin"] | 0;
    config_.relay.state = doc_["relay"]["state"] | HIGH;
    config_.system.debug = doc_["system"]["debug"] | false;
}

void ConfigManager::SyncToDoc()
{
    doc_["relay"]["pin"] = config_.relay.pin;
    doc_["relay"]["state"] = config_.relay.state;
    doc_["system"]["debug"] = config_.system.debug;
}

String ConfigManager::toString()
{
    String s;
    serializeJson(doc_, s);
    return s;
}
