#include "mic2.h"

uint8_t mic2_flag = 0;
bool baidu_audio_flag = 0;
const char *baidu_audio_host = " ";

#define I2S_WS      13
#define I2S_SD      12
#define I2S_SCK     11
#define I2S_PORT    I2S_NUM_0
#define I2S_SAMPLE_RATE   (16000)
#define I2S_SAMPLE_BITS   (16)

const int i2sBufferSize = 5120;
char i2sBuffer[i2sBufferSize];  
const int header_size = 44;     

// websockets 语音识别
using namespace websockets;
WebsocketsClient webSocket_mic2;
void mic2_onMessageCallback(WebsocketsMessage message);
void mic2_onEventsCallback(WebsocketsEvent event, String data);
uint8_t mic2_wbsocketEvent = 0;

// 百度APi用户信息
String baidu_app_id = "118253385";
String baidu_app_key = "jLjoDnVyQ5BQvOFi4D8VkXhM";
String baidu_secret_key = "SJaYImJ4hKQApCgaLnh2WBs8SRdm09Fq";
String baidu_access_token = " ";

// 百度语音识别
String baidu_input_url = "ws://vop.baidu.com/realtime_asr?sn=L1-E2-E3-";
String baidu_input_start_json = "{\"type\": \"START\",\"data\": {\"appid\":"+baidu_app_id+",\"appkey\":\""+baidu_app_key+"\",\"dev_pid\": 1537,\"cuid\": \"cuid-1\",\"format\": \"pcm\", \"sample\": 16000}}";
String baidu_input_end_json = "{\"type\": \"FINISH\"}";
String baidu_input_cancel_json = "{\"type\": \"CANCEL\"}";

// 百度短语音合成
HTTPClient http_audio;
String baidu_output_url = " ";
const int baidu_output_size = 70;

String mic2_text;
String mic2_answer;

void initmic2(void) 
{
    i2s_config_t i2s_config = 
    {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
        .sample_rate = I2S_SAMPLE_RATE,
        .bits_per_sample = i2s_bits_per_sample_t(I2S_SAMPLE_BITS),
        .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
        .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
        .intr_alloc_flags = 0,
        .dma_buf_count = 16,    
        .dma_buf_len = 60,      
        // .use_apll = 1           // use APLL-CLK,frequency 16MHZ-128MHZ,it's for audio
    };

    i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);

    const i2s_pin_config_t pin_config =
    {
        .bck_io_num = I2S_SCK,
        .ws_io_num = I2S_WS,
        .data_out_num = I2S_PIN_NO_CHANGE,
        .data_in_num = I2S_SD
    };

    esp_err_t ret = i2s_set_pin(I2S_PORT, &pin_config);

    webSocket_mic2.onMessage(mic2_onMessageCallback);
    webSocket_mic2.onEvent(mic2_onEventsCallback);
}

