void loop() {
  // read rotative encoder
  readEncoder();

  // manage strobe
  strobe();

  // manage chrono
  chrono();

  // read pressure from sensor
  pressure = readPressure();

  // read and display air speed
  getAirSpeed();
  displayAirSpeed();

  // compute altitude
  altitude = readAltitude(QNH) + 0.5; // convert and round float to int
  if (altitude > 200 && altitude < 500) {
    if (altitude > ALT_max) ALT_max = altitude;
    if (altitude < ALT_min) ALT_min = altitude;
  }

  // compute VSI
  vsi = readVsi();

  // RTC
  displayRTC();

  // read and display temp from sensor
  if (DS_status == SENSOR_FOUND) {
    requestTemperatureMeasure();
    if (getTemperature(&outsideTemperature) == READ_OK) {
      displayTemp(outsideTemperature);
    }
  }

  // display ALT and VSI on LCD (only if pressure sensor is installed)
  if (BMP280_isThere) {
    if (millis() - LCD_lastRefresh >= LCD_refreshInterval) {
      // ALT
      sprintf(buffer, "ALT%5dft", altitude);
      lcd.setCursor(0, 3);
      lcd.print(buffer);

      // VSI
      if (vsi >= VSI_upThreshold || vsi <= VSI_downThreshold) {
        sprintf(buffer, "VSI%5d", abs(vsi));
        lcd.setCursor(12, 2);
        lcd.print(buffer);
        lcd.setCursor(15, 2);
        if (vsi > 0) {
          lcd.write(upArrowNum);
        }
        else if (vsi < 0) {
          lcd.write(dwnArrowNum);
        }
      }
      else
      {
        sprintf(buffer, "VSI%5d", 0);
        lcd.setCursor(12, 2);
        lcd.print(buffer);
        lcd.setCursor(15, 2);
        lcd.print(" ");
      }
      LCD_lastRefresh = millis();
    }
  }

  displayEGT();

  // display on SERIAL
  if (SERIAL_active and (millis() - SERIAL_lastRefresh >= SERIAL_refreshInterval)) {
    Serial.print("Température: ");
    Serial.print(bmp280.readTempC(), 2);
    Serial.print(" °C");
    Serial.print("\t Pression: ");
    Serial.print(pressure);
    Serial.print("\t altitude : ");
    Serial.print(altitude);
    Serial.print(" alt.min : ");
    Serial.print(ALT_min);
    Serial.print(" alt.max : ");
    Serial.print(ALT_max);
    Serial.print("\t airspeed : ");
    Serial.print(airspeed);
    Serial.print("\t vsi : ");
    Serial.print(vsi);
    Serial.print("\t EGT : ");
    Serial.print(EGT1_temp);
    Serial.print("/");
    Serial.println(EGT2_temp);
    SERIAL_lastRefresh = millis();
  }
}
