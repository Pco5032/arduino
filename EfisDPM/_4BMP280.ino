boolean initBMP280() {
  byte error = 0;

  // check if device is present
  if (isIC2DevicePresent(BMP280_ADDRESS)) {
    BMP280_isThere = true;

    // configuration du capteur
    bmp280.settings.commInterface = I2C_MODE;
    bmp280.settings.I2CAddress = BMP280_ADDRESS;
    bmp280.settings.runMode = 3;
    bmp280.settings.tStandby = 0;
    bmp280.settings.filter = 4;
    bmp280.settings.tempOverSample = 1 ;
    bmp280.settings.pressOverSample = 4;
    bmp280.settings.humidOverSample = 0; // no humidity sensor on BMP280, only on BME280
    delay(10);  // attente de la mise en route du capteur. 2 ms minimum

    // chargement de la configuration du capteur
    bmp280.begin();
    delay(10);
  }
  else {
    BMP280_isThere = false;
    Serial.println("No BMP280 pressure sensor found");
    lcd.setCursor(0, 3);
    lcd.print("noALT");
    lcd.setCursor(15, 2);
    lcd.print("noVSI");
  }
  return BMP280_isThere;
}

float readPressure() {
  float currentPressure = 0.0;
  if (BMP280_isThere) {
    bmp280.readTempC(); // !!!! si on ne fait pas cette lecture de temp, la pression lue ensuite n'est pas correcte !!!!
    currentPressure = bmp280.readFloatPressure() / 100;
    return(currentPressure);
  }
}

// calcul altitude au moyen de la formule du nivellement barométrique
// argument 0.190263 = 1/5.255
// argument 3.28084 : conversion en feet
float readAltitude(float seaLevelhPa) {
  float alt;
  alt = 44330.0 * (1.0 - pow((float)pressure / (float)seaLevelhPa, 0.1903)) * 3.28084;
  // averaging
  ALT_index++;
  ALT_sum += alt;
  if (ALT_index >= ALT_maxIndex) {
    alt = ALT_sum / ALT_index;
    ALT_index = 0;
    ALT_sum = 0;
  }
  else
  {
    alt = altitude;
  }
  return alt;
}

// compute VSI and smooth the value
int readVsi() {
  float currentVsi = 0;
  int diffTime = 0;
  
  diffTime = millis() - VSI_lastRefresh;
  VSI_sum += altitude - VSI_prevAltitude;
  if (diffTime >= VSI_refreshInterval) {
    currentVsi = VSI_sum * 60000 / diffTime; // ft/minute
    VSI_sum = 0;
    VSI_lastRefresh = millis();
  }
  else
  {
    currentVsi = vsi;
  }
  VSI_prevAltitude = altitude;
  return (currentVsi);
}


