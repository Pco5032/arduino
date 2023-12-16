byte getDSadress(void) {
  // search DS18B20 address
  ds.reset_search();
  if (!ds.search(DS_addr)) {
     return NO_SENSOR_FOUND;
  }

  /* Vérifie que l'adresse a été correctement reçue */
  if (OneWire::crc8(DS_addr, 7) != DS_addr[7]) {
    return INVALID_ADDRESS;
  }

  /* Vérifie qu'il s'agit bien d'un DS18B20 */
  if (DS_addr[0] != DS_ADDRESS) {
    return INVALID_SENSOR;
  }

  // sensor DS18B20 founbd
  return SENSOR_FOUND;
}

// init a temp request
void requestTemperatureMeasure() {
  /* Reset le bus 1-Wire et sélectionne le capteur */
  ds.reset();
  ds.select(DS_addr);

  /* Lance une prise de mesure de température et attend la fin de la mesure */
  ds.write(0x44, 1);
}

// get temp after delay following a temp request
byte getTemperature(float *outsideTemperature) {
  int i;
  byte data[9];
  if (millis() - DS_betweenTempMillis >= DS_betweenTempMeasure) {
    DS_betweenTempMillis = millis();

    /* Reset le bus 1-Wire, sélectionne le capteur et envoie une demande de lecture du scratchpad */
    ds.reset();
    ds.select(DS_addr);
    ds.write(0xBE);

    /* Lecture du scratchpad */
    for (byte i = 0; i < 9; i++) {
      data[i] = ds.read();
    }

    /* Calcul de la température en degré Celsius */
    *outsideTemperature = ((data[1] << 8) | data[0]) * 0.0625;

    // Pas d'erreur
    return READ_OK;
  }
  else {
    return READ_WAIT;
  }
}

// display temp
void displayTemp(float temp) {
  // set the cursor position
  u8g2.setCursor(0, 35);
  // print with 1 decimal
  u8g2.print(temp, 1);
  u8g2.print("°C");
  return;
}

