SET(SOURCES screens/ui_Main.c
    screens/ui_Set.c
    screens/ui_Serial.c
    screens/ui_Weather.c
    screens/ui_Music.c
    screens/ui_GPT.c
    screens/ui_Game.c
    screens/ui_Cal.c
    screens/ui_About.c
    ui.c
    components/ui_comp_hook.c
    ui_helpers.c
    images/ui_img_set_released_png.c
    images/ui_img_set_pressed_png.c
    images/ui_img_serial_released_png.c
    images/ui_img_serial_pressed_png.c
    images/ui_img_weather_released_png.c
    images/ui_img_weather_pressed_png.c
    images/ui_img_music_released_png.c
    images/ui_img_music_pressed_png.c
    images/ui_img_gpt_released_png.c
    images/ui_img_gpt_pressed_png.c
    images/ui_img_game_released_png.c
    images/ui_img_game_pressed_png.c
    images/ui_img_cal_released_png.c
    images/ui_img_about_released_png.c
    images/ui_img_back_released_png.c
    images/ui_img_back_pressed_png.c
    fonts/ui_font_AlimamaShuHeiTiFont16Bpp4.c)

add_library(ui ${SOURCES})
