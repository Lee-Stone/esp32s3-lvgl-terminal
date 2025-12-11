#include "lv_games.h"

void ui_gameson_init(void)
{
    ui_GameSonBackToGame = lv_imgbtn_create(ui_GameSon);
    lv_imgbtn_set_src(ui_GameSonBackToGame, LV_IMGBTN_STATE_RELEASED, NULL, &ui_img_123003138, NULL);
    lv_imgbtn_set_src(ui_GameSonBackToGame, LV_IMGBTN_STATE_PRESSED, NULL, &ui_img_684857571, NULL);
    lv_obj_set_height(ui_GameSonBackToGame, 64);
    lv_obj_set_width(ui_GameSonBackToGame, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_x(ui_GameSonBackToGame, 2);
    lv_obj_set_y(ui_GameSonBackToGame, 8);

    lv_obj_add_event_cb(ui_GameSonBackToGame, ui_event_GameSonBackToGame, LV_EVENT_ALL, NULL);
}

void ui_gameson_clear(void)
{
    lv_obj_clean(ui_GameSon);
}