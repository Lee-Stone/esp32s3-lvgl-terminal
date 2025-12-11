#ifndef MIC2_HH
#define MIC2_HH

#include <all_data.h>

int getmic2data(char *data, int numData);
void wavHeader(byte* header, int wavSize);
void getbaiduToken(void);

#endif