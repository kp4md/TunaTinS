/*
 * The Tuna Tin S
 * by Bob Fontana AK3Y
 * 
 * Revised 09 April 2021 by Carol Milazzo KP4MD
 *   -- added default QRP frequencies and band switching for 80, 40, 60, 30, 20, 15 and 10 meters
 *   -- added band indicator to display
 * Revised 27 March 2021
 *   -- added provision for 5 fixed frequency CW channels on 60 meters
 *   -- fixed EEProm code so that header changes to Band will trigger rewrite
 * Revised 24 February 2021
 *   -- added timing loop to EEPROM write; current settings will not be written to nonvolatile
 *   -- memory until DELAY_TIME has been exceeded.  This prevents too many writes to EEPROM which
 *   -- can only handle around 100,000 writes
 * Revised 21 February 2021
 *   -- added EEPROM storage of last used frequency and tuning rate (loads upon reboot)
 *   -- Note: if only tuning rate is changed, data is not stored
 * Revised 12 December 2020
 *   -- added an additional tuning increment (10Hz, 100Hz and 1000Hz)
 *   -- pressing switch toggles from 1000 --> 100 --> 10 --> 1000 Hz
 * Revised 10 December 2020
 *   -- added 30 meters to frequency bands
 *   -- modified display to indicate tuning rate
 * Revised 3 March 2020 -- added Transmit/Receive function to kill synthesizer output on Receive
 * Revised 25 February 2020 -- added Calibration Offset
 *   -- Note: calibration offset should be computed for each band of operation
 * 
 * 12 February 2020
 */
 
#include <Wire.h>                 // Include I2L Library for Communications with OLED Display and Synthesizer
#include <Adafruit_SI5351.h>      // Include SI5351 Synthesizer Library
#include <Adafruit_SSD1306.h>     // Include OLED Display Library
#include <EEPROM.h>               // Include EEPROM Library

#define SCREEN_WIDTH      128     // OLED display width, in pixels
#define SCREEN_HEIGHT      32     // OLED display height, in pixels
#define E_INIT           1023     // Last position in EEPROM to determine if memory has been initialized


// KY040 Rotary Encoder Module connections
const int PinCLK=2;               // CLOCK (Interrupt line)  IMPORTANT! Decouple Pins 2 and 4 with 0.68uF caps to ground
const int PinSW=3;                // Push Button Switch
const int PinDT=4;                // Encoder DT Signal
const int TR_Pin=8;               // Add T/R switching capability to synthesizer board

/* 
 *  Define Band of Operation, Initial Frequency and Calibration Offset if needed
 *  NOTE: For 60 meters, there are only 5 frequencies allowable for the CW carrier
 *    5332, 5348, 5358.5, 5373 and 5405 kHz
 *    To start up for the first time on the nth channel, set Freq1 = n and Band = 60
 *    The Rotary Encoder will now be used to cycle through the 5 channels
 *    The push button switch will have not effect
 *    
*/
long Freq160 = 1810000;             // QRP Center Frequencies
long Freq80 = 3560000;              //
long Freq60 = 1;                    //
long Freq40 = 7030000;              // 
long Freq30 = 10106000;             //
long Freq20 = 14060000;             //
long Freq17 = 18096000;             //
long Freq15 = 21060000;             //
long Freq12 = 24906000;             //
long Freq10 = 28060000;             //
long Freq1 = 7030000;               // Set initial frequency/Channel
                                    // For 60 meter operation, Freq1 = Channel number (1..5)
int Band = 40;                      // Define frequency band (80, 60, 40, 30, 20, 15, 10)
                                    //
                                    // For example, to use Tuna Tin S on 60 meter band:
                                    //     Set Freq1 = 1, Band = 60
                                    // This will initiate the unit on Channel 1, or 5332 kHz
                                    //
int deltaFreq = 1000;               // Initial tuning rate (ignored for 60 meter operation)
const long Calibrate_Offset = 13460; // Set Calibration offset (see article and above note)  40m 520   10m 2080

