// ============================================
// ThermoSmart_Relay_v1.2.ino - С АВТОРИЗАЦИЕЙ
// Адаптировано для ESP32 Relay X1 v1.2
// ============================================

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <Wire.h>
#include <time.h>
#include <Update.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>

#include "config.h"
#include "buzzer.h"
#include "stats.h"
#include "logger.h"
#include "web.h"

// ========== Объекты ==========
WebServer server(80);
DNSServer dnsServer;
Preferences preferences;
ThermostatStats stats;

// ========== Глобальные переменные ==========
float currentTemp = 22.5;
float currentHum = 55.0;
float autoTarget = DEFAULT_AUTO_TARGET;
float hysteresis = DEFAULT_HYSTERESIS;
float dayTemp = DEFAULT_DAY_TEMP;
float nightTemp = DEFAULT_NIGHT_TEMP;
String dayStart = DEFAULT_DAY_START;
String nightStart = DEFAULT_NIGHT_START;
String currentMode = DEFAULT_MODE;
bool manualRelay = false;
bool relayState = false;
bool sensorFault = false;
uint8_t sensorReadFailures = 0;
unsigned long nextSensorAlarmAt = 0;
String adminPassword = DEFAULT_PASSWORD;
unsigned long lastWiFiCheck = 0;
unsigned long lastSensorRead = 0;
unsigned long buttonPressStart = 0;
bool apModeActive = false;
bool authenticated = false;
unsigned long lastMemCheck = 0;
bool buzzerEnabled = true;  // ← Новая переменная для управления звуком
bool filesystemReady = false;

// ============================================
// ФУНКЦИИ ДАТЧИКА SHTC3
// ============================================
uint8_t shtc3Crc8(const uint8_t* data, size_t length) {
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : crc << 1;
        }
    }
    return crc;
}

bool readSHTC3(float &temp, float &hum) {
    Wire.beginTransmission(SHTC3_ADDR);
    Wire.write(0x35); Wire.write(0x17);
    if (Wire.endTransmission() != 0) {
        LOG_ERROR("SHTC3 wakeup failed");
        return false;
    }
    delay(1);
    
    Wire.beginTransmission(SHTC3_ADDR);
    Wire.write(0x78); Wire.write(0x66);
    if (Wire.endTransmission() != 0) {
        LOG_ERROR("SHTC3 measure failed");
        return false;
    }
    delay(12);
    
    uint8_t measurement[6];
    if (Wire.requestFrom(SHTC3_ADDR, 6) == 6) {
        for (uint8_t i = 0; i < sizeof(measurement); i++) {
            measurement[i] = static_cast<uint8_t>(Wire.read());
        }

        Wire.beginTransmission(SHTC3_ADDR);
        Wire.write(0xB0); Wire.write(0x98);
        Wire.endTransmission();

        if (shtc3Crc8(measurement, 2) != measurement[2] ||
            shtc3Crc8(measurement + 3, 2) != measurement[5]) {
            return false;
        }

        uint16_t tempRaw = (static_cast<uint16_t>(measurement[0]) << 8) | measurement[1];
        uint16_t humRaw = (static_cast<uint16_t>(measurement[3]) << 8) | measurement[4];
        
        temp = -45.0 + 175.0 * (tempRaw / 65535.0);
        hum = 100.0 * (humRaw / 65535.0);
        
        // Применяем калибровку
        temp += TEMP_CALIBRATION;
        hum += HUM_CALIBRATION;
        if (hum < 0) hum = 0;
        if (hum > 100) hum = 100;
        
        return true;
    }
    LOG_ERROR("SHTC3 read failed");
    return false;
}

