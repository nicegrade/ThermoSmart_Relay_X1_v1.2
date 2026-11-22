// ============================================
// web_interface.h - Веб-интерфейс с авторизацией
// sessionStorage + правильная авторизация
// ============================================
#ifndef WEB_INTERFACE_H
#define WEB_INTERFACE_H

#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <Update.h>
#include "stats.h"
#include "config.h"
#include "logger.h"

// ========== Внешние переменные ==========
extern WebServer server;
extern DNSServer dnsServer;
extern Preferences preferences;
extern ThermostatStats stats;
extern bool filesystemReady;

extern float currentTemp, currentHum;
extern float autoTarget, hysteresis, dayTemp, nightTemp;
extern String dayStart, nightStart, currentMode;
extern bool manualRelay, relayState;
extern bool sensorFault;
extern uint8_t sensorReadFailures;
extern String adminPassword;
extern bool apModeActive;
extern bool authenticated;
extern bool buzzerEnabled;

extern void saveAllSettings();
extern float getCurrentTarget();
extern String getCurrentPeriod();
extern void factoryReset();
extern void buzzerClick();
extern void buzzerDoubleClick();
extern void buzzerLongBeep();

// ========== БУФЕР ==========
#define SEND_BUFFER_SIZE 4096
static char sendBuffer[SEND_BUFFER_SIZE];

// HTML-шаблоны страниц вынесены в отдельные заголовки.
#include "web_login_page.h"
#include "web_main_page.h"
#include "web_stats_page.h"
#include "web_logs_page.h"

// ============================================
// ФУНКЦИИ ОТПРАВКИ
// ============================================
void sendCachedPage(const char* page) {
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    
    size_t len = strlen_P(page);
    server.setContentLength(len);
    server.send(200, "text/html; charset=utf-8", "");
    
    size_t pos = 0;
    while (pos < len) {
        size_t chunk = (len - pos < SEND_BUFFER_SIZE - 1) ? (len - pos) : (SEND_BUFFER_SIZE - 1);
        memcpy_P(sendBuffer, page + pos, chunk);
        sendBuffer[chunk] = '\0';
        server.sendContent(sendBuffer);
        pos += chunk;
        yield();
    }
}

void sendAPI(const String& data) {
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.send(200, "application/json", data);
}

void sendStorageUnavailable() {
    server.send(503, "application/json", "{\"error\":\"filesystem_unavailable\"}");
}

void sendUnauthorized() {
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("WWW-Authenticate", "Bearer realm=\"ThermoSmart\"");
    server.send(401, "application/json", "{\"error\":\"Unauthorized\"}");
}

// ============================================
// АВТОРИЗАЦИЯ
// ============================================
bool isAuthorized() {
    if (authenticated) return true;
    
    if (server.hasHeader("Authorization")) {
        String auth = server.header("Authorization");
        if (auth.startsWith("Bearer ")) {
            String token = auth.substring(7);
            if (token == adminPassword) {
                authenticated = true;
                LOG_INFO("User authenticated via Bearer token");
                return true;
            }
        }
    }
    return false;
}

// ============================================
// OTA HANDLER
// ============================================
void handleOTAUpload() {
    if (!isAuthorized()) {
        server.send(401, "text/plain", "Unauthorized");
        return;
    }
    
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
        if (!Update.begin(maxSketchSpace, U_FLASH)) {
            LOG_ERROR("OTA: Update.begin failed");
            return;
        }
        LOG_INFO("OTA: Starting update");
    } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
            LOG_ERROR("OTA: Write failed");
            return;
        }
    } else if (upload.status == UPLOAD_FILE_END) {
        if (Update.end(true)) {
            LOG_INFO("OTA: Update successful");
            server.send(200, "text/plain", "OK");
            delay(100);
            ESP.restart();
        } else {
            LOG_ERROR("OTA: Update end failed");
            server.send(500, "text/plain", "Update failed");
        }
    }
}

