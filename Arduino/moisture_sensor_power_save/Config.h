#ifndef Config_h
#define Config_h

/********** NODE ID ********/
const int NODE_ID = 3;
/********** NODE ID ********/

#define FALSE 0
#define TRUE 1

enum output_format {
  DEBUG,
  XBEE
};



/* time for different sensor modes */
const short SENSOR_POWER_UP = 1 * 1000;
const short WDT_SLEEP_CYCLES = 45; /* 45 cycles is 8*45 seconds = 6 min */
const short XBEE_DELAY = 2*1000;


const char WRITE_TO_SD = FALSE;
const char OUTPUT_MODE = XBEE;


/* pin configuration */
const short WATERMARKVA_PIN = 0;     /* analog 0 for sensor signal */
const short watermarkEnablePin = 13; /* to turn on/off SSVA */
const short XBEE_SLEEP_PIN = 12;       /* set high to turn off xbee */
const short FAST_SAMPLE_PIN = 5; /* used to enable fast sampling for debug */


#endif