bool readSensors() {
    float t, h;
    if (readSHTC3(t, h)) {
        currentTemp = t;
        currentHum = h;
        sensorReadFailures = 0;
        if (sensorFault) {
            sensorFault = false;
            nextSensorAlarmAt = 0;
            LOG_INFO("SHTC3 sensor recovered");
        }
        return true;
    } else {
        if (sensorReadFailures < SENSOR_FAILURE_THRESHOLD) sensorReadFailures++;
        if (!sensorFault && sensorReadFailures >= SENSOR_FAILURE_THRESHOLD) {
            sensorFault = true;
            nextSensorAlarmAt = 0;
            LOG_ERROR("SHTC3 sensor fault after %u consecutive read failures", static_cast<unsigned int>(sensorReadFailures));
        }
        Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
        Wire.setClock(100000);
        return false;
    }
}

// ============================================
// НАСТРОЙКИ И ПАМЯТЬ
// ============================================
void loadSettings() {
    preferences.begin("thermostat", false);
    currentMode = preferences.getString("mode", DEFAULT_MODE);
    autoTarget = preferences.getFloat("autoTarget", DEFAULT_AUTO_TARGET);
    hysteresis = preferences.getFloat("hysteresis", DEFAULT_HYSTERESIS);
    dayTemp = preferences.getFloat("dayTemp", DEFAULT_DAY_TEMP);
    nightTemp = preferences.getFloat("nightTemp", DEFAULT_NIGHT_TEMP);
    dayStart = preferences.getString("dayStart", DEFAULT_DAY_START);
    nightStart = preferences.getString("nightStart", DEFAULT_NIGHT_START);
    manualRelay = preferences.getBool("manualRelay", false);
    adminPassword = preferences.getString("admin_password", DEFAULT_PASSWORD);
    buzzerEnabled = preferences.getBool("buzzerEnabled", true);  // ← Загружаем состояние звука
    preferences.end();
    
    autoTarget = constrain(autoTarget, MIN_TEMP, MAX_TEMP);
    dayTemp = constrain(dayTemp, MIN_TEMP, MAX_TEMP);
    nightTemp = constrain(nightTemp, MIN_TEMP, MAX_TEMP);
    hysteresis = constrain(hysteresis, MIN_HYSTERESIS, MAX_HYSTERESIS);
    
    LOG_INFO("Settings loaded: mode=%s, target=%.1f°C, buzzer=%s", 
             currentMode.c_str(), autoTarget, buzzerEnabled ? "ON" : "OFF");
}

void saveAllSettings() {
    preferences.begin("thermostat", false);
    preferences.putString("mode", currentMode);
    preferences.putFloat("autoTarget", autoTarget);
    preferences.putFloat("hysteresis", hysteresis);
    preferences.putFloat("dayTemp", dayTemp);
    preferences.putFloat("nightTemp", nightTemp);
    preferences.putString("dayStart", dayStart);
    preferences.putString("nightStart", nightStart);
    preferences.putBool("manualRelay", manualRelay);
    preferences.putString("admin_password", adminPassword);
    preferences.putBool("buzzerEnabled", buzzerEnabled);  // ← Сохраняем состояние звука
    preferences.end();
}

void factoryReset() {
    LOG_WARN("Factory reset initiated");
    preferences.begin("thermostat", false);
    preferences.clear();
    preferences.end();
    preferences.begin("wifi", false);
    preferences.clear();
    preferences.end();
    
    adminPassword = DEFAULT_PASSWORD;
    autoTarget = DEFAULT_AUTO_TARGET;
    hysteresis = DEFAULT_HYSTERESIS;
    dayTemp = DEFAULT_DAY_TEMP;
    nightTemp = DEFAULT_NIGHT_TEMP;
    dayStart = DEFAULT_DAY_START;
    nightStart = DEFAULT_NIGHT_START;
    currentMode = DEFAULT_MODE;
    manualRelay = false;
    buzzerEnabled = true;
    
    ThermoSmartLogger::clear();
    LOG_INFO("Factory reset completed");
    delay(500);
    ESP.restart();
}

void checkBootButton() {
    if (digitalRead(BOOT_BUTTON_PIN) == LOW) {
        if (buttonPressStart == 0) {
            buttonPressStart = millis();
        } else if (millis() - buttonPressStart > RESET_HOLD_TIME) {
            LOG_WARN("Factory reset via button");
            factoryReset();
        }
    } else {
        if (buttonPressStart > 0 && millis() - buttonPressStart > 100) {
            if (buzzerEnabled) buzzerClick();  // ← Проверяем звук
        }
        buttonPressStart = 0;
    }
}

