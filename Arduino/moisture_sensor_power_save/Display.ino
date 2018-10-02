
void displayMessage(String message) {
  mySerial.write(254); // move cursor to beginning of first line and print
  mySerial.write(128);

  /* clear the display */
  mySerial.write("                ");
  mySerial.write("                ");

  mySerial.write(254); // move cursor to beginning of first line and print
  mySerial.write(128);

  mySerial.print(message);

}

void updateDisplay(WatermarkReading* watermarkReading) {

  mySerial.write(254); // move cursor to beginning of first line and print
  mySerial.write(128);
  mySerial.write("Moisture (kPa): ");

  mySerial.write(254); // move cursor to beginning of second line and clear
  mySerial.write(192);
  mySerial.write("                ");

  mySerial.write(254); // move cursor to beginning of second line and print
  mySerial.write(192);
  mySerial.print(watermarkReading->watermarkVACBString);

}

void writeReading(WatermarkReading* watermarkReading) {

  /* Write to console */
  if (OUTPUT_MODE == DEBUG) {
    Serial.println("Analog pin " + String(WATERMARKVA_PIN, DEC) + " value is " + String(watermarkReading->watermarkVAResponse, DEC));
    Serial.println("Soil moisture voltage is " + watermarkReading->watermarkVAVoltageString);
    Serial.println("Soil moisture pressure is " + watermarkReading->watermarkVACBString);
  }

  /* Format Xbee string */
  String logString = String(NODE_ID, DEC) + ", "
                     + String(watermarkReading->watermarkVAResponse, DEC) + ", "
                     +  watermarkReading->watermarkVAVoltageString + ", "
                     + watermarkReading->watermarkVACBString;

  /* turn the XBee on */
  pinMode(XBEE_SLEEP_PIN, OUTPUT);
  digitalWrite(XBEE_SLEEP_PIN, LOW);
 
  /* wait for the XBee to wakeup */
  delay(XBEE_DELAY);
 
  /* transmit the data */
  Serial.println(logString);
 
  /* wait for the XBee before sleeping again */
  delay(XBEE_DELAY);

  /* put the XBee to sleep unil next observation */
  digitalWrite(XBEE_SLEEP_PIN, HIGH);
}
