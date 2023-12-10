void chrono() {
  CHR_todo = checkChronoSwitch();
  if (CHR_todo == STARTSTOP) {
    if (CHR_running) {
      chronoStop();
    } else {
      chronoStart();
    }
  } else if (CHR_todo == RESTART) {
    chronoReStart();
  } else if (CHR_todo == RAZ) {
    chronoRAZ();
  }
  if (CHR_running) {
    displayChrono();
  }
}

byte checkChronoSwitch() {
  unsigned long pressedTime;

  // read input from chrono switch (reverse read value because of PULLUP).
  int reading = !digitalRead(CHR_switchPin);

  // If the switch changed state and is now pressed, record at what time
  if (reading != CHR_lastButtonState && CHR_lastButtonState == LOW) {
    CHR_lastPressedTime = millis();
  }

  // check if button is released
  if (reading == LOW && CHR_lastButtonState == HIGH) {
    char txt[8];
    pressedTime = millis() - CHR_lastPressedTime;
  }
  CHR_lastButtonState = reading;

  if (pressedTime >= CHR_RAZtime) {
    return (RAZ);
  } else if (pressedTime >= CHR_RESTARTtime) {
    return (RESTART);
  } else if (pressedTime >= CHR_STARTSTOPtime) {
    return (STARTSTOP);
  } else return (NONE);

}

void chronoStart() {
  if (CHR_millisStart == 0) {
    CHR_millisStart = millis();
  }
  CHR_running = true;
}

void chronoStop() {
  CHR_running = false;
}

void chronoReStart() {
  CHR_millisStart = millis();
  CHR_running = true;
}

void chronoRAZ() {
  CHR_millisStart = 0;
  CHR_running = false;
  // set the cursor to column 0, line 1 and display on LCD
  lcd.setCursor(0, 1);
  lcd.print("00:00:00");
}

void displayChrono() {
  if (millis() - CHR_displayLast < CHR_displayInterval) {
    return;
  }
  CHR_displayLast = millis();

  char txt[8];
  unsigned long elapsedFromStart = millis() - CHR_millisStart;
  int hr = elapsedFromStart / 3600000;
  int min = elapsedFromStart / 60000 - (hr * 60);
  int sec = (elapsedFromStart / 1000) - (hr * 3600) - (min * 60);

  // format and display current chrono time
  sprintf(txt, "%02d:%02d:%02d", hr, min, sec);
  lcd.setCursor(0, 1);
  lcd.print(txt);
}
