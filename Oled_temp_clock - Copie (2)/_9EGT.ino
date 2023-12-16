// read, format and display EGT temp
void displayEGT() {
  float readTemp = 0.0;
  
  // read temp from sensor
  readTemp = EGT1.readCelsius();
  
  // set the cursor position
  u8g2.setCursor(0, 35);
  // print with no decimal
  u8g2.print(readTemp, 0);
  u8g2.print("°C");
  return;
}
