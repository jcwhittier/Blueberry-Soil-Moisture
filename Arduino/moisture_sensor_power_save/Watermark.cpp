#include "Arduino.h"
#include "Watermark.h"
#include "Config.h"



void Watermark::readWatermark(int watermarkPin, WatermarkReading *currentReading) {



  /* take the reading */
  currentReading->watermarkVAResponse = analogRead(watermarkPin);

  /* process the reading */
  currentReading->watermarkVAVoltage = ANALOG_TO_V * currentReading->watermarkVAResponse;
  double watermarkVACB = currentReading->watermarkVAVoltage / WATERMARK_V_TO_CB;

  /* Convert the voltage to a string */
  char tempString[10];
  dtostrf(currentReading->watermarkVAVoltage, 1, 4, tempString);
  currentReading->watermarkVAVoltageString = String(tempString);
  currentReading->watermarkVAVoltageString.trim();

  /* Convert the moisture to a string */
  dtostrf(watermarkVACB, 3, 3, tempString);
  currentReading->watermarkVACBString = String(tempString);
  currentReading->watermarkVACBString.trim();


}

void Watermark::doReadWatermark(WatermarkReading *currentReading) {

  /* configure the pin to power on the Watermark and enable */
  pinMode(watermarkEnablePin, OUTPUT);
  digitalWrite(watermarkEnablePin, HIGH);

  /* wait while the sensor powers on */
  delay(SENSOR_POWER_UP);

  /* read the sensor */
  readWatermark(WATERMARKVA_PIN, currentReading);

  /* turn off the sensor */
  digitalWrite(watermarkEnablePin, LOW);
}