long oldFreq = 0;
int TR_State = 0;

struct myStoredData {
  long freq;
  int Band;                         // Band is included for future mod to allow bandswitching
  int deltaFreq;
} D1;

const int eeAddress = 0;

unsigned long delayStart = 0;
unsigned long DELAY_TIME = 10000;  // 10 second delay before EEPROM is updated

Adafruit_SI5351 clockgen = Adafruit_SI5351();
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4);

void isr ()  {                    // Interrupt Service Routine runs if Rotary Encoder CLK pin falls
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {

    if (!digitalRead(PinDT)) {

       if (deltaFreq == 0) {
 switch (Band) {
      case 160:  Band = 10; Freq160 = Freq1; Freq1 = Freq10; break;
      case 80:  Band = 160; Freq80 = Freq1; Freq1 = Freq160; break;
      case 60:  Band = 80; Freq60 = Freq1; Freq1 = Freq80; break;
      case 40:  Band = 60; Freq40 = Freq1; Freq1 = Freq60; break;
      case 30:  Band = 40; Freq30 = Freq1; Freq1 = Freq40; break;
      case 20:  Band = 30; Freq20 = Freq1; Freq1 = Freq30; break;
      case 17:  Band = 20; Freq17 = Freq1; Freq1 = Freq20; break;
      case 15:  Band = 17; Freq15 = Freq1; Freq1 = Freq17; break;
      case 10:  Band = 15; Freq10 = Freq1; Freq1 = Freq15; break;
      default:  break;
    }
      }
      else {
      if (Band == 60) {
        Freq1 -= 1; 
        if (Freq1 <= 0) Freq1 = 5;
      }
      else Freq1 -= deltaFreq;
    }
    }
    else {

 if (deltaFreq == 0) {
  switch (Band) {
      case 160:  Band = 80; Freq160 = Freq1; Freq1 = Freq80; break;
      case 80:  Band = 60; Freq80 = Freq1; Freq1 = Freq60; break;
      case 60:  Band = 40; Freq60 = Freq1; Freq1 = Freq40; break;
      case 40:  Band = 30; Freq40 = Freq1; Freq1 = Freq30; break;
      case 30:  Band = 20; Freq30 = Freq1; Freq1 = Freq20; break;
      case 20:  Band = 17; Freq20 = Freq1; Freq1 = Freq17; break;
      case 17:  Band = 15; Freq17 = Freq1; Freq1 = Freq15; break;
      case 15:  Band = 10; Freq15 = Freq1; Freq1 = Freq10; break;
      case 10:  Band = 160; Freq10 = Freq1; Freq1 = Freq160; break;
      default:  break;
    }
      }   
      else {
      if (Band == 60) {
        Freq1 += 1; 
        if (Freq1 >= 6) Freq1 = 1;
      }
      else Freq1 += deltaFreq;
      }
    }
  }   
  lastInterruptTime = interruptTime;
}

void Splash()                     // Splash screen displayed at power on/reset
{  
  const char* text1 = "Tuna Tin S QRP";
  const char* text2 = "by";
  const char* text3 = "Bob Fontana AK3Y";

  display.clearDisplay();         // Clears the graphics display buffer.
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(display.width()/2 - strlen(text1)*6/2,0);   // Each character is approximately 6 pixels wide in default type font
  display.println(text1);
  display.setCursor(display.width()/2 - strlen(text2)*6/2,9); 
  display.println(text2);
  display.setCursor(display.width()/2 - strlen(text3)*6/2,18);
  display.println(text3);
  
  display.display();                                            // Need this command to refresh screen
}

