#include "wifiuser.h"
#include "lvgl.h"
#include "ui/src/ui.h"
#include "data.h"
#include <WiFi.h>

void WiFi_update() 
{    
    if(Set.WiFi_connect_flag)WiFi_connect();
    
    if(Set.WiFi_connect_stauts || WiFi.status() == WL_CONNECTED)
    {
        lv_label_set_text_fmt(ui_LabelWiFiState, "已连接 %s", WiFi.SSID());
        lv_obj_add_state(ui_SwitchWiFi, LV_STATE_CHECKED);
        lv_textarea_set_text(ui_TextAreaXiaozhiQuestion, "你能做什么？");
        lv_textarea_set_text(ui_TextAreaXiaoZhiAnswer,
                         "我可以回答各种问题，提供建议，帮助解决问题，生成代码示例，进行翻译，进行文本分析，以及执行各种其他任务。无论是关于编程、学术、日常生活还是其他领域的问题，我都会尽力提供帮助和支持。");
        lv_obj_clear_state(ui_SwitchXiaoZhiSpeak, LV_STATE_DISABLED);
    }
    else
    {
        lv_label_set_text(ui_LabelWiFiState,"未连接");
        if(lv_obj_has_state(ui_SwitchWiFi, LV_STATE_CHECKED))
        {
            lv_obj_clear_state(ui_SwitchWiFi, LV_STATE_CHECKED);
        }
        lv_textarea_set_text(ui_TextAreaXiaozhiQuestion, "请连接WiFi后使用小智功能哦~");
        lv_textarea_set_text(ui_TextAreaXiaoZhiAnswer, "请连接WiFi后使用小智功能哦~");
        lv_obj_add_state(ui_SwitchXiaoZhiSpeak, LV_STATE_DISABLED);
    }

    if(Set.WiFi_button_flag == 1)
    {
        lv_roller_set_options(ui_RollerWiFi, "正在扫描附近WiFi...\n", LV_ROLLER_MODE_NORMAL);
        WiFi_scan();
    }
    else
    {
        lv_roller_set_options(ui_RollerWiFi, Set.WiFi_scan_all.c_str(), LV_ROLLER_MODE_NORMAL);
    }
}

void WiFi_scan()
{
    int n = WiFi.scanNetworks();
    if (n == 0) 
    {
        Set.WiFi_scan_all = "附近无可用WiFi\n";
    } 
    else
    {
        for (int i = 0; i < n; ++i)
        {
            Set.WiFi_scan_one = WiFi.SSID(i) + "\n";
            Set.WiFi_scan_all += Set.WiFi_scan_one;
        }
        Set.WiFi_button_flag = 0;
    }
    if(!Set.WiFi_connect_stauts)WiFi_init(); 
}

void WiFi_connect()
{
    WiFi.begin(Set.WiFi_name, Set.WiFi_password);
    for(int i = 0; i < Set.WiFi_connect_timeout; ++i)
    {
        uint32_t connect_status = WiFi.status();
        // Serial.println(connect_status);
        if(connect_status == WL_CONNECTED)
        {
        	// Serial.println("success");
        	Set.WiFi_connect_stauts = 1;
        	break;
        }
        else Set.WiFi_connect_stauts = 0;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    if(Set.WiFi_connect_stauts)
    {
        if(!Data.time_flag){time_get();Data.time_flag = 1;}
        if(!Data.weather_flag)Data.weather_flag = 1;

        lv_obj_add_state(ui_SwitchWiFi, LV_STATE_CHECKED);
        lv_textarea_set_text(ui_TextAreaXiaozhiQuestion, "你能做什么？");
        lv_textarea_set_text(ui_TextAreaXiaoZhiAnswer,
                         "我可以回答各种问题，提供建议，帮助解决问题，生成代码示例，进行翻译，进行文本分析，以及执行各种其他任务。无论是关于编程、学术、日常生活还是其他领域的问题，我都会尽力提供帮助和支持。");
        lv_obj_clear_state(ui_SwitchXiaoZhiSpeak, LV_STATE_DISABLED);
    }
    else
    {
        if(lv_obj_has_state(ui_SwitchWiFi, LV_STATE_CHECKED))
        {
            lv_obj_clear_state(ui_SwitchWiFi, LV_STATE_CHECKED);
        }
        lv_textarea_set_text(ui_TextAreaXiaozhiQuestion, "请连接WiFi后使用小智功能哦~");
        lv_textarea_set_text(ui_TextAreaXiaoZhiAnswer, "请连接WiFi后使用小智功能哦~");
        lv_obj_add_state(ui_SwitchXiaoZhiSpeak, LV_STATE_DISABLED);
    }
    lv_obj_add_flag(ui_SpinnerWiFi, LV_OBJ_FLAG_HIDDEN);
    Set.WiFi_connect_flag = 0;
}

void WiFi_init()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin();
    for(int i = 0; i < Set.WiFi_connect_timeout; ++i)
    {
        uint32_t connect_status = WiFi.status();
        // Serial.println(connect_status);
        if(connect_status == WL_CONNECTED)
        {
        	// Serial.println("success");
        	Set.WiFi_connect_stauts = 1;
        	break;
        }
        else Set.WiFi_connect_stauts = 0;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    if(Set.WiFi_connect_stauts)
    {
        if(!Data.time_flag){time_get();Data.time_flag = 1;}
        if(!Data.weather_flag)Data.weather_flag = 1;

        lv_obj_add_state(ui_SwitchWiFi, LV_STATE_CHECKED);
        lv_textarea_set_text(ui_TextAreaXiaozhiQuestion, "你能做什么？");
        lv_textarea_set_text(ui_TextAreaXiaoZhiAnswer,
                         "我可以回答各种问题，提供建议，帮助解决问题，生成代码示例，进行翻译，进行文本分析，以及执行各种其他任务。无论是关于编程、学术、日常生活还是其他领域的问题，我都会尽力提供帮助和支持。");
        lv_obj_clear_state(ui_SwitchXiaoZhiSpeak, LV_STATE_DISABLED);
    }
    else
    {
        if(lv_obj_has_state(ui_SwitchWiFi, LV_STATE_CHECKED))
        {
            lv_obj_clear_state(ui_SwitchWiFi, LV_STATE_CHECKED);
        }
        lv_textarea_set_text(ui_TextAreaXiaozhiQuestion, "请连接WiFi后使用小智功能哦~");
        lv_textarea_set_text(ui_TextAreaXiaoZhiAnswer, "请连接WiFi后使用小智功能哦~");
        lv_obj_add_state(ui_SwitchXiaoZhiSpeak, LV_STATE_DISABLED);
    }
}
