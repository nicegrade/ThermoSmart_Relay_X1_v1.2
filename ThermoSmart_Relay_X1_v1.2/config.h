// ============================================
// thermostat_config.h - ESP32 Relay X1 v1.2
// ============================================
#ifndef THERMOSTAT_CONFIG_H
#define THERMOSTAT_CONFIG_H

#include <Arduino.h>

// ========== Пины для ESP32 Relay X1 v1.2 ==========
#define RELAY_PIN       16      // Реле (GPIO16)
#define LED_PIN         23      // Встроенный светодиод (GPIO23)
#define BUZZER_PIN      22      // Зуммер (GPIO22)
#define BOOT_BUTTON_PIN 0       // Кнопка BOOT (GPIO0) — она же для сброса

// ========== I2C для датчика SHTC3 ==========
#define I2C_SDA_PIN     4       // SDA для SHTC3
#define I2C_SCL_PIN     5       // SCL для SHTC3

// ========== Настройки SHTC3 ==========
#define SHTC3_ADDR              0x70
#define SHTC3_CMD_WAKEUP        0x3517
#define SHTC3_CMD_SLEEP         0xB098
#define SHTC3_CMD_MEASURE_TF    0x7866
#define SHTC3_CMD_MEASURE_HF    0x5C24

// ========== КАЛИБРОВКА ДАТЧИКА ==========
// Корректировка температуры (в градусах Цельсия)
// Положительное значение - увеличивает показания
// Отрицательное - уменьшает
// Если датчик показывает 27, а надо 24 → установите -3.0
#define TEMP_CALIBRATION        -3.0

// Корректировка влажности (в процентах)
#define HUM_CALIBRATION         0.0

// ========== Тайминги ==========
#define SENSOR_READ_INTERVAL    2000
#define SENSOR_FAILURE_THRESHOLD 5
#define SENSOR_ALARM_INTERVAL   5000UL
#define WEB_SERVER_PORT         80
#define AP_TIMEOUT              300000
#define RESET_HOLD_TIME         5000
#define MEMORY_CHECK_INTERVAL   60000

// ========== Настройки по умолчанию ==========
#define DEFAULT_AUTO_TARGET     22.0
#define DEFAULT_HYSTERESIS      0.5
#define DEFAULT_DAY_TEMP        22.0
#define DEFAULT_NIGHT_TEMP      18.0
#define DEFAULT_DAY_START       "07:00"
#define DEFAULT_NIGHT_START     "23:00"
#define DEFAULT_MODE            "auto"
#define DEFAULT_PASSWORD        "admin"
#define AP_SSID                 "ThermoSmart"
#define AP_PASSWORD             "18101990"

// ========== Пределы ==========
#define MIN_TEMP                1.0
#define MAX_TEMP                30.0
#define MIN_HYSTERESIS          0.1
#define MAX_HYSTERESIS          2.0

// ========== Пороги памяти ==========
#define LOW_MEMORY_THRESHOLD     30000
#define CRITICAL_MEMORY_THRESHOLD 20000

#endif