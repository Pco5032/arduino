byte getDSadress(void) {
  int i;

  // search DS18B20 address
  ds.reset_search();
  if (!ds.search(DS_addr)) {
    Serial.println("No DS18B20 temp sensor found");
    return NO_SENSOR_FOUND;
  }

  /* Vérifie que l'adresse a été correctement reçue */
  if (OneWire::crc8(DS_addr, 7) != DS_addr[7]) {
    Serial.println("Invalid address");
    return INVALID_ADDRESS;
  }

  /* Vérifie qu'il s'agit bien d'un DS18B20 */
  if (DS_addr[0] != DS_ADDRESS) {
    Serial.println("Detected sensor is not DS18B20");
    return INVALID_SENSOR;
  }

  Serial.print("DS18B20 found ROM =");
  for (i = 0; i < 8; i++) {
    Serial.write(' ');
    Serial.print(DS_addr[i], HEX);
  }
  Serial.println();
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
  char txt[7];
  // set the cursor to column 11, line 0
  // (note: line 0 is the 1st row, since counting begins with 0):
  lcd.setCursor(14, 0);
  
  // format and display temp
  dtostrf(temp, 3, 1, txt);

  // remplacer les caractères non utilisés par un espace pour aligner l'affichage à droite
  if (abs(temp) < 10)
    {
    lcd.print(" ");
  }
  if (temp >= 0)
  {
    lcd.print(" ");
  }
  lcd.print(txt);
  
  // ajouter symbole '°'
  lcd.print(char(223));
  return;
}

// display temp not avail
void displayNoTemp()
{
  // set the cursor to column 11, line 0
  lcd.setCursor(14, 0); 
  lcd.print("NoTemp");
}
