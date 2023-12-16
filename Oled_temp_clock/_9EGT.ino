// read, format and display EGT temp
void displayEGT() {
  float readTemp1 = 0.0;
  float readTemp2 = 0.0;
  
  // read temp from sensor
  readTemp1 = EGT1.readCelsius();
  readTemp2 = EGT2.readCelsius();
  
  // display EGT1
  u8g2.setCursor(y1, x1);
  // print with no decimal
  u8g2.print(readTemp1, 0);
  u8g2.print("°C");

  // display EGT2
  u8g2.setCursor(y2, x2);
  // print with no decimal
  u8g2.print(readTemp2, 0);
  u8g2.print("°C");
  return;
}
