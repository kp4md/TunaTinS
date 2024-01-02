/*
   The Tuna Tin S v2.1.0
   Original sketch by Bob Fontana, AK3Y
   Revisions since 09 April 2021 by Carol Milazzo, KP4MD
   See Changelog at https://github.com/kp4md/TunaTinS/
   New features with change from the Adafruit to the Etherkit Si5351 library
   4 kHz - 225 MHz continuous coverage, frequency steps 1 MHz, 100 kHz, 10 kHz, 1 kHz, 100 Hz, 10 Hz and 1 Hz
   Use TunaTinS Calibration Utility v2.1.0 to calibrate the Si5351 frequency to within 1 Hz at 10 MHz
*/

#include <Wire.h>              // Include I2L Library for Communications with OLED Display and Synthesizer
#include <Adafruit_SSD1306.h>  // Include OLED Display Library
#include <EEPROM.h>            // Include EEPROM Library
#include "si5351.h"            // Include Etherkit Si5351 Library

Si5351 si5351;

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels
#define E_INIT 1023       // Last position in EEPROM to determine if memory has been initialized

// KY040 Rotary Encoder Module connections
const int PinCLK = 2;  // CLOCK (Interrupt line)  IMPORTANT! Decouple Pins 2 and 4 with 0.68uF caps to ground
const int PinSW = 3;   // Push Button Switch
const int PinDT = 4;   // Encoder DT Signal
const int TR_Pin = 8;  // Add T/R switching capability to synthesizer board

/*
    Define Bands of Operation, Initial Frequencies and parts per billion frequency correction
    NOTE: For 60 meters, there are only 5 frequencies allowable for the CW carrier
      5332, 5348, 5358.5, 5373 and 5405 kHz
      To start up for the first time on the nth channel, set Freq1 = n and Band = 60
      The Rotary Encoder will now be used to cycle through the 5 channels
      The push button alternates the encoder mode between frequency adjust and tuning step size adjust
*/
// Define Frequency Bands - The first element of each array is index number 0
char* BandString[] = { "2200", "630", "160", "80", "60", "40", "30", "20", "17", "15", "12", "10", "6", "2", "1.25" };

// Default QRP Calling Frequencies - 60m default is Channel 1
long FreqArray[15] = { 137500, 472500, 1810000, 3560000, 0, 7030000, 10106000, 14060000, 18096000, 21060000, 24906000, 28060000, 50060000, 144060000, 222060000 };

// Define Frequency Step Sizes in Hz - 0=Band Change
long StepArray[8] = { 0, 1000000, 100000, 10000, 1000, 100, 10, 1};  // Step #0 is band change
char* StepString[] = { "Band", "1 MHz", "100 kHz", "10 kHz", "1 kHz", "100 Hz", "10 Hz", "1 Hz"};

// Define 60m (Band 3) Channel Frequencies
long ChanArray[5] = { 5332000, 5348000, 5358500, 5373000, 5405000 };

int Band = 5;                  // Initializes to Band 5, that is 40m on startup
long Freq1 = FreqArray[Band];  // Initializes to QRP Calling frequency/Channel on the startup Band
byte EncMode = 0;              // Initializes Encoder mode. Adjust frequency = 0, Adjust tuning step = 1
//
// For example, to use Tuna Tin S on 60 meter band: Set Band = 4
// This will start the unit on the 60m Calling Channel 5332 kHz
//
int sa = 4;  // Initialize tuning step size to 1 kHz
long deltaFreq = StepArray[sa];

const long ppb = 0;  // Substitute this zero with your parts per billion Si5351 frequency reference error here
// Use the TunaTinS_Calibration Utility v.2.1.0 to determine the value of ppb or else use the manual calculation described below.
// 1. Change the above line to => const long Calibrate_Offset = 0;
// 2. Set the Tuna Tin S Dial Frequency to 10000.000 kHz and run the sketch.
// 3. Measure the Si5351's True Frequency output in Hz with an accurate reference frequency counter or or compare with a receiver tuned to 10 MHz WWV frequency standard.
// 4. Calculate the value ppb = (True Freq - 10000000) x 100 and edit the sketch and replace the zero value of ppb.

long oldFreq = 0;
int TR_State = 0;

struct myStoredData {
  long freq;
  int Band;
  long deltaFreq;
} D1;

const int eeAddress = 0;

unsigned long delayStart = 0;
unsigned long DELAY_TIME = 10000;  // 10 second delay before EEPROM is updated

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4);

