boolean initRTC() {
  // check if device is present
  if (isIC2DevicePresent(DS3231_ADDRESS)) {
    RTC_isThere = true;
    rtc.begin();
    if (rtc.lostPower()) {
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
      // This line sets the RTC with an explicit date & time, for example to set
      // Dec 16, 2023 at 3am you would call:
      // rtc.adjust(DateTime(2023, 12, 16, 3, 0, 0));
    }
  } 
  else {
    RTC_isThere = false;
  }
  return RTC_isThere;
}

void displayRTC() {
  u8g2.setCursor(0, 0);
  if (RTC_isThere) {
     
     // read current time
     DateTime now = rtc.now();
     // format and display current time
     sprintf(buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
     u8g2.print(buffer);
  } 
  else {
     u8g2.print("NoClock");
  }
}
