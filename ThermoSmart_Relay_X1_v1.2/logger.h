// ============================================
// thermostat_logger.h - Система логирования в SPIFFS
// ТОЛЬКО ВАЖНЫЕ СОБЫТИЯ
// ============================================
#ifndef THERMOSTAT_LOGGER_H
#define THERMOSTAT_LOGGER_H

#include <Arduino.h>
#include <SPIFFS.h>
#include <time.h>

#define LOG_FILE "/logs.txt"
#define MAX_LOG_SIZE (20 * 1024)   // 20 KB

enum LogLevel {
    LOG_INFO = 0,
    LOG_WARN = 1,
    LOG_ERROR = 2
};

class ThermoSmartLogger {
private:
    static bool _enabled;
    static unsigned long _lastFlush;
    static String _buffer;
    static int _bufferLines;
    static const char* _levelNames[3];

    static String getTimestamp() {
        time_t now = time(nullptr);
        struct tm* t = localtime(&now);
        char buf[32];
        if (now >= 100000 && t && strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t) > 0) {
            return String(buf);
        } else {
            unsigned long ms = millis();
            unsigned long sec = ms / 1000;
            unsigned long min = sec / 60;
            unsigned long hours = min / 60;
            snprintf(buf, sizeof(buf), "uptime %lud %02lu:%02lu:%02lu", hours / 24, hours % 24, min % 60, sec % 60);
        }
        return String(buf);
    }

    static void rotateLog() {
        if (!SPIFFS.exists(LOG_FILE)) return;
        
        File f = SPIFFS.open(LOG_FILE, "r");
        if (!f) {
            Serial.println("ERROR: Cannot inspect log file before rotation");
            return;
        }
        
        size_t size = f.size();
        f.close();
        
        if (size < MAX_LOG_SIZE) return;
        
        if (SPIFFS.exists(LOG_FILE ".bak")) {
            if (!SPIFFS.remove(LOG_FILE ".bak")) {
                Serial.println("ERROR: Cannot remove previous log backup");
                return;
            }
        }
        if (!SPIFFS.rename(LOG_FILE, LOG_FILE ".bak")) {
            Serial.println("ERROR: Cannot rotate active log");
            return;
        }
        
        File newLog = SPIFFS.open(LOG_FILE, "w");
        if (!newLog) {
            Serial.println("ERROR: Cannot create new active log");
            return;
        }
        newLog.println("=== Log rotated ===");
        newLog.close();
    }

