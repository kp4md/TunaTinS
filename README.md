# TunaTinS
Arduino sketch for Bob Fontana AK3Y's Tuna Tin S QRPp CW Transmitter 

This code is used with the Tuna Tin S, a low power amateur radio radiotelegraphy (CW) transmitter designed by Bob Fontana, AK3Y. 
The full circuit description and construction details are published in December 2020 QST Journal http://www.arrl.org/qst. 
The original Arduino sketch code is accessible to ARRL members in the http://www.arrl.org/qst-in-depth archive. 

The transmitter is comprised of two circuits: 
1. an Arduino Nano microcontroller driving an Si5351 frequency synthesizer module with an OLED digital frequency readout and
2. an IRF510 RF amplifier with a low pass filter.  The typical power output is 0.5 watts. 

The original Arduino sketch code set up the Si5351 module for operation within one frequency band. 
An encoder switch allowed selection of frequency and tuning steps within that one band. 

The Arduino IDE requires installation of the Adafruit SSD1306 (Adafruit_SSD1306.h) and the Adafruit Si5351 (Adafruit_SI5351.h) Libraries to compile this sketch. 
Starting with v2.1.0, NT7S's Etherkit Si5351 library (https://github.com/etherkit/Si5351Arduino) replaced the Adafruit Si5351 library.
The Arduino Nano Upload Procedure is posted at https://groups.io/g/DIYRadio/message/186

The 31 December 2023 TunaTinS_v2.1.0 Arduino sketch release includes the following changes since the v2.0.0 release:

1. NT7S’s Etherkit Si5351 library replaces the Adafruit Si5351 library.
2. The Etherkit Si5351 library automates the PLL setup and offers continuous RF spectrum coverage from 4 kHz through 225 MHz.
3. The Si5351 synthesizer now covers all 15 amateur radio frequency bands with presets for QRP calling frequencies from 137 kHz (2200m) through 222 MHz (1.25m).
4. Use the encoder control to adjust either the frequency or the tuning steps.  Depress the control to alternate its mode between digit select and digit adjust.
5. The tuning steps are now: Band change, 1 MHz, 100 kHz, 10 kHz, 1 kHz, 100 Hz, 10 Hz and 1 Hz, in either direction. 
6. You can now correct for Si5351 frequency reference crystal error to within 1 Hz at 10 MHz.  The companion TunaTinS_v2.1.0_Calibration sketch calculates and displays the parts per billion (ppb) crystal frequency correction factor (the third argument in the Etherkit Si5351 library’s si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0); command).

Procedure for Frequency Calibration with the TunaTinS_v2.1.0_Calibration Arduino sketch
1. Download and run the TunaTinS_v2.1.0_Calibration sketch.  
2. Tune a receiver to WWV on 10 MHz or to another 10 MHz frequency reference (or measure with a frequency counter).
3. Adjust the Tuna Tin S encoder to zero beat the Si5351 signal against the 10 MHz reference signal.
4. Read and record the ppb value from the first line of the OLED display. https://github.com/kp4md/TunaTinS/blob/main/IMG_5873.JPG
5. Open the TunaTinS_v2.1.0 sketch in the Arduino IDE.
6. Go to Line 59 and replace the 0 with your calculated ppb value.
const long ppb = 0;  // Substitute this zero with your parts per billion Si5351 frequency reference error here
7. Save the edited sketch and upload it to your Arduino.
8. It is normal to observe 2-3 Hz frequency drift over time due to temperature variation.

The 01 January 2022 sketch release v2.0.0 is posted at https://github.com/kp4md/TunaTinS/releases/tag/v2.0.0-beta and includes the following updates since the 15APR21 release:

1. 630 meter and 6 meter bands have been added. The Si5351 synthesizer now covers 12 amateur frequency bands from 472 kHz through 50 MHz.
2. 45 lines have been eliminated by moving the band constants into arrays and cleaning up the code. The compiled program size is now 23204 bytes.
3. New PLL setup constants improve frequency tracking on the 5 MHz, 10 MHz and 24 MHz bands.

The Tuna Tin S transmitter requires distinct low pass filters for operation on each frequency band.  
See http://shop.qrp-labs.com/LPF for some low pass filter options and http://shop.qrp-labs.com/kits/ULPF for a filter switching board kit.

Please contact our team via https://groups.io/g/DIYRadio or mailto://contact@n6na.org if you wish to:
1. Report a bug
2. Discuss the current state of the code
3. Submit a fix
4. Propose new features

Thank you!