void handleAPI() {
    if (!isAuthorized()) {
        sendUnauthorized();
        return;
    }

    if (server.method() == HTTP_GET) {
        StaticJsonDocument<512> doc;
        doc["temp"] = currentTemp;
        doc["hum"] = currentHum;
        doc["currentTarget"] = getCurrentTarget();
        doc["mode"] = currentMode;
        doc["period"] = getCurrentPeriod();
        doc["relay"] = relayState;
        doc["manualRelay"] = manualRelay;
        doc["sensorFault"] = sensorFault;
        doc["sensorReadFailures"] = sensorReadFailures;
        doc["autoTarget"] = autoTarget;
        doc["dayTemp"] = dayTemp;
        doc["nightTemp"] = nightTemp;
        doc["dayStart"] = dayStart;
        doc["nightStart"] = nightStart;
        doc["hysteresis"] = hysteresis;
        doc["uptime"] = millis() / 1000;

        String response;
        serializeJson(doc, response);
        sendAPI(response);
        return;
    }

    if (server.method() != HTTP_POST) {
        server.send(405, "application/json", "{\"error\":\"Method not allowed\"}");
        return;
    }

    StaticJsonDocument<256> doc;
    if (deserializeJson(doc, server.arg("plain"))) {
        server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
        return;
    }

    bool needSave = false;
    if (doc.containsKey("mode")) {
        String val = doc["mode"].as<String>();
        if (val == "auto" || val == "daynight" || val == "manual") {
                currentMode = val;
                if (buzzerEnabled) buzzerDoubleClick();
                needSave = true;
                LOG_INFO("Mode: %s", val.c_str());
            }
        }
        if (doc.containsKey("autoTarget")) {
            float val = doc["autoTarget"].as<float>();
            if (val >= MIN_TEMP && val <= MAX_TEMP) {
                autoTarget = val;
                if (buzzerEnabled) buzzerClick();
                needSave = true;
                LOG_INFO("Auto target: %.1f°C", val);
            }
        }
        if (doc.containsKey("hysteresis")) {
            float val = doc["hysteresis"].as<float>();
            if (val >= MIN_HYSTERESIS && val <= MAX_HYSTERESIS) {
                hysteresis = val;
                needSave = true;
                LOG_INFO("Hysteresis: %.1f°C", val);
            }
        }
        if (doc.containsKey("dayTemp")) {
            float val = doc["dayTemp"].as<float>();
            if (val >= MIN_TEMP && val <= MAX_TEMP) {
                dayTemp = val;
                if (buzzerEnabled) buzzerClick();
                needSave = true;
                LOG_INFO("Day temp: %.1f°C", val);
            }
        }
        if (doc.containsKey("nightTemp")) {
            float val = doc["nightTemp"].as<float>();
            if (val >= MIN_TEMP && val <= MAX_TEMP) {
                nightTemp = val;
                if (buzzerEnabled) buzzerClick();
                needSave = true;
                LOG_INFO("Night temp: %.1f°C", val);
            }
        }
        if (doc.containsKey("dayStart")) {
            String val = doc["dayStart"].as<String>();
            if (val.length() == 5 && val[2] == ':') {
                dayStart = val;
                needSave = true;
                LOG_INFO("Day start: %s", val.c_str());
            }
        }
        if (doc.containsKey("nightStart")) {
            String val = doc["nightStart"].as<String>();
            if (val.length() == 5 && val[2] == ':') {
                nightStart = val;
                needSave = true;
                LOG_INFO("Night start: %s", val.c_str());
            }
        }
        if (doc.containsKey("manualRelay")) {
            manualRelay = doc["manualRelay"].as<bool>();
            needSave = true;
            LOG_INFO("Manual relay: %s", manualRelay ? "ON" : "OFF");
        }
        
        if (needSave) {
            saveAllSettings();
            sendAPI("{\"status\":\"ok\",\"saved\":true}");
        } else {
            sendAPI("{\"status\":\"ok\",\"saved\":false}");
        }
}