public:
    static void begin(bool storageAvailable = true) {
        _enabled = storageAvailable;
        _lastFlush = 0;
        _buffer.reserve(1024);
        _bufferLines = 0;

        if (!_enabled) {
            Serial.println("ERROR: Log storage unavailable; filesystem was not formatted");
            return;
        }
        
        if (!SPIFFS.exists(LOG_FILE)) {
            File f = SPIFFS.open(LOG_FILE, "w");
            if (f) {
                f.println("=== ThermoSmart Log Started ===");
                f.close();
            } else {
                Serial.println("ERROR: Cannot create log file");
            }
        }
    }

    static void enable() { _enabled = true; }
    static void disable() { _enabled = false; }
    static bool isEnabled() { return _enabled; }

    static void log(LogLevel level, const char* format, ...) {
        if (!_enabled) return;
        
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        
        String timestamp = getTimestamp();
        String entry = "[" + timestamp + "] [" + String(_levelNames[level]) + "] " + String(buffer) + "\n";
        
        _buffer += entry;
        _bufferLines++;
        
        if (_bufferLines > 5) {
            flush();
        }
    }

    static void info(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        log(LOG_INFO, "%s", buffer);
    }

    static void warn(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        log(LOG_WARN, "%s", buffer);
    }

    static void error(const char* format, ...) {
        char buffer[256];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        log(LOG_ERROR, "%s", buffer);
    }

    // Специальная функция для логирования состояния реле
    static void logRelay(bool state, float temp, float hum, float target) {
        if (!_enabled) return;
        
        String status = state ? "ON" : "OFF";
        String entry = "[" + getTimestamp() + "] [RELAY] " + status + 
                       " | T: " + String(temp, 1) + "°C" +
                       " | H: " + String(hum, 0) + "%" +
                       " | Target: " + String(target, 1) + "°C\n";
        
        _buffer += entry;
        _bufferLines++;
        
        if (_bufferLines > 5) {
            flush();
        }
    }

    static void flush() {
        if (!_enabled || _buffer.length() == 0) return;
        
        rotateLog();
        
        File f = SPIFFS.open(LOG_FILE, "a");
        if (!f) {
            Serial.println("ERROR: Cannot open log file for writing; retaining buffered entries");
            return;
        }
        size_t expected = _buffer.length();
        size_t written = f.print(_buffer);
        f.close();
        if (written != expected) {
            Serial.printf("ERROR: Incomplete log write (%u of %u bytes); retaining buffer\n",
                          static_cast<unsigned int>(written),
                          static_cast<unsigned int>(expected));
            int writtenLines = 0;
            for (size_t i = 0; i < written; i++) {
                if (_buffer[i] == '\n') writtenLines++;
            }
            _buffer.remove(0, written);
            _bufferLines -= writtenLines;
            if (_bufferLines < 0) _bufferLines = 0;
            return;
        }
        _buffer = "";
        _bufferLines = 0;
        _lastFlush = millis();
    }

    static bool getLogs(int limit, String& output) {
    if (!_enabled) return false;
    if (!SPIFFS.exists(LOG_FILE)) {
        output = "No logs available\n";
        return true;
    }
    
    File f = SPIFFS.open(LOG_FILE, "r");
    if (!f) {
        Serial.println("ERROR: Cannot open log file for reading");
        return false;
    }
    
    size_t size = f.size();
    if (size == 0) {
        f.close();
        output = "Log file is empty\n";
        return true;
    }
    if (limit <= 0) {
        f.close();
        output = "";
        return true;
    }
    
    // Читаем весь файл и берем последние N строк
    output = f.readString();
    f.close();
    if (output.length() != size) {
        Serial.println("ERROR: Incomplete log read");
        return false;
    }
    
    // Разбиваем на строки
    int lineCount = 0;
    int pos = output.length() - 1;
    
    // Ищем с конца
    for (int i = output.length() - 1; i >= 0 && lineCount < limit; i--) {
        if (output[i] == '\n') {
            lineCount++;
            if (lineCount == limit) {
                pos = i + 1;
                break;
            }
        }
    }
    
    // Если строк меньше чем лимит
    if (lineCount < limit) {
        pos = 0;
    }
    
    output.remove(0, pos);
    if (output.length() == 0) output = "No logs found\n";
    return true;
}

    static bool clear() {
        if (!_enabled) return false;
        if (SPIFFS.exists(LOG_FILE)) {
            if (!SPIFFS.remove(LOG_FILE)) {
                Serial.println("ERROR: Cannot remove active log");
                return false;
            }
        }
        if (SPIFFS.exists(LOG_FILE ".bak")) {
            if (!SPIFFS.remove(LOG_FILE ".bak")) {
                Serial.println("ERROR: Cannot remove log backup");
                return false;
            }
        }
        File f = SPIFFS.open(LOG_FILE, "w");
        if (!f) {
            Serial.println("ERROR: Cannot recreate log file");
            return false;
        }
        f.println("=== Log cleared ===");
        f.close();
        _buffer = "";
        _bufferLines = 0;
        return true;
    }

    static size_t getLogSize() {
        if (!_enabled) return 0;
        if (!SPIFFS.exists(LOG_FILE)) return 0;
        File f = SPIFFS.open(LOG_FILE, "r");
        if (!f) return 0;
        size_t size = f.size();
        f.close();
        return size;
    }
};

// Статические переменные
bool ThermoSmartLogger::_enabled = true;
unsigned long ThermoSmartLogger::_lastFlush = 0;
String ThermoSmartLogger::_buffer = "";
int ThermoSmartLogger::_bufferLines = 0;
const char* ThermoSmartLogger::_levelNames[3] = {"INFO", "WARN", "ERROR"};

// ============================================
// МАКРОСЫ
// ============================================
#define LOG_INFO(...) ThermoSmartLogger::info(__VA_ARGS__)
#define LOG_WARN(...) ThermoSmartLogger::warn(__VA_ARGS__)
#define LOG_ERROR(...) ThermoSmartLogger::error(__VA_ARGS__)
#define LOG_RELAY(state, temp, hum, target) ThermoSmartLogger::logRelay(state, temp, hum, target)

#endif