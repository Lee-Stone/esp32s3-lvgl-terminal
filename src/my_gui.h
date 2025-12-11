#ifndef MY_UI_HH
#define MY_UI_HH

#include <all_data.h>

void my_gui_init(void);
static void my_event_cb_switch(lv_event_t * event);
static void my_event_cb_forward(lv_event_t * event);
static void my_event_cb_back(lv_event_t * event);
static void my_event_cb_left(lv_event_t * event);
static void my_event_cb_right(lv_event_t * event);
static void my_event_cb_textarea(lv_event_t * event);
static void my_event_cb_keyboard(lv_event_t * event);

#endif