void SetFreq(int Band, long Freq) {     // Use SI5351 Fractional Mode to set frequency at Port output
                                        // Port 0 uses PLL_A, Ports 1 and 2 use PLL_B
                                        // Freq in Hertz - 10 Hertz minimum resolution
                                        //    For 60 meters, Freq is the Channel Number; e.g., Freq = 1, 2, ... 5
                                        //    Calibration offset is ignored for 60 meter operation (chennelized)
  long d, k, l, m, n;
    
  if (Band == 60) {
    switch ((int)Freq) {
      case 1:  n = 164; break;
      case 2:  n = 196; break;
      case 3:  n = 217; break;
      case 4:  n = 246; break;
      case 5:  n = 310; break;
      default: break;
    }
    n -= 1;             // Adjust for Calibrate_Offset
  clockgen.setupPLL(SI5351_PLL_B, 21, n, 500);
  clockgen.setupMultisynth(1, SI5351_PLL_B, 100, 0, 1);
  }
  else {
   Freq -= int(Freq / Calibrate_Offset);             // Adjust for Calibrate_Offset
   switch (Band) {
      case 160: d = 6250; k = 400; l = 0; m = 1; Freq -= 1750000L; Freq /= 10L; break;
      case 80:  d = 12500; k = 200; l = 0; m = 1; Freq -= 3500000L; Freq /= 10L; break;
      case 40:  d = 25000; k = 100; l = 0; m = 1; Freq -= 7000000L; Freq /= 10L; break;
      case 30:  d = 250000; k = 70; l = 0; m = 1; Freq -= 10000000L; Freq *= 7L; Freq /= 10L; break;
      case 20:  d = 50000; k = 50; l = 0; m = 1; Freq -= 14000000L; Freq /= 10L; break;
      case 17:  d = 62500; k = 40; l = 0; m = 1; Freq -= 17500000L; Freq /= 10L; break;
      case 15:  d = 75000; k = 33; l = 1; m = 3; Freq -= 21000000L; Freq /= 10L; break;
      case 10:  d = 100000; k = 25; l = 0; m = 1; Freq -= 28000000L; Freq /= 10L; break;
      default:  break;
    }   
    clockgen.setupPLL(SI5351_PLL_B, 28, Freq, d);
    clockgen.setupMultisynth(1, SI5351_PLL_B, k, l, m);
  }

    /*                                                          // Set output Port 2 to 1/2 Port 1 frequency for companion receiver
    clockgen.setupMultisynth(2, SI5351_PLL_B, k, l, m);       
    clockgen.setupRdiv(2, SI5351_R_DIV_2);
    */
 }

void Display_Freq(long freq) {
  int f_int,f_frac;
  char buf[50];

  display.clearDisplay();         // Clears the graphics display buffer.

  if (Band != 60) { 
    f_int = freq/1000;
    f_frac = (freq-(long)f_int*1000)/10;
    String Bandstring = String(Band)+"m Freq (kHz)"; 
        
    display.setTextSize(1);
    display.setCursor(display.width() / 2 - (strlen("40m ") + strlen("Freq (kHz)")) * 6 / 2, 0); // Each character is approximately 6 pixels wide in default type font
    display.print(Bandstring);

    display.setTextSize(2);
    if (f_int <10000) {
        display.setCursor(display.width() / 2 - 38, 10);  
    }
    else {
        display.setCursor(display.width() / 2 - 50, 10);
    }
    if (f_frac < 10) {
        snprintf(buf,sizeof(buf),"%d.%d%d",f_int,0,f_frac);
    }
    else {
        snprintf(buf,sizeof(buf),"%d.%d",f_int,f_frac);
    }
    display.print(buf);
  
    display.setTextSize(1);  // Display delta frequency
    switch (deltaFreq) {
      case 0: display.setCursor(display.width() / 2 - strlen("Step Band") * 6 / 2 ,25); display.print("Step Band"); break;
      case 10: display.setCursor(display.width() / 2 - strlen("Step 10 Hz") * 6 / 2 ,25); display.print("Step 10 Hz"); break;
      case 100: display.setCursor(display.width() / 2 - strlen("Step 100 Hz") * 6 / 2 ,25); display.print("Step 100 Hz"); break;
      case 1000: display.setCursor(display.width() / 2 - strlen("Step 1 kHz") * 6 / 2 ,25); display.print("Step 1 kHz"); break;
      case 10000: display.setCursor(display.width() / 2 - strlen("Step 10 kHz") * 6 / 2 ,25); display.print("Step 10 kHz"); break; 
    }
  }
  else {
    display.setTextSize(1);
    display.setCursor(display.width() / 2 - 40, 0);
    String Bandstring = String(Band)+"m "; 
    display.print(Bandstring);
    switch(freq) {
      case 1: display.print("Channel 1"); break;
      case 2: display.print("Channel 2"); break;
      case 3: display.print("Channel 3"); break;
      case 4: display.print("Channel 4"); break;
      case 5: display.print("Channel 5"); break;
    }
    display.setTextSize(2);
    display.setCursor(display.width() / 2 - 38, 10);
      switch(freq) {
      case 1: display.print("5332.0"); break;
      case 2: display.print("5348.0"); break;
      case 3: display.print("5358.5"); break;
      case 4: display.print("5373.0"); break;
      case 5: display.print("5405.0"); break;   
    }
    display.setTextSize(1);
    if (deltaFreq == 0) { 
      display.setCursor(display.width() / 2 - strlen("Step Band") * 6 / 2 ,25); display.print("Step Band"); 
      }  
    else { 
      display.setCursor(display.width() / 2 - strlen("kHz") * 6 / 2 ,25); display.print("kHz"); } 
  }
  display.display();
}

