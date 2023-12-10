#include <LiquidCrystal_I2C.h>
#include "SparkFunBME280.h" // version 1.2, pas au-delà !
#include "Wire.h"
#include <EEPROM.h>
#include "Encoder.h"
// Date and time functions using a DS3231 RTC connected via I2C
#include "RTClib.h"
// include the OneWire library used by DS18B20 temperature sensor
#include "OneWire.h"
#include <max6675.h>

const int eeQNHAddress = 0; // QNH EEPROM address
bool menuMode = false;
bool setQNHMode = false;

// final vars
int altitude = 0;      // feet
int vsi = 0;           // feet/min
int airspeed = 0;      // km/h
float pressure = 0.0;  // hPa
float outsideTemperature = 0.0; // °C

// Serial.print display vars
bool SERIAL_active = false; // serial print in loop or not
int SERIAL_refreshInterval = 500; // milliseconds
unsigned long SERIAL_lastRefresh = 0;

// Rotative encoder vars
// NB : Digital Pins Usable For external Interrupts, Uno, Nano, Mini, other 328-based : pins 2, 3.
//      If not avail, will use "pin change" instead of "external" interrupt.
const int ENC_A = 2;   // encoder's A output
const int ENC_B = 3;   // encoder's B output
const int ENC_SW = 4;  // encoder's click button
long ENC_oldPosition = 0;

unsigned long ENC_buttonTimer = 0;
bool ENC_buttonActive = false;
bool ENC_longPressActive = false;
long ENC_longPressTime = 1000;

// Pression vars
#define BMP280_ADDRESS 0x76
bool BMP280_isThere = false;

// Altitude vars
int QNH = 0;
int ALT_index = 0;
float ALT_sum = 0.0;
const int ALT_maxIndex = 10;
int ALT_max = 0;
int ALT_min = 10000;

// ASI vars
const int ASI_pin = A0;             // MPXV7002DP Vout pin - A0
const float ASI_airDensity = 1.225; // 1.225 k/m3 is standard air density
const float ASI_analogUnitValue = 3.90625;   // analogRead value range from 0 to 1023, MPXV7002DP from -2kPa to +2Kpa, that makes 3.90625Pa per unit
const int ASI_warmUpTime = 20;     // time (ms) required for the product to meet the specified output voltage after the Pressure has been stabilized.
const int ASI_nZeroReadings = 50;  // number of readings to do and average to compute zero reference
const int ASI_nReadings = 10;      // number of readings to do and average to compute current value
int ASI_reading[ASI_nReadings];    // array of readings to average
int ASI_sumReadings = 0;           // sum of ASI_reading array items
int ASI_iReading = 0;              // index
float ASI_zeroReading = 0.0;       // sensor output at zero reference (atmospheric pressure is a zero reference for gauge type measurement)
unsigned long ASI_readTime = 0;    // time when sensor was last read
unsigned long ASI_lastDisplay = 0; // time when ASI was last displayed
int ASI_displayInterval = 500;     // interval between ASI display

// VSI vars
int VSI_index = 0;
int VSI_upThreshold = 90; // ft/min
int VSI_downThreshold = -90; // ft/min
int VSI_refreshInterval = 1000;
unsigned long VSI_lastRefresh = 0;
float VSI_sum = 0.0;
int VSI_prevAltitude = 0;

// LCD vars
int LCD_pressure = 0;
int LCD_refreshInterval = 1000; // milliseconds
unsigned long LCD_lastRefresh = 0;
char buffer[20];
const byte upArrowNum = 0;
const byte dwnArrowNum = 1;

// custom characters :
byte upArrow[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
};
byte dwnArrow[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100,
};

// CHRONO vars
// Set chrono switch pin number
const int CHR_switchPin = 6;
// chrono switch time settings
unsigned long CHR_lastPressedTime = 0;  // the last time the switch was pressed
unsigned long CHR_RAZtime = 5000;
unsigned long CHR_RESTARTtime = 1000;
unsigned long CHR_STARTSTOPtime = 70;
// return from check chrono switch
enum CHRONO_ACTION {
  NONE,
  STARTSTOP,
  RESTART,
  RAZ
};
byte CHR_todo;
unsigned long CHR_displayInterval = 1000;
unsigned long CHR_displayLast;
unsigned long CHR_millisStart;
boolean CHR_running = false;
// button state
int CHR_lastButtonState = LOW;

// STRobe vars
// Set strobe pin numbers
const int STR_switchPin = 11; // the number of the strobe switch pin
const int STR_indicPin = 13;  // the number of the indicator LED pin
const int STR_pin = 10;       // the number of the strobe LED pin
const int STR_ONDuration = 80;
const int STR_OFFDuration = 50;
const int STR_betweenFlashDuration = 1500; // time between flashes
unsigned long STR_betweenStrobeMillis = 0;
int STR_switchState = LOW;    // the current strobeSwitch state

// RTC vars
#define DS3231_ADDRESS  0x68
bool RTC_isThere = false;
unsigned long RTC_lastRefresh = 0;
bool RTC_setH = false; // si TRUE : setting des heures, si FALSE : setting des minutes

// EGT vars
int EGT1_DO = 7; 
int EGT1_CS = 8; 
int EGT1_CLK = 9;
int EGT2_DO = A1; 
int EGT2_CS = A2; 
int EGT2_CLK = A3;
int EGT1_temp;
int EGT2_temp = 0;
int EGT_refreshInterval = 1000;
unsigned long EGT_lastRefresh = 0;

// TEMP vars : set DS18B20 temperature sensor pin number
const int DS18B20Pin = 5;
#define DS_ADDRESS  0x28
byte DS_addr[8];
byte DS_status;
const int DS_betweenTempMeasure = 1000; // Duree entre les mesures de température
unsigned long DS_betweenTempMillis = 0;

/* Return from function getTemperature() used by DS18B20 */
enum DS18B20_RCODES {
  READ_OK,  // Lecture ok
  NO_SENSOR_FOUND,  // Pas de capteur
  INVALID_ADDRESS,  // Adresse reçue invalide
  INVALID_SENSOR,  // Capteur invalide (pas un DS18B20)
  SENSOR_FOUND,
  READ_WAIT
};

// declare LCD and set address to 0x3f for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x3f, 20, 4);

// declare pressure sensor
BME280 bmp280;

// declare rotary encoder
Encoder myEnc(ENC_A, ENC_B);

// declare rtc object
RTC_DS3231 rtc;

// declare DS18B20 temp sensor
OneWire ds(DS18B20Pin);

// declare EGT
MAX6675 EGT1(EGT1_CLK, EGT1_CS, EGT1_DO);
MAX6675 EGT2(EGT2_CLK, EGT2_CS, EGT2_DO);


