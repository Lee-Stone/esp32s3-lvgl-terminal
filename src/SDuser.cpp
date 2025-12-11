#include <SDuser.h>

// #define SD_CS         12
// #define SD_SCK        15
// #define SD_MISO       17
// #define SD_MOSI       7

// last
// #define SD_CS         9
// #define SD_SCK        15
// #define SD_MISO       17
// #define SD_MOSI       7

#define SD_CS         9
#define SD_SCK        14
#define SD_MISO       17
#define SD_MOSI       10

SPIClass SD_SPI(HSPI);

void initSD(void) 
{
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    SD_SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    SD_SPI.setFrequency(4000000); // last 1000000
    if(!SD.begin(SD_CS, SD_SPI))Serial.println("Card Mount Failed");
    else Serial.println("Card Mount Success");
    
    uint8_t cardType = SD.cardType();
    if(cardType == CARD_NONE)Serial.println("No SD card attached");
    else
    {
        Serial.print("SD Card Type: ");
        if(cardType == CARD_MMC)Serial.println("MMC");
        else if(cardType == CARD_SD)Serial.println("SDSC");
        else if(cardType == CARD_SDHC)Serial.println("SDHC");
        else Serial.println("UNKNOWN");
    }

    // lv_fs_fatfs_init();
}

// void lv_fs_test(void)
// {
// 	lv_fs_file_t f;
// 	lv_fs_res_t res;
// 	res = lv_fs_open(&f, "S:test/file.txt", LV_FS_MODE_RD);
// 	if(res != LV_FS_RES_OK) 
// 	{
// 		Serial.println("Open fail");
// 	}
// 	else
// 	{
// 		Serial.println("Open OK");
// 		uint32_t read_num;
// 		uint8_t buf[8];
// 		res = lv_fs_read(&f, buf, 8, &read_num);
// 		Serial.printf("read:%s",buf);
// 		lv_fs_close(&f);
// 	}
// }