#include <U8g2lib.h>
#include <SPI.h>

#define SCREEN U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI
#define SPI_CLK 14

#define SHOW 0 // 0 -> display channels 0-12; 1 -> display channels 13-15 + failsafe bit

// SCREEN object
SCREEN oled(U8G2_R2, 10, 15,16);

uint16_t channels[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

void setup()                    
{
   SPI.setSCK(SPI_CLK);
   SPI.begin();   
   oled.begin();
   //oled.setFont(u8g2_font_6x10_mf);
   oled.setFont(u8g2_font_profont11_mf);

}

void loop()                    
{
    updateDisplay(); 
    delay(100);
}


void updateDisplay()
{ 
  oled.clearBuffer();
  
  if (!SHOW) // show channesl 0-12
      {
      int channel = 0;
      for (int i = 10; i < 63; i += 9)
          {
          oled.setCursor(0, i);
          oled.print("CH" + String(channel) + ": " + rightJustify(channels[channel], channel)); 
          oled.print(" CH" + String(channel+7) + ": " + rightJustify(channels[channel+7], channel+7));
          channel++;
          }
      }
   else // show channels 13-15 + failsafe
      {
      int channel = 13;
      for (int i = 10; i < 29; i += 9)
          {
          oled.setCursor(0, i);
          oled.print("CH" + String(channel) + ": " + rightJustify(channels[channel], channel));
          channel++;
          }
      oled.setCursor(0, 37);
      oled.print("Failsafe bit: " + String(0));
      }
   
  oled.sendBuffer();
}

String rightJustify(int num, int channel)
{
    String numString = String(num);
    int numLength = numString.length();

    if (channel < 10)
        {
        while (numLength < 5)
            {
            numString = " " + numString;
            numLength++;
            }
        }
     else
        {
        while (numLength < 4)
            {
            numString = " " + numString;
            numLength++;
            }        
        }
        
    return numString;
}
