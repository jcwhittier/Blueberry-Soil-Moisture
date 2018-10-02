#ifndef Watermark_h
#define Watermark_h

#include "Arduino.h"

const double WATERMARK_V_TO_CB = 0.0117155;
const double ANALOG_TO_V = ((double)5.) / ((double)1023.); 

typedef struct WatermarkReadings {
  short watermarkVAResponse;
  double watermarkVAVoltage;
  String watermarkVAVoltageString;
  String watermarkVACBString;
} WatermarkReading;



class Watermark
{
  public:
    static void readWatermark(int watermarkPin, WatermarkReading *currentReading); 
    static void doReadWatermark(WatermarkReading *currentReading);   
};


#endif
