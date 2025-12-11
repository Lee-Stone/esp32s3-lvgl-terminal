#include <weather.h>

//获取时间
struct tm timeinfo;

//获取天气
String url = "https://api.seniverse.com/v3/weather/now.json";
String key = "SHdGnMJ4MzuCuy0-L";
String locations = "深圳";
String language = "zh-Hans";
String unit = "c";

String url_2 = "https://api.seniverse.com/v3/weather/daily.json";
String start = "0";
String days = "7";
#define get_days 7

String city,temp,text,humi,last_update;
String data,date,text_day,low,high,precip;
String week_weather[7]={"周日","周一","周二","周三","周四","周五","周六"};

void get_time_init(void)
{   
    configTime(28800, 0,"ntp1.aliyun.com","ntp2.aliyun.com", "ntp3.aliyun.com");  //阿里云服务器
}

void get_weather(void)
{
    // 创建 HTTPClient 对象
    HTTPClient http;
    HTTPClient http_2;

    // 发送GET请求
    http.begin(url+"?key="+key+"&location="+locations+"&language="+language+"&unit="+unit);
    http_2.begin(url_2+"?key="+key+"&location="+locations+"&language="+language+"&unit="+unit+"&start="+start+"&days="+days);

    int httpCode = http.GET();
    int httpCoed_2 = http_2.GET();

    // 获取响应正文
    String response = http.getString();
    String response_2 = http_2.getString();
    // Serial.println(response);
    // Serial.println(response_2);

    http.end();
    http_2.end();

    // 创建 DynamicJsonDocument 对象
    DynamicJsonDocument doc(1024);
    DynamicJsonDocument doc_2(512*get_days);

    // 解析 JSON 数据
    deserializeJson(doc, response);
    deserializeJson(doc_2, response_2);

    // 从解析后的 JSON 文档中获取值
    city = doc["results"][0]["location"]["name"].as<String>();
    temp = doc["results"][0]["now"]["temperature"].as<String>();
    text = doc["results"][0]["now"]["text"].as<String>();
    humi = doc["results"][0]["now"]["humidity"].as<String>();
    last_update = doc["results"][0]["last_update"].as<String>();

    text_day = doc_2["results"][0]["daily"][1]["text_day"].as<String>();
    low = doc_2["results"][0]["daily"][1]["low"].as<String>();
    high = doc_2["results"][0]["daily"][1]["high"].as<String>();
    precip = doc_2["results"][0]["daily"][1]["precip"].as<String>();

    data = "\0";
    for(int i = 0; i < get_days; ++i)
    {   
        if(i == 0)date = "今天";
        else date = week_weather[(timeinfo.tm_wday+i<=6) ? (timeinfo.tm_wday+i) : (timeinfo.tm_wday+i-7)];
        data += 
        date+
        " "+doc_2["results"][0]["daily"][i]["text_day"].as<String>()+
        " 温度:"+doc_2["results"][0]["daily"][i]["low"].as<String>()+
        "~"+doc_2["results"][0]["daily"][i]["high"].as<String>()+
        " 降水:"+doc_2["results"][0]["daily"][i]["precip"].as<String>()+
        "\n";
    }
    
    lv_label_set_text_fmt( ui_LabelLocation, "地点:%s", city.c_str());
    lv_label_set_text_fmt( ui_LabelTemp, "温度:%s度 湿度:%s%s", temp.c_str(), humi.c_str(), "%");
    lv_label_set_text_fmt( ui_LabelCode, "天气:%s", text.c_str());
    lv_label_set_text_fmt( ui_LabelUpdateTime, "上次更新时间:%s", last_update.c_str());

    lv_label_set_text_fmt( ui_LabelAllWeather, "明天%s,天气%s,温度%s~%s度,降水概率%s", 
                            week_weather[(timeinfo.tm_wday+1<=6) ? (timeinfo.tm_wday+1) : (timeinfo.tm_wday+1-7)],
                            text_day.c_str(), low.c_str(), high.c_str(), precip.c_str());
    lv_roller_set_options( ui_RollerWeather, data.c_str(), LV_ROLLER_MODE_NORMAL );

    //Serial.printf("温度: %s,天气: %s,湿度：%s,城市：%s\r\n", temp, text, humi, city);
}

void get_time_weather(void) //lv_timer_t * timer
{
    if(time_flag)
    {
        if(getLocalTime(&timeinfo))
        {   
            const char* week[7]={"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
            lv_label_set_text_fmt(ui_LabelTime1,"%02d:%02d:%02d",timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec);
            lv_label_set_text_fmt(ui_LabelTime2,"%d/%02d/%02d %s",timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday,week[timeinfo.tm_wday]);
            if(weather_flag)
            {
                get_weather();
                lv_calendar_set_today_date(ui_Calendar,timeinfo.tm_year+1900,timeinfo.tm_mon+1,timeinfo.tm_mday);
                lv_calendar_set_showed_date(ui_Calendar,timeinfo.tm_year+1900,timeinfo.tm_mon+1);
                weather_flag = 0;   
            }
        }
    }
}