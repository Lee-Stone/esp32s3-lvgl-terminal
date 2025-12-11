#ifndef TASK_H
#define TASK_H

#include "config.h"

void task_init();
void WiFiTask_create();
void serialTask_create();
void xiaozhiTask_create();
void WiFiTask_delete();
void serialTask_delete();
void xiaozhiTask_delete();

void lvgl_task(void *pt);
void WiFi_task(void *pt);
void data_task(void *pt);
void serial_task(void *pt);
void music_task(void *pt);
void xiaozhi_task(void *pt);

#endif // TASK_H
