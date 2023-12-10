void saveQNH() {
  Serial.print("save QNH ");
  Serial.println(QNH);

  // write QNH into eeprom
  EEPROM.put(eeQNHAddress, QNH);
  lcd.setCursor(11, 3);
  lcd.print(" ");  // remove "s" on LCD as we are not in set mode anymore
}

void setQNH() {
  lcd.setCursor(11, 3);
  lcd.print("s"); // display "s" on LCD to show we are in set mode
  long newPosition = myEnc.read() / 4;
  if (newPosition != ENC_oldPosition) {
    QNH = QNH + newPosition - ENC_oldPosition;
    ENC_oldPosition = newPosition;
    displayQNH();
  }
}

void displayQNH() {
  Serial.print("Ref : ");
  Serial.println(QNH);

  // display QNH on lcd
  sprintf(buffer, "QNH %4d", QNH);
  lcd.setCursor(12, 3);
  lcd.print(buffer);
}