void isr() {  // Interrupt Service Routine runs if Rotary Encoder CLK pin falls
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {

    if (!digitalRead(PinDT)) {  // If encoder rotated CCW

      if (EncMode == 1) {          // If Encoder in tuning step mode
        if (Band == 4) {           // If Band is 60m
          deltaFreq = !deltaFreq;  // toggle deltaFreq between 0 and 1
        } else {                   // If Band is not 60m increment tuning step, cycle through values
          sa -= 1;
          sa = (sa + 8) % 8;
          deltaFreq = StepArray[sa];
        }
      }

      else {                   // If Encoder in frequency step mode
        if (deltaFreq == 0) {  // If deltaFreq is zero, save Freq1 and decrement Band, cycle through values
          FreqArray[Band] = Freq1;
          Band -= 1;
          Band = (Band + 15) % 15;
          Freq1 = FreqArray[Band];  // Load saved Freq1 for new band
        } else {                    // If deltaFreq is not zero
          if (Band == 4) {          // If Band is 60m decrement Channel, cycle through values
            Freq1 -= 1;
            Freq1 = (Freq1 + 5) % 5;
          } else {
            Freq1 -= deltaFreq;  // If Band is not 60m decrement Frequency but not below 4 kHz
            if (Freq1 < 4000) Freq1 = 4000;
          }
        }
      }
    }

    else {  // If encoder rotated CW

      if (EncMode == 1) {          // If Encoder in tuning step mode
        if (Band == 4) {           // If Band is 60m
          deltaFreq = !deltaFreq;  // toggle deltaFreq between 0 and 1
        } else {                   // If Band is not 60m decrement tuning step, cycle through values
          sa += 1;
          sa = sa % 8;
          deltaFreq = StepArray[sa];
        }
      }

      else {                   // If Encoder in frequency step mode
        if (deltaFreq == 0) {  // If deltaFreq is zero, save Freq1, increment Band, cycle through values
          FreqArray[Band] = Freq1;
          Band += 1;
          Band = Band % 15;
          Freq1 = FreqArray[Band];  // Load saved Freq1 for new band
        } else {                    // If deltaFreq is not zero
          if (Band == 4) {          // If Band is 60m increment Channel, cycle through values
            Freq1 += 1;
            Freq1 = Freq1 % 5;
          } else {
            Freq1 += deltaFreq;  // If Band is not 60m increment Frequency, but not above 225 MHz
            if (Freq1 > 225000000) Freq1 = 225000000;
          }
        }
      }
    }
  }
  lastInterruptTime = interruptTime;
}

void Splash()  // Splash screen displayed at power on/reset
{
  const char* text1 = "Tuna Tin S QRP";
  const char* text2 = "Bob Fontana AK3Y &";
  const char* text3 = "Carol Milazzo KP4MD";
  const char* text4 = "v2.1.0 31Dec2023";

  display.clearDisplay();  // Clears the graphics display buffer.
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(display.width() / 2 - strlen(text1) * 6 / 2, 0);  // Each character is approximately 6 pixels wide in default type font
  display.println(text1);
  display.setCursor(display.width() / 2 - strlen(text2) * 6 / 2, 8);
  display.println(text2);
  display.setCursor(display.width() / 2 - strlen(text3) * 6 / 2, 16);
  display.println(text3);
  display.setCursor(display.width() / 2 - strlen(text4) * 6 / 2, 24);
  display.println(text4);

  display.display();  // Need this command to refresh screen
}

void SetFreq(int Band, long Freq) {  // Use SI5351 Fractional Mode to set frequency at Port output
  // Port 0 uses PLL_A, Ports 1 and 2 use PLL_B
  // Freq in Hertz - 1 Hertz minimum resolution
  // For 60 meters, Freq is the Channel Number; e.g., Freq = 1, 2, ... 5

  if (Band == 4) {  // If Band is 60m retrieve Channel Frequency
    Freq = ChanArray[Freq1];
  }

  //Set CLK1
  si5351.set_freq(Freq * 100ULL, SI5351_CLK1);

  // Query a status update and wait a bit to let the Si5351 populate the
  // status flags correctly.
  si5351.update_status();
  delay(500);
}
/*                                // Set output Port 2 to 1/2 Port 1 frequency for companion receiver
  clockgen.setupMultisynth(2, SI5351_PLL_B, k, l, m);
  clockgen.setupRdiv(2, SI5351_R_DIV_2);
*/

