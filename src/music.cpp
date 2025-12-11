#include <music.h>

#define I2S_DOUT      39
#define I2S_BCLK      40
#define I2S_LRC       41

// #define I2S_DOUT      41
// #define I2S_BCLK      40
// #define I2S_LRC       39

Audio audio;
String songlist;
const char * songoptions;

void initmusic(void) 
{
    audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
    audio.setVolume(16); // 0...21
    audio.setBalance(100);
    // audio.connecttoFS(SD, "红尘客栈.mp3");
    readmusic();
}

void readmusic(void)
{
    File root = SD.open("/");
    File file = root.openNextFile();
    while (file)
    {
        static int cnt = 0;
        if(cnt <= 1)cnt++;
        if (file.isDirectory())
        {
            // Serial.printf("DIR: %s\n", file.name());
        }
        else
        {
            const char *filename = file.name();

            int8_t len = strlen(filename);
            const char *MP3_EXT = ".mp3";
            if ((filename[0] != '.') && (strcmp(MP3_EXT, &filename[len - 4]) == 0))
            {
                if(cnt == 1)strcpy(songname, filename);
                songlist += filename; 
                songlist += "\n";
            }
        }
        file = root.openNextFile();
    }
    songoptions = songlist.c_str();
}

bool pause_flag = 0;
void getmusic(void)//lv_timer_t * timer
{
    audio.loop();
    audio.setVolume(voice); 
    if(play_flag == 1)
    {
        if(pause_flag && !audio.isRunning())
        {
            audio.pauseResume();
            pause_flag = 0;
        }
        if(start_flag)
        {
            if(baidu_audio_flag == 1)
            {
                audio.connecttohost(baidu_audio_host);
                // Serial.printf("%s\n", baidu_audio_host);
            }
            else if(game_yang_flag == 1)
            {
                audio.connecttoFS(SD, "普通Disco-羊了个羊.mp3");
            }
            else if(game_2048_flag == 1)
            {
                audio.connecttoFS(SD, "哈基米起床-哈基米.mp3");
            }
            else if(game_xiaole_flag == 1)
            {
                audio.connecttoFS(SD, "123我哈你-哈基米.mp3");
            }
            else if(game_pvz_flag == 1)
            {
                audio.connecttoFS(SD, "植物大战僵尸-背景乐.mp3");
            }
            else
            {
                audio.connecttoFS(SD, songname);
                // Serial.printf("%s\n", songname);
            }
            start_flag = 0;
        }
        if(!audio.isRunning() && start_flag == 0)
        {
            if(baidu_audio_flag == 1)
            {
                baidu_audio_flag = 0;
                play_flag = 0;
            }
            else start_flag = 1; 
        }
    }
    else 
    {
        if(!pause_flag && audio.isRunning())
        {
            audio.pauseResume();
            pause_flag = 1;
        }
    }
}

// // optional
// void audio_info(const char *info){
//     Serial.print("info        "); Serial.println(info);
// }
// void audio_id3data(const char *info){  //id3 metadata
//     Serial.print("id3data     ");Serial.println(info);
// }
// void audio_eof_mp3(const char *info){  //end of file
//     Serial.print("eof_mp3     ");Serial.println(info);
// }
// void audio_showstation(const char *info){
//     Serial.print("station     ");Serial.println(info);
// }
// void audio_showstreamtitle(const char *info){
//     Serial.print("streamtitle ");Serial.println(info);
// }
// void audio_bitrate(const char *info){
//     Serial.print("bitrate     ");Serial.println(info);
// }
// void audio_commercial(const char *info){  //duration in sec
//     Serial.print("commercial  ");Serial.println(info);
// }
// void audio_icyurl(const char *info){  //homepage
//     Serial.print("icyurl      ");Serial.println(info);
// }
// void audio_lasthost(const char *info){  //stream URL played
//     Serial.print("lasthost    ");Serial.println(info);
// }