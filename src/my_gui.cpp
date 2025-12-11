#include <my_gui.h>

bool init_flag = 0;
void init_clear(void)
{
    lv_obj_clear_flag( ui_PanelTime, LV_OBJ_FLAG_HIDDEN );   
    lv_obj_clear_flag( ui_PanelInit, LV_OBJ_FLAG_HIDDEN );   

    if(wifi_button_flag)
    {
        WiFi.disconnect();isconnect = 0;
        lv_roller_set_options( ui_RollerWiFi, "正在搜索可用的网络...\n", LV_ROLLER_MODE_NORMAL );
        lv_obj_add_flag( ui_EnterPassword, LV_OBJ_FLAG_HIDDEN ); 
    }

    if(isconnect)
    {
        lv_obj_clear_flag( ui_WiFiNoConnect, LV_OBJ_FLAG_HIDDEN );
        lv_obj_add_flag( ui_WiFiIsConnect, LV_OBJ_FLAG_HIDDEN );
        lv_label_set_text_fmt(ui_ConnectStatus,"已连接 %s", WiFi.SSID());
        lv_obj_clear_flag( ui_ConnectStatus, LV_OBJ_FLAG_HIDDEN ); 

        lv_obj_add_flag( ui_LabelNoWiFI, LV_OBJ_FLAG_HIDDEN );   
        lv_obj_clear_flag( ui_TabWeather, LV_OBJ_FLAG_HIDDEN );

        lv_obj_add_flag( ui_LabelNoWiFi, LV_OBJ_FLAG_HIDDEN );   
        lv_obj_clear_flag( ui_GPTQuestion, LV_OBJ_FLAG_HIDDEN );
        lv_obj_clear_flag( ui_TextAreaQuestion, LV_OBJ_FLAG_HIDDEN );
        lv_obj_clear_flag( ui_TextAreaAnswer, LV_OBJ_FLAG_HIDDEN );
        lv_obj_clear_flag( ui_GPTAnswer, LV_OBJ_FLAG_HIDDEN );
        lv_obj_clear_flag( ui_SwitchSpeak, LV_OBJ_FLAG_HIDDEN );
    }
    else
    {
        lv_obj_clear_flag( ui_WiFiIsConnect, LV_OBJ_FLAG_HIDDEN );
        lv_obj_add_flag( ui_WiFiNoConnect, LV_OBJ_FLAG_HIDDEN );

        lv_label_set_text(ui_ConnectStatus,"未连接");
        lv_obj_clear_flag( ui_ConnectStatus, LV_OBJ_FLAG_HIDDEN );  

        lv_obj_add_flag( ui_TabWeather, LV_OBJ_FLAG_HIDDEN );
        lv_obj_clear_flag( ui_LabelNoWiFI, LV_OBJ_FLAG_HIDDEN );   

        lv_obj_clear_flag( ui_LabelNoWiFi, LV_OBJ_FLAG_HIDDEN );   
        lv_obj_add_flag( ui_GPTQuestion, LV_OBJ_FLAG_HIDDEN );
        lv_obj_add_flag( ui_TextAreaQuestion, LV_OBJ_FLAG_HIDDEN );
        lv_obj_add_flag( ui_TextAreaAnswer, LV_OBJ_FLAG_HIDDEN );
        lv_obj_add_flag( ui_GPTAnswer, LV_OBJ_FLAG_HIDDEN );
        lv_obj_add_flag( ui_SwitchSpeak, LV_OBJ_FLAG_HIDDEN );
    }
}

lv_obj_t *ta;
lv_obj_t *kb;
lv_obj_t *tabview;