// ============================================
// ПРОВЕРКА WI-FI
// ============================================
void checkWiFi() {
    if (apModeActive) return;
    
    if (WiFi.status() != WL_CONNECTED) {
        if (millis() - lastWiFiCheck > 10000) {
            lastWiFiCheck = millis();
            LOG_WARN("WiFi disconnected, reconnecting...");
            WiFi.reconnect();
            if (WiFi.status() != WL_CONNECTED) {
                LOG_WARN("WiFi reconnect failed, switching to fallback AP mode");
                apModeActive = true;
                authenticated = false;
                WiFi.mode(WIFI_AP_STA);
                WiFi.softAP(AP_SSID, AP_PASSWORD);
                dnsServer.start(53, "*", WiFi.softAPIP());
            }
        }
    }
}

// ============================================
// СЕТЬ И ВРЕМЯ
// ============================================
bool connectToWiFi() {
    preferences.begin("wifi", false);
    String ssid = preferences.getString("ssid", "");
    String password = preferences.getString("password", "");
    preferences.end();
    
    if (ssid.length() == 0) {
        LOG_WARN("No WiFi credentials found");
        return false;
    }
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    LOG_INFO("Connecting to WiFi: %s", ssid.c_str());
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40) {
        delay(500);
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("WiFi connected, IP: %s", WiFi.localIP().toString().c_str());
        return true;
    } else {
        LOG_ERROR("WiFi connection failed");
        return false;
    }
}

