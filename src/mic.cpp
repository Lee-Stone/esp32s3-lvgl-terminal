#include <mic.h>

#define ADC_OUT 1

bool mic_flag = 0;

String voice_text;
String voice_answer;
String voice_get;
String token = "24.7959df1e882c23ab78f9680a66f9b301.2592000.1715859312.282335-55916277";

const int recordTimeSeconds = 4; //定时器录音时间 4s
hw_timer_t * timer = NULL;
void IRAM_ATTR onTimer(); //中断

const int adc_data_len = 16000 * recordTimeSeconds;
uint16_t *adc_data;
const int data_json_len = adc_data_len * 2 * 1.4;
char *data_json;
String data_json2;

uint32_t num =  0;
bool adc_start_flag = 0;        //开始标志
bool adc_read_flag = 0;
bool adc_complete_flag =  0;     //完成标志

// 百度STT API设置
const char* API_KEY = "OKRxtGRoqjFciDCv65G2n6eg";
const char* SECRET_KEY = "2JhkKysMqhTvxD40TPMBrbtOE1VKHYSQ";
const char* ID = "55916277";

// MINIMAX
String GroupKey = (String)"Bearer " + "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJHcm91cE5hbWUiOiLmnY7mnY_no4oiLCJVc2VyTmFtZSI6Iuadjuadj-ejiiIsIkFjY291bnQiOiIiLCJTdWJqZWN0SUQiOiIxNzY4NTM4MDU2MDAxODYzNzMzIiwiUGhvbmUiOiIxOTU3MjI5Mjg1MyIsIkdyb3VwSUQiOiIxNzY4NTM4MDU1OTkzNDc1MTI1IiwiUGFnZU5hbWUiOiIiLCJNYWlsIjoiIiwiQ3JlYXRlVGltZSI6IjIwMjQtMDMtMTggMTA6MzU6MzMiLCJpc3MiOiJtaW5pbWF4In0.QWhsm1BqZAkR6k8OcertP2qx64LI8zzi3sbasfkSQfgTnIYg8wuzJQ5EeMpfnmgo7Dwj46oGHnaYHJ3QKVeCg7IASstBxJ4Zpwuu6MK-zj2sYBBQDh71mJZd67DbHkGeCl2VxDZsx4xLQNSH4QXpfZYs6NvJSPNgSgEuPRPqWI5i-7HY20inzraTZ_QLZY4fnwbIOZ1BdWzTOmKBXHfk2MVpzD3JV64qq1rUNgA_hFc14z6VVYpfmTIREr6zTAZHM2sT_xMR_gAoFlzlkLXf5QqMIG9Bg17V0I_jFdGsigxNoV9vDC3enE464X8Ddg78YNvMQe25VCyLilRkLC-nHA";
String GroupID = "1768538055993475125";

void initmic(void) 
{
    pinMode(ADC_OUT, ANALOG);

    adc_data = (uint16_t *)ps_malloc(adc_data_len * sizeof(uint16_t)); //使用片外PSRAM
    if (!adc_data)Serial.println("Failed to allocate memory for adc_data");

    data_json = (char *)ps_malloc(data_json_len * sizeof(char)); 
    if (!data_json)Serial.println("Failed to allocate memory for data_json");
    
    timer = timerBegin(1, 40, true);    //  80M的时钟 40分频 1M
    timerAlarmWrite(timer, 125, true);  //  1M  计125个数进中断  16K
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmEnable(timer);
    timerStop(timer);   //先暂停
}

void getmic(void) 
{
    if(mic_flag)
    {
        Serial.println("Start");
        audio.stopSong();
        vTaskDelay(100);

        adc_start_flag = 1;
        timerStart(timer);

        while(!adc_complete_flag)
        {  
            vTaskDelay(100);
        }

        if(adc_complete_flag)
        {
            timerStop(timer);

            lv_textarea_set_text(ui_TextAreaQuestion,"请等待...");
            Serial.println("Over");

            //获取ChatGPT答案文本
            aseembleJson(token);
            sendToSTT();

            lv_textarea_set_text(ui_TextAreaQuestion, voice_text.c_str());
            lv_textarea_set_text(ui_TextAreaAnswer, voice_answer.c_str());
            lv_obj_clear_state(ui_SwitchSpeak, LV_STATE_CHECKED | LV_STATE_DISABLED);
            lv_label_set_text(ui_LabelGPTSwitch, "OFF");

            adc_complete_flag = 0;     
            mic_flag = 0;
        }
    }
}