void my_gui_init(void)
{
    tabview = lv_tabview_create(lv_scr_act(), LV_DIR_TOP, 0);
    lv_obj_t *tab1 = lv_tabview_add_tab(tabview, "Tab 1");
    lv_obj_t *tab2 = lv_tabview_add_tab(tabview, "Tab 2");
    lv_obj_t *tab3 = lv_tabview_add_tab(tabview, "Tab 3");
    lv_obj_t *tab_btn =lv_tabview_get_tab_btns(tabview);
    lv_obj_set_style_border_width(tab_btn, 0, LV_PART_ITEMS| LV_STATE_CHECKED);

    lv_obj_t *sw1 = lv_switch_create(tab2);
    lv_obj_set_size(sw1, 80, 50);
    lv_obj_align(sw1, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(sw1, my_event_cb_switch,LV_EVENT_ALL,NULL);

    lv_obj_t *btn1 = lv_btn_create(tab2);
    lv_obj_set_size(btn1, 80, 50);
    lv_obj_align_to(btn1, sw1, LV_ALIGN_OUT_TOP_MID, 0, -30);
    lv_obj_add_event_cb(btn1, my_event_cb_forward, LV_EVENT_ALL,NULL);

    lv_obj_t *btn2 = lv_btn_create(tab2);
    lv_obj_set_size(btn2, 80, 50);
    lv_obj_align_to(btn2, sw1, LV_ALIGN_OUT_BOTTOM_MID, 0, 30);
    lv_obj_add_event_cb(btn2, my_event_cb_back, LV_EVENT_ALL,NULL);

    lv_obj_t *btn3 = lv_btn_create(tab2);
    lv_obj_set_size(btn3, 80, 50);
    lv_obj_align_to(btn3, sw1, LV_ALIGN_OUT_LEFT_MID, -30, 0);
    lv_obj_add_event_cb(btn3, my_event_cb_left,LV_EVENT_ALL,NULL);

    lv_obj_t *btn4 = lv_btn_create(tab2);
    lv_obj_set_size(btn4, 80, 50);
    lv_obj_align_to(btn4, sw1, LV_ALIGN_OUT_RIGHT_MID, 30, 0);
    lv_obj_add_event_cb(btn4, my_event_cb_right, LV_EVENT_ALL, NULL);

    lv_obj_t *label1 = lv_label_create(tab2);
    lv_label_set_recolor(label1, true);
    lv_label_set_text_fmt(label1, "#ff0000 Forward#");
    lv_obj_align_to(label1, btn1, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label2 = lv_label_create(tab2);
    lv_label_set_recolor(label2, true);
    lv_label_set_text_fmt(label2, "#ff0000 Back#");
    lv_obj_align_to(label2, btn2, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label3 = lv_label_create(tab2);
    lv_label_set_recolor(label3, true);
    lv_label_set_text_fmt(label3, "#ff0000 Left#");
    lv_obj_align_to(label3, btn3, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *label4 = lv_label_create(tab1);
    lv_label_set_recolor(label4, true);
    lv_label_set_text_fmt(label4, "#ff0000 Right#");
    lv_obj_align_to(label4, btn4, LV_ALIGN_CENTER, 0, 0);

    ta = lv_textarea_create(tab1);
    lv_obj_set_size(ta,200,80);
    lv_obj_add_event_cb(ta, my_event_cb_textarea, LV_EVENT_ALL, NULL);
    kb = lv_keyboard_create(tab1);
    lv_keyboard_set_textarea(kb, ta);
    lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);	
    lv_obj_add_event_cb(kb, my_event_cb_keyboard, LV_EVENT_ALL, NULL);
}

static void my_event_cb_switch(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t* obj = lv_event_get_target(event);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        printf("%s\r\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
    }
}

static void my_event_cb_forward(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_PRESSING)printf("Forward\r\n");
    else if(code == LV_EVENT_RELEASED)printf("Stop\r\n");
}

static void my_event_cb_back(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_PRESSING)printf("Back\r\n");
    else if(code == LV_EVENT_RELEASED)printf("Stop\r\n");
}

static void my_event_cb_left(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_PRESSING)printf("Left\r\n");
    else if(code == LV_EVENT_RELEASED)printf("Stop\r\n");
}

static void my_event_cb_right(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);

    if(code == LV_EVENT_PRESSING)printf("Right\r\n");
    else if(code == LV_EVENT_RELEASED)printf("Stop\r\n");
}

const char *txt_ta,*txt_kb;
static void my_event_cb_textarea(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t *target = lv_event_get_target(event);

    if(code == LV_EVENT_VALUE_CHANGED)
    {
        txt_ta = lv_textarea_get_text(target);
    }
    else if(code == LV_EVENT_PRESSED)
    {
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);;
    }
    else if(code == LV_EVENT_DEFOCUSED)
    {
        lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
    }
}
static void my_event_cb_keyboard(lv_event_t * event)
{
    lv_event_code_t code = lv_event_get_code(event);
    lv_obj_t *target = lv_event_get_target(event);

    if(code == LV_EVENT_VALUE_CHANGED)
    {
        uint16_t id = lv_btnmatrix_get_selected_btn(target);
        txt_kb = lv_btnmatrix_get_btn_text(target, id);

        if(strcmp(txt_kb, LV_SYMBOL_KEYBOARD) == 0)
        {
            //if(lv_keyboard_get_mode(target) == LV_KEYBOARD_MODE_NUMBER)lv_keyboard_set_mode(target, LV_KEYBOARD_MODE_TEXT_LOWER);
            //else lv_keyboard_set_mode(target, LV_KEYBOARD_MODE_NUMBER);
            lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
        }
        else if(strcmp(txt_kb, LV_SYMBOL_OK) == 0)
        {
            printf("%s\r\n",txt_ta);
        }
    }
}
