#include "config.h"

cfg_set Set = {
    .screen_light = 100,
    .WiFi_scan_one = "",
    .WiFi_scan_all = "",
    .WiFi_button_flag = true,
    .WiFi_name = {'\0'},
    .WiFi_password = "",
    .WiFi_connect_stauts = false,
    .WiFi_connect_flag = false,
    .WiFi_connect_timeout = 10 * 5
};

cfg_data Data = {
    .time_flag = false,
    .weather_flag = false
};

cfg_serial Uart = {
    .tx_data = "",
    .rx_data = ""
};

cfg_music Music = {
    .song_options = "",
    .song_name = {'\0'},
    .song_list = "",
    .play_flag = false,
    .start_flag = false,
    .pause_flag = false,
    .volume_value = 16
};

cfg_game Game = {
    .game2048_flag = false,
    .yang_flag = false,
    .xiaoxiaole_flag = false,
    .pvz_flag = false
};


cfg_xiaozhi Xiaozhi = {
    .question_text = "",
    .answer_text = "",
    .answer_url = "",
    .ask_flag = false,
    .answer_flag = false
};