#include "wifiuser.h"
#include <StreamString.h>

#define wifi_connect_time 10*5
#define wifi_init_time 10*3

String scan_WiFi;
String all_WiFi;

bool isconnect = 0;
bool time_flag = 0;
bool weather_flag = 0;
bool wifi_scan_flag = 1;
bool wifi_button_flag = 0;
bool wifi_connect_flag = 0;

char wifi_name[64];
const char * wifi_password;

void WiFiUpdate(void) 
{
    if(wifi_scan_flag)scanWiFi();
    if(wifi_connect_flag)connectWiFi();
    
    if(wifi_button_flag)
    {
        WiFi.disconnect();isconnect = 0;
        lv_roller_set_options( ui_RollerWiFi, "正在搜索可用的网络...\n", LV_ROLLER_MODE_NORMAL );
        lv_obj_add_flag( ui_EnterPassword, LV_OBJ_FLAG_HIDDEN ); 
    }
    else
    {
        if(WiFi.status() == WL_DISCONNECTED)isconnect=0;
        lv_roller_set_options( ui_RollerWiFi, all_WiFi.c_str(), LV_ROLLER_MODE_NORMAL );
        lv_obj_clear_flag( ui_EnterPassword, LV_OBJ_FLAG_HIDDEN );
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

void scanWiFi(void)
{
    int n = WiFi.scanNetworks();
    if (n == 0) 
    {
        lv_roller_set_options( ui_RollerWiFi, "无可用网络\n", LV_ROLLER_MODE_NORMAL );
    } 
    else
    {
        for (int i = 0; i < n; ++i)
        {
            scan_WiFi=WiFi.SSID(i)+"\n";
            all_WiFi+=scan_WiFi;
        }
        if(!isconnect)initWiFi(); //自动连接上一次WiFi
        strcpy(wifi_name, WiFi.SSID().c_str());
        wifi_scan_flag = 0;wifi_button_flag = 0;
    }
}

void connectWiFi(void)
{
    WiFi.begin(wifi_name,wifi_password);
    for(int i = 0; i < wifi_connect_time; ++i)
    {
        uint32_t connect_status = WiFi.status();
        Serial.println(connect_status);
        if(connect_status == WL_CONNECTED){Serial.println("success");isconnect = 1;break;}
        else isconnect = 0;
        delay(100);
    }
    if(isconnect)
    {
        if(time_flag == 0){get_time_init();time_flag = 1;}
        if(weather_flag == 0)weather_flag = 1;
    }
    else if(isconnect == 0)WiFi.disconnect();
    lv_obj_add_flag(ui_SpinnerWaitWiFi, LV_OBJ_FLAG_HIDDEN);
    wifi_connect_flag = 0;
}

void initWiFi(void)
{
    WiFi.begin();
    for(int i = 0; i < wifi_init_time; ++i)
    {
        uint32_t connect_status = WiFi.status();
        Serial.println(connect_status);
        if(connect_status == WL_CONNECTED){Serial.println("success");isconnect = 1;break;}
        else isconnect = 0;

        delay(100);
    }
    if(isconnect)
    {
        if(time_flag == 0){get_time_init();time_flag = 1;}
        if(weather_flag == 0)weather_flag = 1;

        // if(init_flag == 0)init_flag = 1;
    }
    else if(isconnect == 0){WiFi.disconnect();}
}