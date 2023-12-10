boolean initRTC() {
  byte error = 0;

  // check if device is present
  if (isIC2DevicePresent(DS3231_ADDRESS)) {
    RTC_isThere = true;
    rtc.begin();
    if (rtc.lostPower()) {
      lcd.setCursor(0, 0);
      lcd.print("set RTC !");
      delay(3000);
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // This line sets the RTC with an explicit date & time, for example to set
      // January 21, 2014 at 3am you would call:
      // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
    }
  }
  else {
    Serial.println("No DS3231 clock found");
    RTC_isThere = false;
    lcd.setCursor(0, 0);
    lcd.print("NoClock");
  }
  return RTC_isThere;
}

void displayRTC() {
  if (RTC_isThere) {
    // refresh current time every second
    if (millis() - RTC_lastRefresh >= 1000) {
      RTC_lastRefresh = millis();
      // read current time
      DateTime now = rtc.now();

      // set the cursor to column 0, line 0
      lcd.setCursor(0, 0);

      // format and display current time
      sprintf(buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
      lcd.print(buffer);
    }
  }
}

void setRTC() {
  int hour = 0;
  int min = 0;
  if (RTC_isThere) {
    // read current time
    DateTime now = rtc.now();
    hour = now.hour();
    min = now.minute();

    long newPosition = myEnc.read() / 4;
    if (RTC_setH) {
      lcd.setCursor(1, 0);
      if (newPosition != ENC_oldPosition) {
        hour = hour + newPosition - ENC_oldPosition;
        if (hour > 23) hour = 0;
        if (hour < 0) hour = 23;
      }
    }
    else {
      lcd.setCursor(4, 0);
      if (newPosition != ENC_oldPosition) {
        min = min + newPosition - ENC_oldPosition;
        if (min > 59) min = 0;
        if (min < 0) min = 59;
      }
    }
    lcd.cursor(); // make cursor visible under hour or second
    ENC_oldPosition = newPosition;
    rtc.adjust(DateTime(now.year(), now.month(), now.day(), hour, min, now.second()));
  }
}