void handleBuzzer() {
    if (!isAuthorized()) {
        sendUnauthorized();
        return;
    }
    
    if (server.method() == HTTP_GET) {
        String resp = "{\"enabled\":";
        resp += buzzerEnabled ? "true" : "false";
        resp += "}";
        sendAPI(resp);
        return;
    }
    
    if (server.method() == HTTP_POST) {
        String body = server.arg("plain");
        StaticJsonDocument<64> doc;
        if (deserializeJson(doc, body)) {
            server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
            return;
        }
        if (doc.containsKey("enabled")) {
            buzzerEnabled = doc["enabled"].as<bool>();
            preferences.begin("thermostat", false);
            preferences.putBool("buzzerEnabled", buzzerEnabled);
            preferences.end();
            LOG_INFO("Buzzer: %s", buzzerEnabled ? "ON" : "OFF");
            sendAPI("{\"status\":\"ok\"}");
            return;
        }
        server.send(400, "application/json", "{\"error\":\"Missing enabled\"}");
    }
}

void handleChangePassword() {
    if (!isAuthorized()) {
        sendUnauthorized();
        return;
    }
    
    String body = server.arg("plain");
    StaticJsonDocument<256> doc;
    if (deserializeJson(doc, body)) {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
        return;
    }
    if (!doc.containsKey("old") || !doc.containsKey("new")) {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing fields\"}");
        return;
    }
    
    String oldPass = doc["old"].as<String>();
    String newPass = doc["new"].as<String>();
    
    if (oldPass != adminPassword) {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Неверный старый пароль\"}");
        return;
    }
    if (newPass.length() < 4) {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Пароль должен быть не менее 4 символов\"}");
        return;
    }
    
    adminPassword = newPass;
    saveAllSettings();
    LOG_INFO("Password changed");
    sendAPI("{\"status\":\"ok\"}");
}

void handleWifiStatus() {
    if (!isAuthorized()) {
        sendUnauthorized();
        return;
    }

    if (apModeActive) {
        String out = "{\"status\":\"ap\",\"ssid\":\"";
        out += AP_SSID;
        out += "\",\"ip\":\"";
        out += WiFi.softAPIP().toString();
        out += "\"}";
        sendAPI(out);
        return;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        String out = "{\"status\":\"connected\",\"ssid\":\"";
        out += WiFi.SSID();
        out += "\",\"ip\":\"";
        out += WiFi.localIP().toString();
        out += "\"}";
        sendAPI(out);
        return;
    }
    sendAPI("{\"status\":\"disconnected\"}");
}

void handleWifiScan() {
    if (!isAuthorized()) {
        sendUnauthorized();
        return;
    }

    if (WiFi.getMode() != WIFI_AP_STA && !apModeActive) {
        WiFi.mode(WIFI_STA);
    }
    if (apModeActive) {
        WiFi.mode(WIFI_AP_STA);
    }
    int networkCount = WiFi.scanNetworks();
    String json = "{\"networks\":[";
    for (int i = 0; i < networkCount; i++) {
        if (i) json += ",";
        json += "{\"ssid\":\"" + WiFi.SSID(i) + "\",\"rssi\":" + String(WiFi.RSSI(i)) + ",\"encryption\":" + String(WiFi.encryptionType(i) != WIFI_AUTH_OPEN) + "}";
    }
    json += "]}";
    WiFi.scanDelete();
    if (apModeActive) {
        WiFi.mode(WIFI_AP_STA);
        WiFi.softAP(AP_SSID, AP_PASSWORD);
    }
    sendAPI(json);
}

void handleWifiConnect() {
    if (!isAuthorized()) {
        sendUnauthorized();
        return;
    }

    String body = server.arg("plain");
    StaticJsonDocument<256> doc;
    if (deserializeJson(doc, body)) {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid JSON\"}");
        return;
    }
    if (!doc.containsKey("ssid")) {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"SSID required\"}");
        return;
    }
    String ssid = doc["ssid"].as<String>();
    String password = doc.containsKey("password") ? doc["password"].as<String>() : "";
    if (ssid.length() == 0) {
        server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"SSID required\"}");
        return;
    }

    preferences.begin("wifi", false);
    preferences.putString("ssid", ssid);
    preferences.putString("password", password);
    preferences.end();

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    int attempts = 0;
    while (attempts < 30 && WiFi.status() != WL_CONNECTED) {
        delay(500);
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        LOG_INFO("WiFi configured from settings: %s", ssid.c_str());
        sendAPI("{\"status\":\"ok\"}");
        delay(800);
        ESP.restart();
        return;
    }

    server.send(200, "application/json", "{\"status\":\"error\",\"message\":\"Не удалось подключиться к Wi‑Fi\"}");
}