void Check_TR_Status() {
    TR_State = digitalRead(TR_Pin);
    if(TR_State==LOW) { //Transmit
       clockgen.enableOutputs(true);  //Enable the clocks
    } else {           // Receive
       clockgen.enableOutputs(false);  //Disable the clocks
    }
 }

void loadSettings() {
  int tempBand;
  long tempFreq;
  if (EEPROM.read(E_INIT) == 'T') {           // EEProm has previously been loaded
    EEPROM.get(eeAddress,D1);
      if((tempBand = D1.Band) == Band)  {     // User has not changed operating band in header from previous load
        Freq1 = D1.freq;                      // OK to load EEProm memory information
        Band = D1.Band;
        deltaFreq = D1.deltaFreq;
      }  
  } else {                                    //  Initial EEProm load into new chip
    D1 = {Freq1,Band,1000};
    EEPROM.put(eeAddress,D1);
    EEPROM.write(E_INIT, 'T');
  }
}

void setup(void) 
{
  Serial.begin(19200);                                          // Initialize Serial Port

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);    // Initialize OLED Display
  clockgen.begin();                             // Initialize SI5351 Clock Generator

  pinMode(PinSW, INPUT);
  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);
  pinMode(TR_Pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(2),isr,FALLING);        // Pins 2 and 3 are interrupts on Arduino Nano

  Splash();                                     // Display Splash screen
  delay(3000);
  display.clearDisplay();
  display.display();

  loadSettings();
}

void loop()
{
  Check_TR_Status();
  if (!(digitalRead(PinSW)) && (Band == 60)) {   // Pushbutton pressed
  switch (deltaFreq) {
    case 0: deltaFreq = 1; break;
    case 1: deltaFreq = 0; break;
    }
  }
  if (!(digitalRead(PinSW)) && (Band != 60)) {   // Pushbutton pressed
  switch (deltaFreq) {
    case 0: deltaFreq = 10000; break;
    case 10: deltaFreq = 0; break;
    case 100: deltaFreq = 10; break;
    case 1000: deltaFreq = 100; break;
    case 10000: deltaFreq = 1000; break;
    }
  }
  while (!digitalRead(PinSW));
  delay(10);                    // Debounce switch

  Display_Freq(Freq1);
  
  if (Freq1 != oldFreq) {
    Serial.println(Freq1);
    Display_Freq(Freq1);
    SetFreq(Band,Freq1);
    oldFreq = Freq1;
    delayStart = millis();
   }

   if((millis()-delayStart)>=DELAY_TIME) {
      D1 = {Freq1,Band,deltaFreq};
      EEPROM.put(eeAddress,D1);            // EEPROM.put will not write values to EEPROM if they did not change
   }
}
