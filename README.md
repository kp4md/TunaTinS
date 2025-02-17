# TunaTinS
Arduino sketch for Bob Fontana AK3Y's Tuna Tin S QRPp CW Transmitter 

This code is used with the Tuna Tin S, a low power amateur radio radiotelegraphy (CW) transmitter designed by Bob Fontana, AK3Y. 
The full circuit description and construction details are published in December 2020 QST Journal with an update in the September 2022 issue https://www.arrl.org/qst . 

The transmitter is comprised of two circuits: 
1. an Arduino Nano microcontroller driving an Si5351 frequency synthesizer module with a 128x64 pixel OLED digital frequency readout and
2. an IRF510 MOSFET RF power amplifier with a low pass filter.  The typical power output is 0.5 watts.

The Tuna Tin S transmitter would require distinct low pass filters for operation on each of multiple frequency bands.  
See https://shop.qrp-labs.com/LPF for some low pass filter options and https://shop.qrp-labs.com/kits/ULPF for a filter switching board kit.

## Original Arduino Sketch
The original Arduino sketch code set up the Si5351 module for operation within one frequency band. 
An encoder switch allowed selection of frequency and tuning steps within that one band. 
The author's original Arduino sketch codes are accessible to ARRL members in the https://www.arrl.org/qst-in-depth archive. 
This updated Arduino sketch is based on Fontana's original code with the enhancements enumerated below.

The Arduino IDE originally required installation of the Adafruit SSD1306 (Adafruit_SSD1306.h) and the Adafruit Si5351 (Adafruit_SI5351.h) Libraries to compile this sketch. 
Starting with v2.1.0, NT7S's Etherkit Si5351 library (https://github.com/etherkit/Si5351Arduino) replaced the Adafruit Si5351 library.
The Upload Procedure for the libraries and sketches is posted at https://groups.io/g/DIYRadio/message/186

## v2.1.0 Arduino Sketch
The 31 December 2023 TunaTinS_v2.1.0 Arduino sketch release https://github.com/kp4md/TunaTinS/releases/tag/v2.1.0 includes the following changes since the v2.0.0 release:

1. NT7S’s Etherkit Si5351 library replaces the Adafruit Si5351 library.  This Etherkit Si5351 library automates the PLL setup and offers continuous RF spectrum coverage from 4 kHz through 225 MHz.
2. The Si5351 synthesizer now covers all 15 amateur radio frequency bands with presets for QRP calling frequencies from 137 kHz (2200m) through 222 MHz (1.25m).  
3. Use the encoder control to adjust either the frequency or the tuning steps.  Depress the encoder control to alternate its mode between digit select and digit adjust.  
4. The tuning steps are now: Band change, 1 MHz, 100 kHz, 10 kHz, 1 kHz, 100 Hz, 10 Hz and 1 Hz, in either direction.  The 60m band tuning steps through the USA's five discrete 5 MHz channels.
5. The new companion TunaTinS_v2.1.0_Calibration sketch calculates and displays the parts per billion (ppb) crystal frequency correction factor (the third argument in the Etherkit Si5351 library’s si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0); command). 
Plugging this factor into the main sketch will correct for Si5351 frequency reference crystal error to within 1 Hz at 10 MHz.

## v2.1.0 Frequency Calibration Utility
Procedure for Frequency Calibration with the TunaTinS_v2.1.0_Calibration Arduino sketch https://github.com/kp4md/TunaTinS/blob/main/TunaTinS_v2.1.0_Calibration.ino
1. Download and run the TunaTinS_v2.1.0_Calibration sketch.  
2. Tune a receiver to WWV on 10 MHz or to another accurate 10 MHz frequency reference (or measure with a frequency counter).
3. Adjust the Tuna Tin S encoder to zero beat the Si5351 signal against the 10 MHz reference signal.
4. Read and record the ppb value from the first line of the OLED display. https://github.com/kp4md/TunaTinS/blob/main/IMG_5873.JPG
5. Open the TunaTinS_v2.1.0 sketch in the Arduino IDE.
6. Go to Line 59 and replace the 0 with your calculated ppb value.
const long ppb = 0;  // Substitute this zero with your parts per billion Si5351 frequency reference error here
7. Save the edited sketch and upload it to your Arduino.
8. It is normal to observe 2-3 Hz frequency drift over time due to ambient temperature variation.

## v2.0.0 Arduino Sketch
The previous 01 January 2022 sketch release v2.0.0 added the following updates since the 15APR21 release:

1. 630 meter and 6 meter bands have been added. The Si5351 synthesizer now covers 12 amateur frequency bands from 472 kHz through 50 MHz.
2. 45 lines have been eliminated by moving the band constants into arrays and cleaning up the code. The compiled program size is now 23204 bytes.
3. New PLL setup constants improve frequency tracking on the 5 MHz, 10 MHz and 24 MHz bands.

## Video Demonstration of Arduino Sketch 
https://www.youtube.com/watch?v=qvt9tF0a9zI 

Please contact our team via https://groups.io/g/DIYRadio or mailto://contact@n6na.org if you wish to:
1. Report a bug
2. Discuss the current state of the code
3. Submit a fix
4. Propose new features

Thank you!

# v2.1.0 Tuna Tin S Arduino Sketch Display
![IMG_5872](https://github.com/user-attachments/assets/26829318-b29b-4858-a3a8-28265da0c341) 
# v2.1.0 Frequency Calibration Utility Display
![IMG_5873](https://github.com/user-attachments/assets/c5b0c5f5-fc86-4ed3-92d8-aa68367a0e22)