void Display_Freq(unsigned long freq) {  // Display Band and Frequency
  unsigned long f_int;
  unsigned int f_frac;
  char buf[50];

  display.clearDisplay();  // Clears the graphics display buffer.

  display.setTextSize(1);  // First Display Line - Band
  String Bandstring = BandString[Band];
  Bandstring = Bandstring + "m Freq (kHz)";

  display.setCursor(display.width() / 2 - Bandstring.length() * 6 / 2, 0);  // Each character is approximately 6 pixels wide in default type font
  display.print(Bandstring);

  if (EncMode == 0) {  // Second Display Line - Frequency
    display.setCursor(0, 11);
    display.print(".");  // Display dot if Encoder in Frequency mode
  }
  display.setTextSize(2);

  if (Band == 4) freq = ChanArray[Freq1];  // If Band is 60m display the frequency instead of the channel number
  f_int = freq / 1000;
  f_frac = (freq - f_int * 1000);

  if (f_int < 1000) {
    display.setCursor(display.width() / 2 - 20, 10);
  } else {
    if (f_int < 10000) {
      display.setCursor(display.width() / 2 - 32, 10);
    } else {
      if (f_int < 100000) {
        display.setCursor(display.width() / 2 - 44, 10);
      } else {
        display.setCursor(display.width() / 2 - 56, 10);
      }
    }
  }
  snprintf(buf, sizeof(buf), "%lu.%003d", f_int, f_frac);
  display.print(buf);

  display.setTextSize(1);  // Third Display Line - Tuning Step

  if (EncMode == 1) {  // Display dot if Encoder in Step mode
    display.setCursor(0, 22);
    display.print(".");
  }

  if (Band == 4 && deltaFreq != 0) {  // If Band is 60m and Encoder Mode is Frequency Display Channel Number
    display.setCursor(display.width() / 2 - strlen("Channel #") * 6 / 2, 25);
    display.print("Channel ");
    display.print(Freq1 + 1);
  } else {
    display.setCursor(display.width() / 2 - (strlen("Step ") + strlen(StepString[sa])) * 6 / 2, 25);
    display.print("Step ");
    display.print(StepString[sa]);
  }

  display.display();
}

void Check_TR_Status() {
  TR_State = digitalRead(TR_Pin);
  if (TR_State == LOW) {                   //Transmit
    si5351.output_enable(SI5351_CLK1, 1);  //Enable the clock
  } else {                                 // Receive
    si5351.output_enable(SI5351_CLK1, 0);  //Disable the clock
  }
}

void loadSettings() {
  int tempBand;
  long tempFreq;
  if (EEPROM.read(E_INIT) == 'T') {  // EEProm has previously been loaded
    EEPROM.get(eeAddress, D1);
    if ((tempBand = D1.Band) == Band) {  // User has not changed operating band in header from previous load
      Freq1 = D1.freq;                   // OK to load EEProm memory information
      Band = D1.Band;
      deltaFreq = D1.deltaFreq;
    }
  } else {  //  Initial EEProm load into new chip
    D1 = { Freq1, Band, 1000 };
    EEPROM.put(eeAddress, D1);
    EEPROM.write(E_INIT, 'T');
  }
}

void setup(void) {
  Serial.begin(19200);  // Initialize Serial Port

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize OLED Display

  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, ppb);  // Initialize SI5351 Clock Generator
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_8MA);
  si5351.output_enable(SI5351_CLK0, 0);                  //1  - Enable / 0 - Disable CLK
  si5351.output_enable(SI5351_CLK1, 1);
  si5351.output_enable(SI5351_CLK2, 0);

  pinMode(PinSW, INPUT);
  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);
  pinMode(TR_Pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), isr, FALLING);  // Pins 2 and 3 are interrupts on Arduino Nano

  Splash();  // Display Splash screen
  delay(3000);
  display.clearDisplay();
  display.display();

  loadSettings();
}

void loop() {

  Check_TR_Status();
  if (!(digitalRead(PinSW))) {  // Check if Pushbutton is pressed toggle Encoder Switch mode
    EncMode = !EncMode;
  }
  while (!digitalRead(PinSW))
    ;
  delay(10);  // Debounce switch

  Display_Freq(Freq1);

  if (Freq1 != oldFreq) {  // Display freq change to serial monitor
    if (Band == 4) {
      Serial.println(ChanArray[Freq1]);
    } else {
      Serial.println(Freq1);
    }

    // Read the Status Register and print it
    si5351.update_status();
    Serial.print("SYS_INIT: ");
    Serial.print(si5351.dev_status.SYS_INIT);
    Serial.print("  LOL_A: ");
    Serial.print(si5351.dev_status.LOL_A);
    Serial.print("  LOL_B: ");
    Serial.print(si5351.dev_status.LOL_B);
    Serial.print("  LOS: ");
    Serial.print(si5351.dev_status.LOS);
    Serial.print("  REVID: ");
    Serial.println(si5351.dev_status.REVID);

    Display_Freq(Freq1);
    SetFreq(Band, Freq1);
    oldFreq = Freq1;
    delayStart = millis();
  }

  if ((millis() - delayStart) >= DELAY_TIME) {
    D1 = { Freq1, Band, deltaFreq };
    EEPROM.put(eeAddress, D1);  // EEPROM.put will not write values to EEPROM if they did not change
  }
}
