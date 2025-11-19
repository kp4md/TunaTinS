# TunaTinS QRP Transmitter
Arduino Nano sketch for Bob Fontana AK3Y's Tuna Tin S QRPp CW Transmitter 

This code is used with the Tuna Tin S, a low power amateur radio radiotelegraphy (CW) transmitter designed by Bob Fontana, AK3Y. 
The full circuit description and construction details are published in December 2020 QST Journal with an update in the September 2022 issue https://www.arrl.org/qst . 

The transmitter is comprised of two circuits: 
1. an Arduino Nano microcontroller driving an Si5351 frequency synthesizer module with a 128x64 pixel OLED digital frequency readout and
2. an IRF510 MOSFET RF power amplifier with a low pass filter.  The typical power output is 0.5 watts.

The Tuna Tin S transmitter requires a distinct bandpass filter for operation on each frequency band. 
A filter design tool is available at [Class E Design Spreadsheet](https://4sqrp.com/resource/classie/classie.html) or [this spreadsheet](https://docs.google.com/spreadsheets/d/1SyXx_rcVZvYHGLNGJtvW9o1FNvxYuF-3/).

## Original Arduino Nano Sketch
The original Arduino sketch code set up the Si5351 module for operation within one frequency band. 
An encoder switch allowed selection of frequency and tuning steps within that one band. 
The author's original Arduino sketch codes are accessible to ARRL members in the https://www.arrl.org/qst-in-depth archive. 
This updated Arduino sketch is based on Fontana's original code with the enhancements enumerated below.

The Arduino IDE originally required installation of the Adafruit SSD1306 (Adafruit_SSD1306.h) and the Adafruit Si5351 (Adafruit_SI5351.h) Libraries to compile this sketch. 
replaced the Adafruit Si5351 library.
The Upload Procedure for the libraries and sketches is posted at https://groups.io/g/DIYRadio/message/186

# [Download TunaTinS_v2.2 Arduino Nano Sketch](https://github.com/kp4md/TunaTinS/releases/download/v2.2/TunaTinS_v2.2.ino)

# [Click here for the Full User Instruction Manual in the Wiki](https://github.com/kp4md/TunaTinS/wiki)

## Required Libraries 
Use the Arduino IDE Library Manager to install the [Adafruit SSD_1306 library](https://github.com/adafruit/Adafruit_SSD1306) and NT7S's [Etherkit Si5351 library](https://github.com/etherkit/Si5351Arduino) before compiling this sketch.

## Features:

The Tuna Tin S v2.2 is a compact, Arduino-based QRP transmitter with:
1. Si5351 frequency synthesizer
2. OLED display (128×32, Adafruit SSD1306)
3. Rotary encoder with push-button
4. TR switch input for transmit mode
5. In addition to controlling the Tuna Tin S transmitter frequencies, the software allows the Si5351 to serve as a low-level signal source (VFO) continuously variable from 4 kHz through 225 MHz.
6. The software incorporates a frequency calibration mode that can achieve 0.1 ppm accuracy over that frequency range. The calibration factor is stored in non-volatile EEPROM memory.
7. Each of the 15 amateur radio frequency bands defaults to its QRP CW calling frequency upon power up.
8. Each frequency band retains its last used frequency in memory while powered up.
9. The user may define and store a default startup frequency in non-volatile memory.
10. The frequency calibration factor (ppb) and the user-defined default startup frequency are stored in EEPROM memory that retains the data even when the power is off.

## Notes

1. NT7S’s Etherkit Si5351 library replaces the Adafruit Si5351 library.  This Etherkit Si5351 library automates the PLL setup and offers continuous RF spectrum coverage from 4 kHz through 225 MHz.
2. The Si5351 synthesizer covers all 15 amateur radio frequency bands with presets for QRP calling frequencies from 137 kHz (2200m) through 222 MHz (1.25m).  
3. The encoder serves multiple functions.  Rotation adjusts a frequency digit or step size.  A quick tap alternate the encoder mode between digit select and digit adjust.  A medium press (0.5-2s) toggles between normal operating mode and calibration mode.  A long press (>2s) saves the displayed frequency as a user-defined startup frequency (in normal mode) or saves the displayed ppb crystal correction factor (in calibration node) into non-volatile EEPROM.
4. The tuning steps are now: Band change, 1 MHz, 100 kHz, 10 kHz, 1 kHz, 100 Hz, 10 Hz and 1 Hz, in either direction.  The 60m band tuning steps through the USA's five discrete 5 MHz channels.
5. The integrated frequency calibration function permits setting the crystal correction factor to 0.1 ppm and saves it into non-volatile EEPROM.

# v2.2 Normal Frequency Digit Adjust Encoder Mode Display
![IMG_6380](https://github.com/user-attachments/assets/f7a71ea2-53a9-4267-b229-0b39b1cc8147)

# v2.2 60m Channel Mode Display
![IMG_6381](https://github.com/user-attachments/assets/42a05b56-4e52-48e6-a4d4-1823ebae2ba6)

# v2.2 Calibration Mode Display
![IMG_6385](https://github.com/user-attachments/assets/850a116f-f182-494f-a300-6aad101853b3)

## Video Demonstration of Arduino Nano Sketch v2.0.0 (an older version)
[![Tuna Tin S Arduino Sketch v2.0.0 Demo](https://img.youtube.com/vi/qvt9tF0a9zI/0.jpg)](https://www.youtube.com/watch?v=qvt9tF0a9zI)

Please contact our team via https://groups.io/g/DIYRadio or mailto://contact@n6na.org if you wish to:
1. Report a bug
2. Discuss the current state of the code
3. Submit a fix
4. Propose new features

Thank you!
