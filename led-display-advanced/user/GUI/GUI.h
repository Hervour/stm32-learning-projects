#ifndef __GUI_H__
#define __GUI_H__
//////////////////////////////////////////////////////////////////////////////////////
#include  "Lcd_Drive.h"
#include  "w25q64.h"

//////////////////////////////////////////////////////////////////////////////////////
typedef  enum{
    DOT16X16aWORD = 1,
    DOT24X24aWORD    ,
    DOT32X16aCHAR    ,
}uniWORDDOTSTYPE;

typedef  enum{
    DIG_UINT8_T  = 8,
    DIG_UINT16_T = 16,
    DIG_UINT32_T = 32,
    DIG_UINT64_T = 64,
}uniDIGITTYPE;

#define PICTURE_H  224
#define PICTURE_W  185


//////////////////////////////////////////////////////////////////
void  Gui_Draw_aPixel(uint16_t ,uint16_t ,uint16_t );
void  Gui_Draw_Line(uint16_t , uint16_t , uint16_t , uint16_t , uint16_t );
void  Gui_Circle(uint16_t ,uint16_t ,uint16_t ,uint16_t ); 
void  Gui_Circle_Fill(uint16_t ,uint16_t ,uint16_t ,uint16_t );
void  Gui_Draw_Rect(uint16_t , uint16_t , uint16_t , uint16_t , uint16_t );
void  Gui_Draw_Asc16_Hzk16(uint16_t , uint16_t , uint16_t , uint16_t , uint8_t *);
void  Gui_Draw_Asc24_Hzk24(uint16_t , uint16_t , uint16_t , uint16_t , uint8_t *);
void  Gui_Draw_A_Asc16(uint16_t , uint16_t , uint16_t , uint16_t , uint8_t );
void  Gui_Draw_Asc32_String(uint16_t , uint16_t , uint16_t , uint16_t , uint8_t *);
void  Gui_Draw_A_Word_32x32(uint16_t , uint16_t , uint16_t , uint16_t ,const uint8_t *);
void  LCD_Draw_Picture(uint16_t xstr,uint16_t ystr,const uint8_t *pic);

void  Gui_Draw_Dot20X40(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, const uint8_t *word);
void  Gui_Draw_Dot40X40(uint16_t x, uint16_t y, uint16_t forcolor, uint16_t bkcolor, const uint8_t *word);

//////////////////////////////////////////////////////////////////////////////////////

#endif

