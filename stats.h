// ============================================
// thermostat_stats.h - Статистика
// ============================================
#ifndef THERMOSTAT_STATS_H
#define THERMOSTAT_STATS_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include "logger.h"

#define STATS_INTERVAL_MS 3600000
#define STATS_RETRY_INTERVAL_MS 60000
#define MAX_RECORDS 720
#define STATS_FILE "/stats.json"
#define STATS_TEMP_FILE "/stats.tmp"
#define STATS_BACKUP_FILE "/stats.bak"

class ThermostatStats {
private:
    unsigned long lastSaveTime;
    unsigned long lastAttemptTime;
    bool statsEnabled;
    bool hasSavedThisBoot;
    bool hasAttemptedThisBoot;
    bool timeWarningLogged;

    bool recoverFiles() {
        if (!SPIFFS.exists(STATS_FILE) && SPIFFS.exists(STATS_BACKUP_FILE)) {
            if (!SPIFFS.rename(STATS_BACKUP_FILE, STATS_FILE)) {
                Serial.println("ERROR: Cannot restore statistics backup");
                LOG_ERROR("Statistics: cannot restore backup file");
                return false;
            }
        }
        if (SPIFFS.exists(STATS_TEMP_FILE) && !SPIFFS.remove(STATS_TEMP_FILE)) {
            Serial.println("ERROR: Cannot remove incomplete statistics temporary file");
            LOG_ERROR("Statistics: cannot remove incomplete temporary file");
            return false;
        }
        if (SPIFFS.exists(STATS_FILE) && SPIFFS.exists(STATS_BACKUP_FILE) &&
            !SPIFFS.remove(STATS_BACKUP_FILE)) {
            Serial.println("ERROR: Cannot remove obsolete statistics backup");
        }
        return true;
    }

    bool writeStatsDocument(DynamicJsonDocument& doc) {
        File tempFile = SPIFFS.open(STATS_TEMP_FILE, "w");
        if (!tempFile) {
            Serial.println("ERROR: Cannot open temporary statistics file for writing");
            LOG_ERROR("Statistics: cannot open temporary file for writing");
            return false;
        }
        size_t expectedSize = measureJson(doc);
        size_t written = serializeJson(doc, tempFile);
        tempFile.close();
        if (written == 0 || written != expectedSize) {
            Serial.println("ERROR: Incomplete statistics write; preserving previous data");
            LOG_ERROR("Statistics: incomplete write (%u/%u bytes)",
                      static_cast<unsigned int>(written),
                      static_cast<unsigned int>(expectedSize));
            return false;
        }

        File verifyFile = SPIFFS.open(STATS_TEMP_FILE, "r");
        if (!verifyFile) {
            Serial.println("ERROR: Cannot verify temporary statistics file");
            LOG_ERROR("Statistics: cannot reopen temporary file for verification");
            return false;
        }
        size_t actualSize = verifyFile.size();
        verifyFile.close();
        if (actualSize != expectedSize) {
            Serial.println("ERROR: Statistics file size verification failed");
            LOG_ERROR("Statistics: size verification failed (%u/%u bytes)",
                      static_cast<unsigned int>(actualSize),
                      static_cast<unsigned int>(expectedSize));
            return false;
        }

        if (SPIFFS.exists(STATS_BACKUP_FILE) && !SPIFFS.remove(STATS_BACKUP_FILE)) {
            Serial.println("ERROR: Cannot remove previous statistics backup");
            LOG_ERROR("Statistics: cannot remove previous backup");
            return false;
        }
        bool hadCurrent = SPIFFS.exists(STATS_FILE);
        if (hadCurrent && !SPIFFS.rename(STATS_FILE, STATS_BACKUP_FILE)) {
            Serial.println("ERROR: Cannot back up current statistics");
            LOG_ERROR("Statistics: cannot back up current file");
            return false;
        }
        if (!SPIFFS.rename(STATS_TEMP_FILE, STATS_FILE)) {
            Serial.println("ERROR: Cannot activate new statistics file");
            LOG_ERROR("Statistics: cannot activate temporary file");
            if (hadCurrent && !SPIFFS.rename(STATS_BACKUP_FILE, STATS_FILE)) {
                Serial.println("ERROR: Cannot restore previous statistics after failed update");
                LOG_ERROR("Statistics: cannot restore previous file");
            }
            return false;
        }
        if (hadCurrent && SPIFFS.exists(STATS_BACKUP_FILE) &&
            !SPIFFS.remove(STATS_BACKUP_FILE)) {
            Serial.println("ERROR: New statistics saved, but old backup could not be removed");
        }
        return true;
    }
    
public:
    ThermostatStats()
        : lastSaveTime(0), lastAttemptTime(0), statsEnabled(true), hasSavedThisBoot(false),
          hasAttemptedThisBoot(false), timeWarningLogged(false) {}
    