void getmic2(void) 
{
    webSocket_mic2.poll();
    if(mic2_flag == 1)
    {
        webSocket_mic2.connect(baidu_input_url);
        if(webSocket_mic2.send(baidu_input_start_json) == 0)mic2_wbsocketEvent = 3;
        getbaiduToken();

        mic2_answer = urlEncode(urlEncode("我是你的语音助手,快来和我聊天吧!"));
        baidu_output_url = "http://tsn.baidu.com/text2audio?tex=" 
                            + mic2_answer 
                            + "&lan=zh&cuid=cuid-1&ctp=1&per=5977&tok=" 
                            + baidu_access_token;
        baidu_audio_host = (const char*)baidu_output_url.c_str();
        baidu_audio_flag = 1;play_flag = 1;start_flag = 1;

        lv_textarea_set_text(ui_TextAreaQuestion," ");
        lv_textarea_set_text(ui_TextAreaAnswer, "快来和我聊天吧...");

        Serial.println("Start getmic2");

        mic2_flag = 0;
    }
    else if(mic2_flag == 2)
    {
        webSocket_mic2.send(baidu_input_end_json);

        Serial.println("End getmic2");

        mic2_flag = 0;
    }
    else if(mic2_flag == 3)
    {
        mic2_answer = get_GPTanswer(mic2_text);

        int16_t answer_index = 0;
        while(1)
        {   
            getmic2data(i2sBuffer, i2sBufferSize);
            webSocket_mic2.sendBinary(i2sBuffer, i2sBufferSize);

            if(baidu_audio_flag == 0)
            {
                if(answer_index >= mic2_answer.length())break; 
                else 
                {
                    String temp = mic2_answer.substring(answer_index, answer_index + baidu_output_size);
                    temp = urlEncode(urlEncode(temp));
                    baidu_output_url = "http://tsn.baidu.com/text2audio?tex=" 
                                        + temp 
                                        + "&lan=zh&cuid=cuid-1&ctp=1&per=5977&tok=" 
                                        + baidu_access_token;
                    baidu_audio_host = (const char*)baidu_output_url.c_str();
                    baidu_audio_flag = 1;play_flag = 1;start_flag = 1; 
                }
                answer_index += (baidu_output_size -1);   
                for(int i = 0; i < 20; i++)lv_textarea_set_text(ui_TextAreaAnswer, mic2_answer.c_str());  
            }

            vTaskDelay(160);
        }

        mic2_flag = 0;
    }

    if(mic2_wbsocketEvent == 1)
    {
        getmic2data(i2sBuffer, i2sBufferSize);
        webSocket_mic2.sendBinary(i2sBuffer, i2sBufferSize);
    }
    else if(mic2_wbsocketEvent == 2)
    {
        mic2_answer = urlEncode(urlEncode("那下次再见咯"));
        baidu_output_url = "http://tsn.baidu.com/text2audio?tex=" 
                            + mic2_answer 
                            + "&lan=zh&cuid=cuid-1&ctp=1&per=5977&tok=" 
                            + baidu_access_token;
        baidu_audio_host = (const char*)baidu_output_url.c_str();
        baidu_audio_flag = 1;play_flag = 1;start_flag = 1;

        lv_textarea_set_text(ui_TextAreaQuestion," ");
        lv_textarea_set_text(ui_TextAreaAnswer, "那下次再见咯...");
        lv_obj_clear_state(ui_SwitchSpeak, LV_STATE_CHECKED);
        lv_label_set_text(ui_LabelGPTSwitch, "OFF");

        mic2_wbsocketEvent = 0;
    }
    else if(mic2_wbsocketEvent == 3)
    {
        lv_textarea_set_text(ui_TextAreaQuestion, "网络似乎不太好呢...");
        lv_textarea_set_text(ui_TextAreaAnswer, "检查一下网络吧...");
        lv_obj_clear_state(ui_SwitchSpeak, LV_STATE_CHECKED);
        lv_label_set_text(ui_LabelGPTSwitch, "OFF");

        mic2_wbsocketEvent = 0;
    }
}

// 百度语音识别
void mic2_onMessageCallback(WebsocketsMessage message)
{
    String data = message.data();

    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, data);

    if (!error) 
    {
        if(doc["err_no"] == 0 && (doc["type"] == "FIN_TEXT") || (doc["type"] == "MID_TEXT"))
        {
            mic2_text = doc["result"].as<String>();
            // Serial.printf("%s\r\n", mic2_text.c_str());

            lv_textarea_set_text(ui_TextAreaQuestion, mic2_text.c_str());

            if(doc["type"] == "FIN_TEXT")mic2_flag = 3;
        }
    }
    else
    {
        Serial.print("JSON解析错误: ");
        Serial.println(error.c_str());
    }
}

void mic2_onEventsCallback(WebsocketsEvent event, String data)
{
    if(event == WebsocketsEvent::ConnectionOpened) 
    {
        Serial.println("Connnection Opened");
        mic2_wbsocketEvent = 1;
    } 
    else if(event == WebsocketsEvent::ConnectionClosed) 
    {
        Serial.println("Connnection Closed");
        mic2_wbsocketEvent = 2;
    } 
}

void getbaiduToken(void)
{
    String url = "https://aip.baidubce.com/oauth/2.0/token?";
    HTTPClient http;   
    http.begin(url+"client_id="+baidu_app_key+"&client_secret="+baidu_secret_key+"&grant_type=client_credentials"); 
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Accept", "application/json");

    int httpCode = http.POST("");
    if(httpCode > 0) 
    {
        String payload = http.getString();
        DynamicJsonDocument doc(1024);
        deserializeJson(doc, payload);
        baidu_access_token = doc["access_token"].as<String>();
    }
    else 
    {
        Serial.printf("Error on HTTP request: %s\n", http.errorToString(httpCode).c_str());
    }
}