void sendToSTT(void)   //上传到百度云
{
    HTTPClient http_text;
    http_text.begin("http://vop.baidu.com/server_api");
    http_text.addHeader("Content-Type", "application/json");

    int httpCode = http_text.POST(data_json);

    if (httpCode > 0) 
    {
        if (httpCode == HTTP_CODE_OK) 
        {
            String payload = http_text.getString();
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            voice_text = doc["result"].as<String>();
            voice_text.trim(); // 去除首尾空白字符
            voice_text.replace("[", ""); // 去除左方括号
            voice_text.replace("]", ""); // 去除右方括号
            voice_text.replace("\"", ""); // 去除双引号

            // Serial.println(voice_text);

            voice_answer = get_GPTanswer(voice_text);
            // Serial.println(voice_answer);
        }
    } 
    else 
    {
        Serial.printf("[HTTP] POST failed, error: %s\n", http_text.errorToString(httpCode).c_str());
    }
    http_text.end();
}

void getToSTT(void)
{
    voice_answer = "你好，我是你的机器人助手。";

    data_json2 = "{\"voice_id\": \"male-qn-qingse\",\"text\":\""+voice_answer+"\",\"model\": \"speech-01\",\"speed\": 1.0,\"vol\": 1.0,\"pitch\": 0,\"audio_sample_rate\": 24000,\"bitrate\": 128000}";

    HTTPClient http_voice;
    // http_voice.begin("https://tsn.baidu.com/text2audio?tex="+encode+"&lan=zh&cuid="+ID+"&ctp=1&tok="+token);
    http_voice.begin("https://api.minimax.chat/v1/t2a_pro?GroupId={"+GroupID+"}");
    http_voice.addHeader("Authorization", GroupKey);
    http_voice.addHeader("Content-Type", "application/json");

    int httpCode = http_voice.POST(data_json2);

    if (httpCode > 0) 
    {
        if (httpCode == HTTP_CODE_OK) 
        {
            String payload = http_voice.getString();
            DynamicJsonDocument doc(4096);
            DeserializationError error = deserializeJson(doc, payload);
            Serial.println(payload);

            if(error) 
            {
                Serial.print("deserializeJson() failed: ");
                Serial.println(error.c_str());
            }

            voice_get = doc["audio_file"].as<String>();
            Serial.println(voice_get);
            // audio.connecttohost(voice_get.c_str());
        }
    } 
    else 
    {
        Serial.printf("[HTTP] POST failed, error: %s\n", http_voice.errorToString(httpCode).c_str());
    }
    http_voice.end();
}

String gainToken(void)   //获取token
{
    HTTPClient http_token;
    String token;

    String url = String("https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&client_id=") + API_KEY + "&client_secret=" + SECRET_KEY;

    http_token.begin(url);
    int httpCode = http_token.GET();

    if(httpCode > 0) 
    {
        if(httpCode == HTTP_CODE_OK) 
        {
            String payload = http_token.getString();
            DynamicJsonDocument doc(1024);
            deserializeJson(doc, payload);
            token = doc["access_token"].as<String>();
        }
    }
    else 
    {
        Serial.printf("[HTTP] GET... failed, error: %s\n", http_token.errorToString(httpCode).c_str());
    }
    http_token.end();

    return token;
}

void aseembleJson(String token)
{
    memset(data_json, '\0',strlen(data_json));   //将数组清空
    strcat(data_json, "{");
    strcat(data_json, "\"format\":\"pcm\",");
    strcat(data_json, "\"rate\":16000,");        //采样率    如果采样率改变了，记得修改该值，只有16000、8000两个固定采样率
    strcat(data_json, "\"dev_pid\":1537,");      //中文普通话
    strcat(data_json, "\"channel\":1,");         //单声道
    strcat(data_json, "\"cuid\":\"04190906\","); //识别码    随便打几个字符，但最好唯一
    strcat(data_json, "\"token\":\"");
    strcat(data_json, token.c_str());            //这里需要修改成自己申请到的token
    strcat(data_json, "\",");          
    sprintf(data_json + strlen(data_json), "\"len\":%d,", adc_data_len * 2);         //数据长度 如果传输的数据长度改变了,记得修改该值,该值是ADC采集的数据字节数,不是base64编码后的长度
    strcat(data_json, "\"speech\":\"");
    strcat(data_json, base64::encode((uint8_t *)adc_data, adc_data_len * sizeof(uint16_t)).c_str());     //base64编码数据
    strcat(data_json, "\"");
    strcat(data_json, "}");
}

void removeBrackets(String str) 
{
    int len = strlen(str.c_str()) + 2;
    int i, j = 0;

    // 遍历字符串，将非方括号的字符复制到原字符串中
    for (i = 0; i <= len; i++)
    {
        if (str[i] != '"' && str[i] != '[' && str[i] != ']') {
            str[j] = str[i];
            j++;
        }
    }

    // 添加字符串结束符
    str[j] = '\0';
}

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
void IRAM_ATTR onTimer()
{
    portENTER_CRITICAL_ISR(&timerMux);

    if(adc_start_flag == 1)
    {
        // if(!adc_read_flag)adc_read_flag = 1;
        adc_data[num] = analogRead(ADC_OUT);
        num++;
        if(num >= adc_data_len)
        {
            adc_complete_flag = 1;
            adc_start_flag = 0;
            num = 0;
        }
    }

    portEXIT_CRITICAL_ISR(&timerMux);
}