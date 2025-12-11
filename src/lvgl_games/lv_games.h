/**
 * @file lv_games.h
 * @souce https://gitee.com/mgod_wu/AiPi-Eyes-Rx
 * @souce https://gitee.com/mgod_wu/AiPi-Eyes-Rx
 */

#ifndef LV_GAMES_H
#define LV_GAMES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"
#include <ui/src/ui.h>

#include "lv_100ask_2048/game2048.h"
#include "yang/yang.h"
#include "xiaoxiaole/xiaoxiaole.h"
#include "pvz/pvz.h"

void ui_gameson_init(void);
void ui_gameson_clear(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*LV_DEMO_H*/
