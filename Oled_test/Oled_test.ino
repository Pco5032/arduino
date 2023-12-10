// Oled display library
#include <U8g2lib.h>
#include <Wire.h>

#define time_delay 2000
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void u8g2_prepare() {
 u8g2.setFont(u8g2_font_logisoso26_tf);
  
 u8g2.setFontRefHeightExtendedText();
 u8g2.setDrawColor(1);
 u8g2.setFontPosTop();
 u8g2.setFontDirection(0);
}

void setup(void) {
 u8g2.begin();
 u8g2_prepare();
}

void loop(void) {
 u8g2.clearBuffer();

 u8g2.setCursor(0, 0);
 u8g2.print("23:59:59");
 
 u8g2.setCursor(0, 35);
 u8g2.print(-15);
 u8g2.print("°C");
 u8g2.sendBuffer();
 delay(2000);
}