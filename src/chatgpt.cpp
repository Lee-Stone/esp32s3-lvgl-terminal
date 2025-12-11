#include <chatgpt.h>

String url_3 = "https://aip.baidubce.com/rpc/2.0/ai_custom/v1/wenxinworkshop/chat/completions?access_token=24.d48bb2dbb7cc5c50af9030da72387457.2592000.1712976820.282335-56444788";
String url_4 = "https://api.minimax.chat/v1/text/chatcompletion_v2";
String MiniMaxKey = (String)"Bearer " + "eyJhbGciOiJSUzI1NiIsInR5cCI6IkpXVCJ9.eyJHcm91cE5hbWUiOiLmnY7mnY_no4oiLCJVc2VyTmFtZSI6Iuadjuadj-ejiiIsIkFjY291bnQiOiIiLCJTdWJqZWN0SUQiOiIxNzY4NTM4MDU2MDAxODYzNzMzIiwiUGhvbmUiOiIxOTU3MjI5Mjg1MyIsIkdyb3VwSUQiOiIxNzY4NTM4MDU1OTkzNDc1MTI1IiwiUGFnZU5hbWUiOiIiLCJNYWlsIjoiIiwiQ3JlYXRlVGltZSI6IjIwMjQtMDMtMTggMTA6MzU6MzMiLCJpc3MiOiJtaW5pbWF4In0.QWhsm1BqZAkR6k8OcertP2qx64LI8zzi3sbasfkSQfgTnIYg8wuzJQ5EeMpfnmgo7Dwj46oGHnaYHJ3QKVeCg7IASstBxJ4Zpwuu6MK-zj2sYBBQDh71mJZd67DbHkGeCl2VxDZsx4xLQNSH4QXpfZYs6NvJSPNgSgEuPRPqWI5i-7HY20inzraTZ_QLZY4fnwbIOZ1BdWzTOmKBXHfk2MVpzD3JV64qq1rUNgA_hFc14z6VVYpfmTIREr6zTAZHM2sT_xMR_gAoFlzlkLXf5QqMIG9Bg17V0I_jFdGsigxNoV9vDC3enE464X8Ddg78YNvMQe25VCyLilRkLC-nHA";

String get_GPTanswer(String question)
{
    HTTPClient http_3;
    http_3.setTimeout(20000);
    http_3.begin(url_4);
    http_3.addHeader("Content-Type", "application/json");
    http_3.addHeader("Authorization", MiniMaxKey);

    String data = //"{\"messages\":[{\"role\": \"user\",\"content\": \"" + question + "\"}],\"disable_search\": false,\"enable_citation\": false}";
                 "{\"model\":\"abab5.5s-chat\",\"messages\":[{\"role\": \"system\",\"content\": \"你是一个聊天机器人，要求下面的回答严格控制在256字符以内。\"},{\"role\": \"user\",\"content\": \"" + question + "\"}]}";
    int httpResponseCode = http_3.POST(data);

    if (httpResponseCode == 200)
    {
        String response = http_3.getString();
        DynamicJsonDocument doc(4096);
        DeserializationError error = deserializeJson(doc, response);

        if(error) 
        {
            Serial.print("deserializeJson() failed: ");
            Serial.println(error.c_str());
            return "<error>";
        }

        String answer = //doc["result"].as<String>();
                        doc["choices"][0]["message"]["content"].as<String>();
        http_3.end();
        return answer;
    } 
    else
    {
        Serial.print("HTTP POST request failed, error code: ");
        Serial.println(httpResponseCode);
        http_3.end();
        return "<error>";
    }
}

void GPTtest(void)
{
    if (Serial.available()) 
    {
        String question = Serial.readStringUntil('\n');
        question.trim();
        Serial.println("Question: " + question);

        String answer = get_GPTanswer(question);
        Serial.println("Answer: " + answer);

        Serial.println("Enter question:");
    }
}