#ifndef __ALL_EXTERN_H__
#define __ALL_EXTERN_H__

#include <Arduino.h>

#define TASK_STATE_MAIN   0
#define TASK_STATE_SERIAL 1
#define TASK_STATE_WIFI   2
#define TASK_STATE_MUSIC  3
#define TASK_STATE_MIC    4

extern uint8_t task_state;
extern bool init_flag;

extern bool wifi_scan_flag;
extern bool wifi_connect_flag;
extern bool wifi_button_flag;
extern bool isconnect;
extern bool time_flag;
extern char wifi_name[64];
extern const char * wifi_password;
extern struct tm timeinfo;
extern bool weather_flag;

extern const char * songoptions;
extern char songname[64];
extern bool start_flag;
extern bool play_flag;
extern int voice;

extern bool mic_flag;
extern uint8_t mic2_flag;
extern bool baidu_audio_flag;
extern const char *baidu_audio_host;

extern bool serial_flag;
extern char * rate_buf;

extern bool game_yang_flag;
extern bool game_2048_flag;
extern bool game_xiaole_flag;
extern bool game_pvz_flag;

#endif
