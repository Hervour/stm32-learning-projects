#include  "common.h"


// 此延迟无法精确定时
void Delay(volatile uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}


// 交换一个字节中的高低位
uint8_t  Swap_bit7_bit0(uint8_t inbyte)
{
    uniBit8Byte  in, ret;
    in.abyte = inbyte;
    ret.bits8.b0 = in.bits8.b7;
    ret.bits8.b1 = in.bits8.b6;
    ret.bits8.b2 = in.bits8.b5;
    ret.bits8.b3 = in.bits8.b4;
    ret.bits8.b4 = in.bits8.b3;
    ret.bits8.b5 = in.bits8.b2;
    ret.bits8.b6 = in.bits8.b1;
    ret.bits8.b7 = in.bits8.b0;
    return  ret.abyte;
}

// 获得字符串是否匹配
ErrorStatus String_Match_OK(char *str1, char *str2)
{
    char        *strmatch;
    ErrorStatus  retval = ERROR;
    
    strmatch = strstr(str1, str2);
    if(strmatch != NULL)  // 不为空，说明字符匹配成功
    {
        retval = SUCCESS;
    }
    return  retval;
}

// 获得字符串匹配时的第一个字符的位置信息
// *first_addr 为 匹配时，*str2的  第一个字符在*str1的位置,   从头开始偏移多少个字节，从0开始
// *end_addr   为 匹配时，*str2的最后一个字符在*str1的位置+1，从头开始偏移多少个字节，从1开始
ErrorStatus Get_String_Match_First_End_Addr(char *str1, char *str2, uint32_t *first_addr, uint32_t *end_addr)
{
    uint32_t     match_addr;
    char        *strmatch;
    ErrorStatus  retval = ERROR;
    
    strmatch = strstr(str1, str2);
    if(strmatch != NULL)  // 不为空，说明字符匹配成功
    {
        retval = SUCCESS;
        match_addr = strmatch - str1;
        *first_addr = match_addr;
        *end_addr   = match_addr + strlen(str2);
    }
    return  retval;
}




