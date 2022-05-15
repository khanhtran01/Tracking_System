#include "set_time_onl.h"
#include "..\uart\uart.h"

unsigned char str_cmp(char str1[], char str2[], char n)
{
    char i = 0;
    for(i = 0; i < n; i++)
    {
        if(str1[i] != str2[i])
            return 1 ;
    }
    return 0;
}
