// read, format and display EGT temp
void displayEGT() {
  char txt[8];
  float temp1 = 0.0;
  float temp2 = 0.0;

  if (millis() - EGT_lastRefresh >= EGT_refreshInterval) {
    // read temp from sensor
    temp1 = EGT1.readCelsius();
    temp2 = EGT2.readCelsius();
    EGT1_temp = temp1 + 0.5;
    EGT2_temp = temp2 + 0.5;

    sprintf(txt, "%03d/%03d", EGT2_temp, EGT1_temp);

    // set the cursor to column 12, line 2
    lcd.setCursor(12, 1);

    lcd.print(txt);

    // ajouter symbole '°'
    lcd.print(char(223));

    EGT_lastRefresh = millis();
  }
  return;
}
