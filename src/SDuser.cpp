#include "sduser.h"
#include <SPI.h>
#include <SD.h>

#define SD_CS         9
#define SD_SCK        14
#define SD_MISO       17
#define SD_MOSI       10

SPIClass SD_SPI(HSPI);

void SD_init() 
{
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    SD_SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    SD_SPI.setFrequency(4000000); // last 1000000
    if(!SD.begin(SD_CS, SD_SPI))Serial.println("Card Mount Failed");
    else Serial.println("Card Mount Success");
}