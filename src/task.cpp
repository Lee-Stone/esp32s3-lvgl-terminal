#include "task.h"
#include "ui/src/ui.h"  
#include "wifiuser.h"
#include "data.h"
#include "sduser.h"
#include "music.h"
#include "xiaozhi_ai.h"
#include "xiaozhi.h"

TaskHandle_t WiFiHandle = nullptr;
TaskHandle_t serialHandle = nullptr;
TaskHandle_t xiaozhiHandle = nullptr;

void task_init()
{
    xTaskCreatePinnedToCore(lvgl_task, "lvgl_task", 1024*30, NULL, 2, NULL, 1);
    xTaskCreatePinnedToCore(data_task, "data_task", 1024*5, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(music_task, "music_task", 1024*10, NULL, 1, NULL, 0);
}

void WiFiTask_create()
{
    if(WiFiHandle == nullptr)
    {
        xTaskCreatePinnedToCore(WiFi_task, "WiFi_task", 1024*5, NULL, 1, &WiFiHandle, 0);
    }
}

void serialTask_create()
{
    if(serialHandle == nullptr)
    {
        xTaskCreatePinnedToCore(serial_task, "serial_task", 1024*5, NULL, 1, &serialHandle, 0);
    }
}

void xiaozhiTask_create()
{
    if(xiaozhiHandle == nullptr)
    {
        xTaskCreatePinnedToCore(xiaozhi_task, "xiaozhi_task", 1024*5, NULL, 1, &xiaozhiHandle, 0);
    }
}

void WiFiTask_delete()
{
    if(WiFiHandle != nullptr)
    {
        Set.WiFi_button_flag = true;
        
        vTaskDelete(WiFiHandle);
        WiFiHandle = nullptr;
    }
}

void serialTask_delete()
{
    if(serialHandle != nullptr)
    {
        vTaskDelete(serialHandle);
        serialHandle = nullptr;
    }
}

void xiaozhiTask_delete()
{
    if(xiaozhiHandle != nullptr)
    {
        Xiaozhi.ask_flag = false;
        Xiaozhi.answer_flag = false;
        if(lv_obj_has_state(ui_SwitchXiaoZhiSpeak, LV_STATE_CHECKED))
        {
            lv_obj_clear_state(ui_SwitchXiaoZhiSpeak, LV_STATE_CHECKED);
        } 

        vTaskDelete(xiaozhiHandle);
        xiaozhiHandle = nullptr;
    }
}

void lvgl_task(void *pt)
{
    while(1)
    {
        lv_timer_handler();  
        vTaskDelay(5);      
    }
}

void WiFi_task(void *pt)
{
    while(1)
    {
        WiFi_update();
        vTaskDelay(50);
    }
}

void data_task(void *pt)
{
    while(1)
    {
        data_update();
        vTaskDelay(1000);
    }
}

void serial_task(void *pt)
{
    while(1)
    {
        if(Serial.available()) 
        {
            String RX = Serial.readStringUntil('\n');
            Uart.rx_data = RX.c_str();
            lv_textarea_add_text(ui_TextAreaRX, Uart.rx_data);
        }
        vTaskDelay(50);
    }   
}

void music_task(void *pt)
{
    while(1)
    {
        music_update();
        vTaskDelay(5);
    }
}

void xiaozhi_task(void *pt)
{
    while(1)
    {
        xiaozhi_update();
        vTaskDelay(5);
    }
}