    void begin(bool storageAvailable = true) {
        statsEnabled = storageAvailable;
        if (statsEnabled) {
            Serial.println("📊 Статистика: файл /stats.json, макс " + String(MAX_RECORDS) + " записей");
            statsEnabled = recoverFiles();
            LOG_INFO("Statistics storage %s", statsEnabled ? "ready" : "unavailable");
        } else {
            Serial.println("ERROR: Statistics storage unavailable; filesystem was not formatted");
            LOG_ERROR("Statistics storage disabled: filesystem unavailable");
        }
    }
    
    void enable() { statsEnabled = true; }
    void disable() { statsEnabled = false; }
    
    void update(float temperature, float humidity) {
        if (!statsEnabled) return;

        time_t timestamp = time(nullptr);
        if (timestamp < 100000) {
            if (!timeWarningLogged) {
                Serial.println("WARNING: Statistics waiting for valid system time");
                LOG_WARN("Statistics waiting for valid system time");
                timeWarningLogged = true;
            }
            return;
        }
        timeWarningLogged = false;

        unsigned long now = millis();
        if (hasSavedThisBoot && now - lastSaveTime < STATS_INTERVAL_MS) return;
        if (hasAttemptedThisBoot && now - lastAttemptTime < STATS_RETRY_INTERVAL_MS) return;

        lastAttemptTime = now;
        hasAttemptedThisBoot = true;
        if (saveRecord(temperature, humidity)) {
            lastSaveTime = now;
            hasSavedThisBoot = true;
        }
    }
    
    bool saveRecord(float temperature, float humidity) {
        if (!statsEnabled) return false;
        time_t now = time(nullptr);
        if (now < 100000) return false;
        
        float tempNorm = roundf(temperature * 10) / 10.0;
        int humNorm = round(humidity);
        
        DynamicJsonDocument doc(65536);
        JsonArray records;
        
        if (SPIFFS.exists(STATS_FILE)) {
            File file = SPIFFS.open(STATS_FILE, "r");
            if (!file) {
                Serial.println("ERROR: Cannot open statistics file for reading");
                LOG_ERROR("Statistics: cannot open current file for reading");
                return false;
            }
            DeserializationError error = deserializeJson(doc, file);
            file.close();
            if (error) {
                Serial.printf("ERROR: Cannot parse statistics file: %s\n", error.c_str());
                LOG_ERROR("Statistics: cannot parse current file: %s", error.c_str());
                return false;
            }
        }
        
        records = doc.as<JsonArray>();
        if (records.isNull()) {
            if (!doc.isNull()) {
                Serial.println("ERROR: Statistics root is not an array");
                LOG_ERROR("Statistics: root JSON value is not an array");
                return false;
            }
            records = doc.to<JsonArray>();
        }

        if (records.size() >= MAX_RECORDS) {
            records.remove(0);
        }
        
        JsonObject record = records.createNestedObject();
        if (record.isNull()) {
            Serial.println("ERROR: Insufficient memory to create statistics record");
            LOG_ERROR("Statistics: insufficient memory to create record");
            return false;
        }
        record["t"] = (uint32_t)now;
        record["tmp"] = tempNorm;
        record["hum"] = humNorm;
        if (!writeStatsDocument(doc)) return false;
        Serial.printf("📊 Сохранено: %.1f°C, %d%% (всего: %d)\n", tempNorm, humNorm, records.size());
        LOG_INFO("Statistics sample saved: %.1f C, %d%%, records=%u",
                 tempNorm, humNorm, static_cast<unsigned int>(records.size()));
        return true;
    }
    
