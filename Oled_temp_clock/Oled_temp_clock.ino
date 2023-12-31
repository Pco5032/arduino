// Oled display library
#include <U8g2lib.h>
#include <Wire.h>
// Date and time functions using a DS3231 RTC connected via I2C
#include "RTClib.h"
// include the OneWire library used by DS18B20 temperature sensor
#include "OneWire.h"
#include <max6675.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

int dispSwitch = 0;

// x/y for line 1 and 2 on OLED
int x1=3, y1=0;
int x2=38, y2=0;

// display var
char buffer[20];

// RTC vars
#define DS3231_ADDRESS  0x68
bool RTC_isThere = false;
unsigned long RTC_lastRefresh = 0;

// EGT vars
int EGT1_DO = 7; 
int EGT1_CS = 8; 
int EGT1_SCK = 9;
int EGT2_DO = 10; 
int EGT2_CS = 11; 
int EGT2_SCK =12;

// TEMP vars : set DS18B20 temperature sensor pin number
const int DS18B20Pin = 5;
#define DS_ADDRESS  0x28
byte DS_addr[8];
byte DS_status;
const int DS_betweenTempMeasure = 1000; // Duree entre les mesures de température
unsigned long DS_betweenTempMillis = 0;
float outsideTemperature = 0.0; // °C

/* Return from function getTemperature() used by DS18B20 */
enum DS18B20_RCODES {
  READ_OK,  // Lecture ok
  NO_SENSOR_FOUND,  // Pas de capteur
  INVALID_ADDRESS,  // Adresse reçue invalide
  INVALID_SENSOR,  // Capteur invalide (pas un DS18B20)
  SENSOR_FOUND,
  READ_WAIT
};

// declare rtc object
RTC_DS3231 rtc;

// declare DS18B20 temp sensor
OneWire ds(DS18B20Pin);

// declare EGT
MAX6675 EGT1(EGT1_SCK, EGT1_CS, EGT1_DO);
MAX6675 EGT2(EGT2_SCK, EGT2_CS, EGT2_DO);

void u8g2_prepare() {
 u8g2.setFont(u8g2_font_logisoso26_tf);
 u8g2.enableUTF8Print();
 u8g2.setFontRefHeightExtendedText();
 u8g2.setDrawColor(1);
 u8g2.setFontPosTop();
 u8g2.setFontDirection(0);
}

void setup(void) {
 // initialize OLED
 u8g2.begin();
 u8g2_prepare();

 // initialize RTC
 initRTC();

 // search DS temperature sensor, store status in DSstatus and address in DSaddr
 DS_status = getDSadress();
}

void loop(void) {
 u8g2.clearBuffer();

  // display EGTs or clock and ext.temp
  if (dispSwitch == 0) {
   displayEGT();
   dispSwitch = 1;
   }
   else {
    // display RTC
    displayRTC();
    // Temp : request measure, read and display temp from sensor
    if (DS_status == SENSOR_FOUND) {
      dispSwitch = 0;
      requestTemperatureMeasure();
      getTemperature(&outsideTemperature);
      displayTemp(outsideTemperature);
      }
      else {
       u8g2.setCursor(0, 35);
       u8g2.print("NoTemp");
       }
  }

 // refresh display
 u8g2.sendBuffer();
 delay(1000);
}