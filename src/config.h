#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <WiFi.h>
#include <string.h>
#include <lv_games.h>

typedef struct {
    int screen_light;
    String WiFi_scan_one;
    String WiFi_scan_all;
    bool WiFi_button_flag;
    char WiFi_name[64];
    const char* WiFi_password;
    bool WiFi_connect_stauts;
    bool WiFi_connect_flag;
    int WiFi_connect_timeout;
} cfg_set;

typedef struct {
    bool time_flag;
    bool weather_flag;
} cfg_data;

typedef struct {
    const char* tx_data;
    const char* rx_data;
} cfg_serial;

typedef struct {
    const char* song_options;
    char song_name[64];
    String song_list;
    bool play_flag;
    bool start_flag;
    bool pause_flag;
    int volume_value;
} cfg_music;

typedef struct {
    bool game2048_flag;
    bool yang_flag;
    bool xiaoxiaole_flag;
    bool pvz_flag;
} cfg_game;

typedef struct {
    String question_text;
    String answer_text;
    String answer_url;
    bool ask_flag;
    bool answer_flag;
} cfg_xiaozhi;

extern cfg_set Set;
extern cfg_data Data;
extern cfg_serial Uart;
extern cfg_music Music;
extern cfg_game Game;
extern cfg_xiaozhi Xiaozhi;

#endif // CONFIG_H