int getmic2data(char *data, int numData)
{
    size_t bytesRead;
    i2s_read(I2S_PORT, (char *)data, numData, &bytesRead, portMAX_DELAY);
    return bytesRead;
}

// 生成wav header，16bit 位深
void wavHeader(byte* header, int wavSize) 
{  
    header[0] = 'R';
    header[1] = 'I';
    header[2] = 'F';
    header[3] = 'F';
    unsigned int fileSize = wavSize + header_size - 8;
    header[4] = (byte)(fileSize & 0xFF); // file size, 4byte integer
    header[5] = (byte)((fileSize >> 8) & 0xFF);
    header[6] = (byte)((fileSize >> 16) & 0xFF);
    header[7] = (byte)((fileSize >> 24) & 0xFF);
    header[8] = 'W';
    header[9] = 'A';
    header[10] = 'V';
    header[11] = 'E';
    header[12] = 'f';
    header[13] = 'm';
    header[14] = 't';
    header[15] = ' ';
    header[16] = 0x10; // length of format data = 16, 4byte integer
    header[17] = 0x00;
    header[18] = 0x00;
    header[19] = 0x00;
    header[20] = 0x01; // format type:1(PCM), 2byte integer
    header[21] = 0x00;
    header[22] = 0x01; // channel number:1, 2byte integer
    header[23] = 0x00;
    header[24] = 0x80; // sample rate:16000=0x00003E80, 4byte integer
    header[25] = 0x3E;
    header[26] = 0x00;
    header[27] = 0x00;
    header[28] = 0x00; // SampleRate*BitPerSample*ChannelNum/8=16000*16*1/8=0x00007D00, 4byte integer
    header[29] = 0x7D;
    header[30] = 0x00;
    header[31] = 0x00;
    header[32] = 0x02; // BitPerSample*ChannelNum/8 = 2, 2byte integer
    header[33] = 0x00;
    header[34] = 0x10; // BitPerSample:16 = 0x0010, 2byte integer
    header[35] = 0x00;
    header[36] = 'd';
    header[37] = 'a';
    header[38] = 't';
    header[39] = 'a';
    header[40] = (byte)(wavSize & 0xFF);
    header[41] = (byte)((wavSize >> 8) & 0xFF);
    header[42] = (byte)((wavSize >> 16) & 0xFF);
    header[43] = (byte)((wavSize >> 24) & 0xFF);
}

// // 讯飞语音识别
// String xunfei_url = "ws://iat-api.xfyun.cn/v2/iat";
// String xunfei_app_id = "5f7a7d9a";
// String xunfei_api_key = "4e15f5f4fbf4487b8d3e92a02c85e095";
// String xunfei_api_secret = "MTc1ZTQ5Njc2ZDZiY2QwYjVjYTAzMGZi";

// String creatXunfeiUrl(void)
// {
//     String url = xunfei_url;
//     String APIKey = xunfei_api_key;
//     String APISecret = xunfei_api_secret;
//     String host = "iat-api.xfyun.cn";
//     String path = "/v2/iat ";

//     // 获取RFC1123时间
//     char date_trans[30] = {0};
//     strftime(date_trans, sizeof(date_trans), "%a, %d %b %Y %H:%M:%S GMT", &timeinfo);
//     String date = date_trans;

//     // 构建签名字符串
//     String signature_origin = "host: " + host + "\n";
//     signature_origin += "date: " + date + "\n";
//     signature_origin += "GET " + path + " HTTP/1.1";

//     // 计算HMAC-SHA256
//     byte hmacResult[32];
//     mbedtls_md_hmac(
//         mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
//         (const unsigned char*)APISecret.c_str(),
//         APISecret.length(),
//         (const unsigned char*)signature_origin.c_str(),
//         signature_origin.length(),
//         hmacResult
//     );

//     // Base64编码签名
//     String signature_sha = base64::encode(hmacResult, 32);

//     // 构建Authorization
//     String authorization_origin = "api_key=\"" + APIKey + "\", algorithm=\"hmac-sha256\", headers=\"host date request-line\", signature=\"" + signature_sha + "\"";
//     String authorization = base64::encode(authorization_origin);

//     // 构造查询参数
//     String query = "authorization=" + urlEncode(authorization);
//     query += "&date=" + urlEncode(date);
//     query += "&host=" + urlEncode(host);

//     url = url + '?' + query;

//     return url;
// }