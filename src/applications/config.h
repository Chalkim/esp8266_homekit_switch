#ifndef HOMEKIT_APPLICATIONS_CONFIG_H_
#define HOMEKIT_APPLICATIONS_CONFIG_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

typedef struct
{
    uint8_t pin;
    uint8_t state;
} RelayConfig_t;

typedef struct
{
    bool debug;
} SystemConfig_t;

typedef struct
{
    RelayConfig_t relay;
    SystemConfig_t system;
} Config_t;

class ConfigManager
{
private:
    const char *configFilePath_;
    StaticJsonDocument<512> doc_;
    void SyncToConfig();
    void SyncToDoc();

public:
    Config_t config_;

    ConfigManager(const char *configFilePath);

    void setConfig(Config_t config);
    Config_t getConfig();

    bool LoadConfig();
    bool SaveConfig();

    String toString();
};

#endif // HOMEKIT_APPLICATION_CONFIG_H_
