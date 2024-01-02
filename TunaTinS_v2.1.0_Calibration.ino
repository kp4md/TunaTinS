/*
   The Tuna Tin S v2.1.0 Calibration
   Original sketch by Bob Fontana, AK3Y
   Revisions since 09 April 2021 by Carol Milazzo, KP4MD
   See Changelog at https://github.com/kp4md/TunaTinS/

   Use this sketch to calibrate your Tuna Tin S Transmitter against an accurate 10 MHz frequency reference.
   Adjust the dial frequency to zero beat the Si5351 output against a 10.000000 MHz reference.
   Read the value of ppb from the OLED display and enter and save it into the Tuna Tin S v2.1.0 Arduino sketch
*/

#include <Wire.h>              // Include I2L Library for Communications with OLED Display and Synthesizer
#include <Adafruit_SSD1306.h>  // Include OLED Display Library
#include "si5351.h"            // Include Etherkit Si5351 Library

Si5351 si5351;

#define SCREEN_WIDTH 128       // OLED display width, in pixels
#define SCREEN_HEIGHT 32       // OLED display height, in pixels

// KY040 Rotary Encoder Module connections
const int PinCLK = 2;          // CLOCK (Interrupt line)  IMPORTANT! Decouple Pins 2 and 4 with 0.68uF caps to ground
const int PinSW = 3;           // Push Button Switch
const int PinDT = 4;           // Encoder DT Signal

// Define Frequency Step Sizes in Hz
long StepArray[4] = { 1000, 100, 10, 1 };
char* StepString[] = { "1 kHz", "100 Hz", "10 Hz", "1 Hz" };

long Freq1 = 10000000;         // Initializes to 10 MHz Reference Frequency
byte EncMode = 0;              // Initializes Encoder mode. Adjust frequency = 0, Adjust tuning step = 1

int sa = 1;  // Initialize tuning step size to 100 Hz
long deltaFreq = StepArray[sa];

long oldFreq = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, 4);

void isr() {  // Interrupt Service Routine runs if Rotary Encoder CLK pin falls
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime > 5) {

    if (!digitalRead(PinDT)) {      // If encoder rotated CCW

      if (EncMode == 1) {           // If Encoder is in tuning step mode
        sa -= 1; sa = (sa + 4) % 4; // increment tuning step, cycle between 1 kHz, 100 Hz, 10 Hz & 1 Hz steps
        deltaFreq = StepArray[sa];
      }
      else Freq1 -= deltaFreq;      // If Encoder is in frequency step mode decrement Frequency
    }

    else {  // If encoder rotated CW

      if (EncMode == 1) {           // If Encoder is in tuning step mode
        sa += 1; sa = sa % 4;       // decrement tuning step, cycle between 1 kHz, 100 Hz, 10 Hz and 1 Hz steps
        deltaFreq = StepArray[sa];
      }
      else Freq1 += deltaFreq;      // If Encoder is in frequency step mode increment Frequency
    }
  }
  lastInterruptTime = interruptTime;
}

void Splash()  // Splash screen displayed at power on/reset
{
  const char* text1 = "Tuna Tin S Calibrate";
  const char* text2 = "Use 10 MHz Reference";
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

void SetFreq(long Freq) {  // Set frequency at Port 1 output
  // Port 0 uses PLL_A, Ports 1 and 2 use PLL_B
  // Freq in Hertz - 1 Hertz minimum resolution

  //Set CLK1
  si5351.set_freq(Freq * 100ULL, SI5351_CLK1);

  // Query a status update and wait a bit to let the Si5351 populate the
  // status flags correctly.
  si5351.update_status();
  delay(500);
}

void Display_Freq(unsigned long freq) {  // Display Frequency
  unsigned long f_int;
  unsigned int f_frac;
  char buf[50];

  display.clearDisplay();                // Clears the graphics display buffer.

  display.setTextSize(1);                // First Display Line - ppb Correction Factor

  display.setCursor(36, 0);
  long ppb = (10000000 - freq) * 100L;
  display.print(ppb); display.print(" ppb");

  if (EncMode == 0) {                    // Second Display Line - Frequency
    display.setCursor(0, 11);
    display.print(".");                  // Display dot if Encoder in Frequency mode
  }
  display.setTextSize(2);

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

  display.setTextSize(1);                // Third Display Line - Tuning Step

  if (EncMode == 1) {                    // Display dot if Encoder in Step mode
    display.setCursor(0, 22);
    display.print(".");
  }

  display.setCursor(display.width() / 2 - (strlen("Step ") + strlen(StepString[sa])) * 6 / 2, 25);
  display.print("Step ");
  display.print(StepString[sa]);

  display.display();
}

void setup(void) {
  Serial.begin(19200);                        // Initialize Serial Port

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize OLED Display

  si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0); // Initialize SI5351 Clock Generator, Set the third argument (Correction factor) to zero ppb
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_8MA);
  si5351.output_enable(SI5351_CLK0, 0);       // 1  - Enable / 0 - Disable CLK
  si5351.output_enable(SI5351_CLK1, 1);       // Enable only Clock 1
  si5351.output_enable(SI5351_CLK2, 0);

  pinMode(PinSW, INPUT);
  pinMode(PinCLK, INPUT);
  pinMode(PinDT, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), isr, FALLING);  // Pins 2 and 3 are interrupts on Arduino Nano

  Splash();  // Display Splash screen
  delay(3000);
  display.clearDisplay();
  display.display();
}

void loop() {

  if (!(digitalRead(PinSW))) {                // Check if Pushbutton is pressed toggle Encoder Switch mode
    EncMode = !EncMode;
  }
  while (!digitalRead(PinSW))
    ;
  delay(10);                                  // Debounce switch

  Display_Freq(Freq1);

  if (Freq1 != oldFreq) {                     // Display freq change to serial monitor

    Serial.println(Freq1);

    si5351.update_status();                   // Read the Status Register and print it
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
    SetFreq(Freq1);
    oldFreq = Freq1;
  }
}