// ============================================
// ИНИЦИАЛИЗАЦИЯ ВЕБ-СЕРВЕРА
// ============================================
void initWebServer() {
    LOG_INFO("Initializing web server with authorization...");
    
    preferences.begin("thermostat", false);
    buzzerEnabled = preferences.getBool("buzzerEnabled", true);
    preferences.end();

    // ===== ГЛАВНАЯ СТРАНИЦА =====
    server.on("/", []() {
        if (isAuthorized()) {
            sendCachedPage(MAIN_PAGE);
            return;
        }

        server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
        server.send(200, "text/html; charset=utf-8", LOGIN_PAGE);
    });
    
    // ===== СТРАНИЦА СТАТИСТИКИ =====
    server.on("/stats", []() {
        if (!isAuthorized()) {
            server.send(401, "text/html", "<script>window.location.href='/login'</script>");
            return;
        }
        sendCachedPage(STATS_PAGE);
    });
    
    // ===== СТРАНИЦА ВХОДА =====
    server.on("/login", []() {
        authenticated = false;
        server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
        server.send(200, "text/html; charset=utf-8", LOGIN_PAGE);
    });
    
    // ===== АУТЕНТИФИКАЦИЯ =====
    server.on("/auth", HTTP_POST, []() {
        String body = server.arg("plain");
        StaticJsonDocument<128> doc;
        
        if (deserializeJson(doc, body)) {
            server.send(403, "application/json", "{\"status\":\"error\"}");
            return;
        }
        
        if (doc.containsKey("password")) {
            String password = doc["password"].as<String>();
            if (password == adminPassword) {
                authenticated = true;
                LOG_INFO("User authenticated successfully");
                server.send(200, "application/json", "{\"status\":\"ok\"}");
                return;
            }
            LOG_WARN("Authentication failed: wrong password");
        }
        
        server.send(403, "application/json", "{\"status\":\"error\"}");
    });
    
    // ===== ВЫХОД =====
    server.on("/logout", HTTP_POST, []() {
        authenticated = false;
        LOG_INFO("User logged out");
        sendAPI("{\"status\":\"ok\"}");
    });
    
    // ===== API =====
    server.on("/api/state", handleAPI);
    server.on("/api/set", HTTP_POST, handleAPI);
    server.on("/api/buzzer", HTTP_GET, handleBuzzer);
    server.on("/api/buzzer", HTTP_POST, handleBuzzer);
    server.on("/api/wifi/status", HTTP_GET, handleWifiStatus);
    server.on("/api/wifi/scan", HTTP_GET, handleWifiScan);
    server.on("/api/wifi/connect", HTTP_POST, handleWifiConnect);
    
    // ===== СТАТИСТИКА =====
    server.on("/api/stats", HTTP_GET, []() {
        if (!isAuthorized()) {
            sendUnauthorized();
            return;
        }
        if (!filesystemReady) {
            sendStorageUnavailable();
            return;
        }
        int days = server.hasArg("days") ? server.arg("days").toInt() : 7;
        if (days < 1) days = 1;
        if (days > 30) days = 30;
        String response;
        if (!stats.getStats(days, response)) {
            server.send(500, "application/json", "{\"error\":\"statistics_read_failed\"}");
            return;
        }
        sendAPI(response);
    });
    
    server.on("/api/clear-stats", HTTP_POST, []() {
        if (!isAuthorized()) {
            sendUnauthorized();
            return;
        }
        if (!filesystemReady) {
            sendStorageUnavailable();
            return;
        }
        LOG_INFO("Statistics cleared");
        if (!stats.clearAll()) {
            server.send(500, "application/json", "{\"error\":\"statistics_clear_failed\"}");
            return;
        }
        sendAPI("{\"status\":\"ok\"}");
    });
    
    // ===== СМЕНА ПАРОЛЯ =====
    server.on("/change-password", HTTP_POST, handleChangePassword);
    
    // ===== OTA =====
    server.on("/update", HTTP_POST, []() {}, handleOTAUpload);
    
    // ===== СИСТЕМНЫЕ КОМАНДЫ =====
    server.on("/restart", HTTP_GET, []() {
        if (!isAuthorized()) {
            sendUnauthorized();
            return;
        }
        LOG_INFO("Restart requested");
        server.send(200, "text/plain", "Restarting...");
        delay(100);
        ESP.restart();
    });
    
    server.on("/factory-reset", HTTP_GET, []() {
        if (!isAuthorized()) {
            sendUnauthorized();
            return;
        }
        LOG_WARN("Factory reset requested");
        server.send(200, "text/plain", "Factory reset...");
        delay(100);
        factoryReset();
    });
    
    server.on("/reset-wifi", HTTP_POST, []() {
        if (!isAuthorized()) {
            sendUnauthorized();
            return;
        }
        LOG_WARN("WiFi reset requested");
        preferences.begin("wifi", false);
        preferences.clear();
        preferences.end();
        sendAPI("{\"status\":\"ok\"}");
        delay(500);
        ESP.restart();
    });
    
    // ===== ЛОГИ =====
    server.on("/logs", HTTP_GET, []() {
        if (!isAuthorized()) {
            server.send(401, "text/html", "<script>window.location.href='/login'</script>");
            return;
        }
        
        sendCachedPage(LOGS_PAGE);
    });
    
    server.on("/api/logs", HTTP_GET, []() {
        if (!isAuthorized()) {
            sendUnauthorized();
            return;
        }
        if (!filesystemReady) {
            sendStorageUnavailable();
            return;
        }
        String logs;
        if (!ThermoSmartLogger::getLogs(9999, logs)) {
            server.send(500, "application/json", "{\"error\":\"logs_read_failed\"}");
            return;
        }
        server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
        server.send(200, "text/plain", logs);
    });
    
    server.on("/api/logs/info", HTTP_GET, []() {
        if (!isAuthorized()) {
            sendUnauthorized();
            return;
        }
        if (!filesystemReady) {
            sendStorageUnavailable();
            return;
        }
        String resp = "{\"size\":" + String(ThermoSmartLogger::getLogSize()) + ",";
        int entries = 0;
        if (SPIFFS.exists(LOG_FILE)) {
            File f = SPIFFS.open(LOG_FILE, "r");
            if (!f) {
                server.send(500, "application/json", "{\"error\":\"logs_read_failed\"}");
                return;
            }
            while (f.available()) {
                if (f.read() == '\n') entries++;
            }
            f.close();
        }
        resp += "\"entries\":" + String(entries) + "}";
        sendAPI(resp);
    });
    
    server.on("/api/logs/clear", HTTP_POST, []() {
        if (!isAuthorized()) {
            sendUnauthorized();
            return;
        }
        if (!filesystemReady) {
            sendStorageUnavailable();
            return;
        }
        if (!ThermoSmartLogger::clear()) {
            server.send(500, "application/json", "{\"error\":\"logs_clear_failed\"}");
            return;
        }
        LOG_INFO("Logs cleared");
        sendAPI("{\"status\":\"ok\"}");
    });
    
    // ===== 404 =====
    server.onNotFound([]() {
        if (apModeActive) {
            server.sendHeader("Location", "http://" + WiFi.softAPIP().toString() + "/", true);
            server.send(302, "text/plain", "");
            return;
        }
        if (!isAuthorized()) {
            server.send(401, "text/html", "<script>window.location.href='/login'</script>");
            return;
        }
        server.send(404, "text/plain", "Not found");
    });
    
    server.begin();
    LOG_INFO("HTTP server started with authentication (sessionStorage)");
    LOG_INFO("Default password: %s", DEFAULT_PASSWORD);
}

// ============================================
// AP MODE / FALLBACK
// ============================================
void setupAPMode() {
    apModeActive = true;
    authenticated = false;
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    dnsServer.start(53, "*", WiFi.softAPIP());

    initWebServer();

    LOG_INFO("AP mode started");
    LOG_INFO("IP: %s", WiFi.softAPIP().toString().c_str());
    LOG_INFO("SSID: %s", AP_SSID);
}

#endif // WEB_INTERFACE_H