bool syncNTP() {
    LOG_INFO("Syncing NTP...");
    configTime(5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    
    time_t now = time(nullptr);
    int attempts = 0;
    while (now < 100000 && attempts < 20) {
        delay(500);
        now = time(nullptr);
        attempts++;
    }
    
    if (now >= 100000) {
        LOG_INFO("NTP sync success");
        return true;
    } else {
        LOG_WARN("NTP sync failed");
        return false;
    }
}

bool initSPIFFS() {
    if (!SPIFFS.begin(false)) {
        Serial.println("ERROR: SPIFFS mount failed; automatic format suppressed to protect stored data");
        return false;
    }
    Serial.printf("SPIFFS mounted: %u/%u bytes used\n",
                  static_cast<unsigned int>(SPIFFS.usedBytes()),
                  static_cast<unsigned int>(SPIFFS.totalBytes()));
    return true;
}

// ============================================
// ЛОГИКА ТЕРМОСТАТА
// ============================================
String getCurrentPeriod() {
    time_t now = time(nullptr);
    if (now < 100000) return "day";
    
    struct tm* timeinfo = localtime(&now);
    char currentTime[6];
    sprintf(currentTime, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
    
    if (strcmp(currentTime, nightStart.c_str()) >= 0 || strcmp(currentTime, dayStart.c_str()) < 0) {
        return "night";
    }
    return "day";
}

float getCurrentTarget() {
    if (currentMode == "manual") return manualRelay ? MAX_TEMP : MIN_TEMP;
    if (currentMode == "auto") return autoTarget;
    return getCurrentPeriod() == "night" ? nightTemp : dayTemp;
}

void updateThermostatLogic() {
    float target = getCurrentTarget();
    bool newRelayState = relayState;
    
    if (sensorFault && currentMode != "manual") {
        newRelayState = false;
    } else if (currentMode == "manual") {
        newRelayState = manualRelay;
    } else {
        if (relayState == false && currentTemp <= target - hysteresis) {
            newRelayState = true;
        } 
        else if (relayState == true && currentTemp >= target) {
            newRelayState = false;
        }
    }
    
    if (newRelayState != relayState) {
        if (newRelayState) {
            if (buzzerEnabled) buzzerLongBeep();  // ← Проверяем звук
            digitalWrite(LED_PIN, HIGH);
            LOG_RELAY(true, currentTemp, currentHum, target);
        } else {
            if (buzzerEnabled) buzzerDoubleLongBeep();  // ← Проверяем звук
            digitalWrite(LED_PIN, LOW);
            LOG_RELAY(false, currentTemp, currentHum, target);
        }
        
        relayState = newRelayState;
        digitalWrite(RELAY_PIN, relayState ? HIGH : LOW);
    }
}

void updateSensorFaultAlarm() {
    if (!sensorFault) {
        nextSensorAlarmAt = 0;
        return;
    }

    unsigned long now = millis();
    if (nextSensorAlarmAt == 0 || static_cast<long>(now - nextSensorAlarmAt) >= 0) {
        buzzerDoubleLongBeep();
        nextSensorAlarmAt = now + SENSOR_ALARM_INTERVAL;
    }
}

// ============================================
// ДИАГНОСТИКА ПАМЯТИ
// ============================================
void checkMemory() {
    if (millis() - lastMemCheck > MEMORY_CHECK_INTERVAL) {
        uint32_t freeHeap = ESP.getFreeHeap();
        uint32_t minFreeHeap = ESP.getMinFreeHeap();
        uint32_t maxAllocHeap = ESP.getMaxAllocHeap();
        if (freeHeap < CRITICAL_MEMORY_THRESHOLD) {
            LOG_ERROR("Low memory: %lu bytes, restarting...", static_cast<unsigned long>(freeHeap));
            ESP.restart();
        }
        static bool lowMemoryWarningLogged = false;
        if (freeHeap < LOW_MEMORY_THRESHOLD || maxAllocHeap < 65536) {
            if (!lowMemoryWarningLogged) {
                LOG_WARN("Memory pressure: heap=%lu, min=%lu, max block=%lu bytes",
                         static_cast<unsigned long>(freeHeap),
                         static_cast<unsigned long>(minFreeHeap),
                         static_cast<unsigned long>(maxAllocHeap));
                lowMemoryWarningLogged = true;
            }
        } else {
            lowMemoryWarningLogged = false;
        }
        lastMemCheck = millis();
    }
}

// ============================================
// SETUP
// ============================================
void setup() {
    Serial.begin(115200);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BOOT_BUTTON_PIN, INPUT_PULLUP);
    
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    relayState = false;
    
    Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
    Wire.setClock(100000);
    
    // Инициализация SPIFFS и логгера
    filesystemReady = initSPIFFS();
    ThermoSmartLogger::begin(filesystemReady);
    stats.begin(filesystemReady);
    buzzerInit(BUZZER_PIN);
    if (buzzerEnabled) buzzerDoubleClick();
    
    LOG_INFO("=== System started ===");
    LOG_INFO("Firmware: v1.2 with Auth");
    LOG_INFO("ESP32 Chip: %s", ESP.getChipModel());
    LOG_INFO("Free heap: %d bytes", ESP.getFreeHeap());
    
    loadSettings();
    
    if (connectToWiFi()) {
        initWebServer();
        syncNTP();
        LOG_INFO("Web server started with authentication");
        LOG_INFO("Default password: %s", DEFAULT_PASSWORD);
    } else {
        LOG_WARN("WiFi not connected, starting AP mode");
        setupAPMode();
        LOG_INFO("AP mode started with WiFi setup page");
    }
    
    delay(500);
    readSensors();
}

// ============================================
// LOOP
// ============================================
void loop() {
    checkBootButton();
    checkMemory();
    checkWiFi();
    
    if (apModeActive) {
        dnsServer.processNextRequest();
    }
    server.handleClient();
    
    if (millis() - lastSensorRead > SENSOR_READ_INTERVAL) {
        lastSensorRead = millis();
        if (readSensors()) stats.update(currentTemp, currentHum);
        updateThermostatLogic();
    }
    
    // Периодически сбрасываем буфер лога
    static unsigned long lastFlush = 0;
    if (millis() - lastFlush > 60000) {  // 60 секунд
        ThermoSmartLogger::flush();
        lastFlush = millis();
    }
    
    buzzerUpdate();
    updateSensorFaultAlarm();
    delay(10);
}