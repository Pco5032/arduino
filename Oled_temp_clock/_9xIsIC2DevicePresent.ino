bool isIC2DevicePresent(byte adress) {
  byte error = 0;

  // check if device is present
  Wire.beginTransmission(adress); // transmit to device (device address is specified in datasheet)
  error = Wire.endTransmission();
  if (error == 0) {
    return true;
  } else {
    return false;
  }
}
