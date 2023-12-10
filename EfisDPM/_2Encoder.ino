void readEncoder() {
  // Read input from encoder switch (reverse read value because of PULLUP).
  int reading = !digitalRead(ENC_SW);

  if (reading == HIGH) {
    if (ENC_buttonActive == false) {
      ENC_buttonActive = true;
      ENC_buttonTimer = millis();
    }
    if ((millis() - ENC_buttonTimer > ENC_longPressTime) && (ENC_longPressActive == false) && !setQNHMode) {
      ENC_longPressActive = true;
      menuMode = !menuMode;
      if (menuMode) RTC_setH = true;
      else lcd.noCursor();

    }
  }
  else
  {
    if (ENC_buttonActive == true) {
      if (ENC_longPressActive == true) {
        ENC_longPressActive = false;
      }
      else
      {
        if (menuMode) {
          RTC_setH = !RTC_setH;
        }
        else
        {
          setQNHMode = !setQNHMode;
          if (setQNHMode) ENC_oldPosition = myEnc.read() / 4; // pour repartir de la position actuelle
          if (!setQNHMode) saveQNH(); // quit menu mode : save QNH into EEPROM
        }
      }
      ENC_buttonActive = false;
    }
  }

  // If we are in "set QNH mode", rotating encoder modifies QNH.
  if (setQNHMode) {
    setQNH();
  }

  // If we are in "menuMode", rotating encoder sets clock.
  if (menuMode) {
    setRTC();
  }
}

