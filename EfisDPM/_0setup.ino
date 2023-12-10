void setup() {
  Serial.begin(9600);
  Wire.begin();

  // initialize the rotative encoder switch pin as an input with pullup
  pinMode(ENC_SW, INPUT_PULLUP);

  // initialize the strobe switch pin as an input
  pinMode(STR_switchPin, INPUT_PULLUP);
  
  // initialize the strobe LEDs pin as output:
  pinMode(STR_pin, OUTPUT);
  pinMode(STR_indicPin, OUTPUT);

  // initialize the chrono switch pin as an input with pullup
  pinMode(CHR_switchPin, INPUT_PULLUP);

  // initialize the LCD
  lcd.begin();
  // Turn on the blacklight
  lcd.backlight();
  // create custom characters
  lcd.createChar(upArrowNum, upArrow);
  lcd.createChar(dwnArrowNum, dwnArrow);

  // initialize BMP280 pressure sensor
  initBMP280();

  // initialize RTC
  initRTC();

  // RAZ chrono
  chronoRAZ();

  // initialize ASI
  initASI();

  // search DS sensor, store status in DSstatus and address in DSaddr
  DS_status = getDSadress();
  if (DS_status != SENSOR_FOUND) displayNoTemp();

  // load and display last QNH from eeprom
  EEPROM.get(eeQNHAddress, QNH);
  if (QNH < 900 || QNH > 1100) {
    QNH = 1013;
  }
  displayQNH();

}
