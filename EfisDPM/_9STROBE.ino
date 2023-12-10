void strobe() {
  // read input from strobe switch (reverse read value because of PULLUP).
  int swState = !digitalRead(STR_switchPin);
  // swState = HIGH; // debug use

  // switch state changed
  if (STR_switchState != swState) {
    STR_switchState = swState;
    if (STR_switchState == LOW) {
      // erase strobe symbol from LCD
      lcd.setCursor(11, 0);
      lcd.print(" ");
      // set strobe signal to OFF
      digitalWrite(STR_pin, LOW);
      digitalWrite(STR_indicPin, LOW);
    }
    else {
      // display strobe symbol on LCD
      lcd.setCursor(11, 0);
      lcd.print("S");
    }
  }
  // switch is on : blink strobe
  if (STR_switchState == HIGH)
    blinkStrobe();
}

void blinkStrobe() {
  // if betweenStrobeDuration elapsed, start blinking sequence
  if (millis() - STR_betweenStrobeMillis >= STR_betweenFlashDuration) {
    STR_betweenStrobeMillis = millis();
    digitalWrite(STR_pin, HIGH);
    digitalWrite(STR_indicPin, HIGH);
    delay(STR_ONDuration);
    digitalWrite(STR_pin, LOW);
    digitalWrite(STR_indicPin, LOW);
    delay(STR_OFFDuration);
    digitalWrite(STR_pin, HIGH);
    digitalWrite(STR_indicPin, HIGH);
    delay(STR_ONDuration);
    digitalWrite(STR_pin, LOW);
    digitalWrite(STR_indicPin, LOW);
  }
}

