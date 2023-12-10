void initASI() {
  int sumReadings = 0;

  // fill reading array with 0
  for (ASI_iReading = 0; ASI_iReading < ASI_nReadings; ASI_iReading++) {
    ASI_reading[ASI_iReading] = 0;
  }

  // some unused readings for stabilization
  for (ASI_iReading = 1; ASI_iReading <= 10; ASI_iReading++)
  {
    delay(ASI_warmUpTime);
    analogRead(ASI_pin);
  }

  // Make some readings and average them. It will be the zero reference.
  for (ASI_iReading = 1; ASI_iReading <= ASI_nZeroReadings; ASI_iReading++)
  {
    delay(ASI_warmUpTime);
    sumReadings = sumReadings + analogRead(ASI_pin);
  }
  ASI_zeroReading = float(sumReadings) / ASI_nZeroReadings;
  Serial.print("ASI_zeroReading: ");
  Serial.println(ASI_zeroReading);
}

void getAirSpeed() {
  int currentReading;
  float dynPress = 0.0;
  float avgReadings = 0.0;
  float newAirspeed = 0.0;

  // read new value only if warmup time is elapsed
  if (millis() - ASI_readTime < ASI_warmUpTime) {
    return;
  }

  ASI_readTime = millis();

  // read current sensor value
  currentReading = analogRead(ASI_pin);

  // Substract previous sampling value
  ASI_sumReadings = ASI_sumReadings - ASI_reading[ASI_iReading];

  // Store current sensor value in array
  ASI_reading[ASI_iReading] = currentReading;

  // Add last sensor value
  ASI_sumReadings = ASI_sumReadings + ASI_reading[ASI_iReading];

  // Increment index. At end of array, go back to beginning.
  ASI_iReading++;
  if (ASI_iReading >= ASI_nReadings - 1) {
    ASI_iReading = 0;
  }

  // compute average
  avgReadings = float(ASI_sumReadings) / ASI_nReadings;
//    Serial.print("ASI_sumReadings: ");
//    Serial.print(ASI_sumReadings);
//    Serial.print(" avgReadings: ");
//    Serial.println(avgReadings);

  // compare to 'reference + 1.5' to limit noise (discard under ~ 12km/h)
  if (avgReadings > ASI_zeroReading + 1.5) {
    dynPress = (avgReadings - ASI_zeroReading) * ASI_analogUnitValue;
    newAirspeed = sqrt((2 * dynPress) / ASI_airDensity) * 3.6;
    // rounding to integer
    airspeed = newAirspeed + 0.5;
  }
  else
  {
    airspeed = 0;
  }
}

void displayAirSpeed() {
  if (millis() - ASI_lastDisplay < ASI_displayInterval) {
    return;
  }
  ASI_lastDisplay = millis();

  // set the cursor to column 0, line 3
  lcd.setCursor(0, 2);

  // format and display ASI
  sprintf(buffer, "ASI%3d   ", airspeed);
  lcd.print(buffer);
}

