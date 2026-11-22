// ============================================
// thermostat_buzzer.h
// ============================================
#ifndef THERMOSTAT_BUZZER_H
#define THERMOSTAT_BUZZER_H

#include <Arduino.h>

static int buzzerPin;
static unsigned long buzzerOffTime = 0;
static int buzzerStage = 0;
static int buzzerBeepDuration = 50;
static int buzzerPauseDuration = 50;

void buzzerInit(int pin) {
    buzzerPin = pin;
    pinMode(buzzerPin, OUTPUT);
    digitalWrite(buzzerPin, LOW);
    buzzerStage = 0;
}

void buzzerBeep(int duration) {
    buzzerStage = 0;
    digitalWrite(buzzerPin, HIGH);
    buzzerOffTime = millis() + duration;
}

void buzzerDouble(int beepMs, int pauseMs) {
    buzzerStage = 1;
    buzzerBeepDuration = beepMs;
    buzzerPauseDuration = pauseMs;
    digitalWrite(buzzerPin, HIGH);
    buzzerOffTime = millis() + beepMs;
}

void buzzerClick() {
    buzzerBeep(50);
}

void buzzerDoubleClick() {
    buzzerDouble(50, 50);
}

void buzzerLongBeep() {
    buzzerBeep(1000);
}

void buzzerDoubleLongBeep() {
    buzzerDouble(500, 500);
}

void buzzerUpdate() {
    if (buzzerOffTime == 0) return;
    
    if (millis() >= buzzerOffTime) {
        digitalWrite(buzzerPin, LOW);
        buzzerOffTime = 0;
        
        if (buzzerStage == 1) {
            buzzerStage = 2;
            buzzerOffTime = millis() + buzzerPauseDuration;
        }
        else if (buzzerStage == 2) {
            buzzerStage = 3;
            digitalWrite(buzzerPin, HIGH);
            buzzerOffTime = millis() + buzzerBeepDuration;
        }
        else if (buzzerStage == 3) {
            buzzerStage = 0;
        }
    }
}

#endif