    bool getStats(int days, String& output) {
        if (!statsEnabled) return false;
        time_t now = time(nullptr);
        uint32_t windowSeconds = static_cast<uint32_t>(days) * 86400UL;
        uint32_t start = now > static_cast<time_t>(windowSeconds)
            ? static_cast<uint32_t>(now - windowSeconds)
            : 0;
        
        if (!SPIFFS.exists(STATS_FILE)) {
            output = "{\"data\":[]}";
            return true;
        }

        File file = SPIFFS.open(STATS_FILE, "r");
        if (!file) {
            Serial.println("ERROR: Cannot open statistics file for reading");
            return false;
        }
        size_t fileSize = file.size();
        DynamicJsonDocument doc(65536);
        DeserializationError error = deserializeJson(doc, file);
        file.close();
        if (error) {
            Serial.printf("ERROR: Cannot parse statistics file: %s\n", error.c_str());
            return false;
        }

        JsonArray records = doc.as<JsonArray>();
        if (records.isNull()) {
            Serial.println("ERROR: Statistics root is not an array");
            return false;
        }

        output = "";
        output.reserve(fileSize + 16);
        output = "{\"data\":[";
        bool first = true;
        char point[80];
        for (JsonVariant v : records) {
            uint32_t timestamp = v["t"].as<uint32_t>();
            if (timestamp < start) continue;

            int length = snprintf(
                point, sizeof(point), "%s{\"t\":%lu,\"tmp\":%.1f,\"hum\":%d}",
                first ? "" : ",",
                static_cast<unsigned long>(timestamp),
                v["tmp"].as<float>(),
                v["hum"].as<int>());
            if (length < 0 || static_cast<size_t>(length) >= sizeof(point)) {
                Serial.println("ERROR: Statistics record formatting failed");
                return false;
            }
            output += point;
            first = false;
        }
        output += "]}";
        return true;
    }
    
    // Получить количество записей в статистике
    int getRecordCount() {
        if (!statsEnabled) return -1;
        if (!SPIFFS.exists(STATS_FILE)) return 0;
        
        File file = SPIFFS.open(STATS_FILE, "r");
        if (!file) return 0;
        
        DynamicJsonDocument doc(65536);
        DeserializationError error = deserializeJson(doc, file);
        file.close();
        
        if (error) return 0;
        
        JsonArray records = doc.as<JsonArray>();
        return records.size();
    }
    
    // Получить размер файла статистики в байтах
    size_t getFileSize() {
        if (!statsEnabled) return 0;
        if (!SPIFFS.exists(STATS_FILE)) return 0;
        
        File file = SPIFFS.open(STATS_FILE, "r");
        if (!file) return 0;
        
        size_t size = file.size();
        file.close();
        return size;
    }
    
    bool clearAll() {
        if (!statsEnabled) return false;
        if (SPIFFS.exists(STATS_TEMP_FILE) && !SPIFFS.remove(STATS_TEMP_FILE)) {
            Serial.println("ERROR: Cannot remove temporary statistics file");
            return false;
        }
        if (SPIFFS.exists(STATS_BACKUP_FILE) && !SPIFFS.remove(STATS_BACKUP_FILE)) {
            Serial.println("ERROR: Cannot remove statistics backup");
            return false;
        }
        if (SPIFFS.exists(STATS_FILE)) {
            if (!SPIFFS.remove(STATS_FILE)) {
                Serial.println("ERROR: Cannot remove statistics file");
                return false;
            }
        }
        lastSaveTime = 0;
        lastAttemptTime = 0;
        hasSavedThisBoot = false;
        hasAttemptedThisBoot = false;
        Serial.println("📊 Статистика удалена");
        return true;
    }
};

#endif