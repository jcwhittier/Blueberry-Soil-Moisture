#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#include "Watermark.h"
#include "Config.h"

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>




/* function prototypes */
void updateDisplay(WatermarkReading* watermarkReading);
void displayMessage(String message);
void writeReading(WatermarkReading* watermarkReading);
void enterSleep(void);
ISR(WDT_vect);
void configureWDT(void);
int isFastSampleEnabled(void);

/* Attach the serial display's RX line to digital pin 2 */
SoftwareSerial mySerial(3, 2); /* pin 2 = TX, pin 3 = RX (unused) */

File myFile;
WatermarkReading currentWatermarkReading;
volatile int f_wdt = 1;
short sleepCounter = 1;



void setup() {

  mySerial.begin(9600);

  Serial.begin(9600);

  pinMode(10, OUTPUT);

  if (WRITE_TO_SD) {
    if (!SD.begin(4)) {
      Serial.println("initialization failed!");
      return;
    }
  }

    if (OUTPUT_MODE == DEBUG) {
      Serial.println("Setup Complete");
    }

  configureWDT();


  delay(100);
  
  displayMessage("Setup Complete");
  delay(100);
 // setupComplete = true;

}

void loop() {
  
  if (f_wdt == 1)
  {
    f_wdt = 0;

    if ( ((sleepCounter % WDT_SLEEP_CYCLES) == 0 ) || isFastSampleEnabled()){
      sleepCounter = 1;

      /* take the reading: turn the sensor on, wait, read, turn off */
      Watermark::doReadWatermark(&currentWatermarkReading);

      /* transmit the result */
      writeReading(&currentWatermarkReading);

      /* update the LCD if attached */
      updateDisplay(&currentWatermarkReading);

    } else {
      sleepCounter++;
    }

    /* power down until next WDT */
    enterSleep();

  } else {
    Serial.println("failed to sleep properly");
  }
}




