#ifndef __ALL_DATA_H__
#define __ALL_DATA_H__

#include <Arduino.h>

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <CST816S.h>

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <base64.h>
#include <UrlEncode.h>
#include <string.h>
#include <ArduinoWebsockets.h>
#include <WiFiClientSecure.h>

#include <SPI.h>
#include <driver/i2s.h>
#include <Audio.h>
#include <SD.h>

#include <ui/src/ui.h>
#include <my_gui.h>

#include <all_extern.h>

void init_clear(void);

void WiFiUpdate(void); //lv_timer_t * timer
void scanWiFi(void);
void connectWiFi(void);
void initWiFi(void);

void get_time_weather(void); //lv_timer_t * timer
void get_time_init(void);
void get_weather(void);

void initSD(void);
// void lv_fs_test(void);

void initmusic(void);
void readmusic(void);
void getmusic(void);//lv_timer_t * timer
extern Audio audio;

void initmic(void);
void getmic(void);
String gainToken(void);
void sendToSTT(void);
void getToSTT(void);
void aseembleJson(String token);
void removeBrackets(String str);
void initmic2(void);
void getmic2(void);

String get_GPTanswer(String question);
void GPTtest(void);

void lv_100ask_2048_simple_test(void);

#endif