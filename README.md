# TunaTinS QRP Transmitter
Arduino Nano sketch for Bob Fontana AK3Y's Tuna Tin S QRPp CW Transmitter 

This code is used with the Tuna Tin S, a low power amateur radio radiotelegraphy (CW) transmitter designed by Bob Fontana, AK3Y. 
The full circuit description and construction details are published in December 2020 QST Journal with an update in the September 2022 issue https://www.arrl.org/qst . 

The transmitter is comprised of two circuits: [See Hardware Diagrams](https://github.com/kp4md/TunaTinS/wiki/Hardware-Diagrams)
1. an Arduino Nano microcontroller, an EC11 (KY-040) rotary encoder with push-button, an Si5351 frequency synthesizer module, a 128x64 pixel OLED digital frequency readout; and,
2. an IRF510 MOSFET RF power amplifier with a 7 MHz Class E output filter network.  The typical power output is 0.5 watts.  A distinct output filter is required for operation on each frequency band. 
The NMØS Class E filter design tool is available at [Class E Design Spreadsheet](https://4sqrp.com/resource/classie/classie.html) or [this spreadsheet](https://docs.google.com/spreadsheets/d/1SyXx_rcVZvYHGLNGJtvW9o1FNvxYuF-3/).

## Original Arduino Nano Sketch
The original Arduino sketch code set up the Si5351 module for operation within one frequency band. 
An encoder switch allowed selection of frequency and tuning steps within that one band. 
The author's original Arduino sketch codes are accessible to ARRL members in the https://www.arrl.org/qst-in-depth archive for December 2020. 
This updated Arduino sketch is based on Fontana's original code with the enhancements enumerated below.

The original sketch required installation of the Adafruit SSD1306 (Adafruit_SSD1306.h) and the Adafruit Si5351 (Adafruit_SI5351.h) Libraries into the Arduino IDE for compilation. 

# [Download TunaTinS_v2.2 Arduino Nano Sketch](https://github.com/kp4md/TunaTinS/releases/download/v2.2/TunaTinS_v2.2.ino)

# [Click here for the Full User Instruction Manual in the Wiki](https://github.com/kp4md/TunaTinS/wiki)

## Required Libraries 
Use the Arduino IDE Library Manager to install the [Adafruit SSD_1306 library](https://github.com/adafruit/Adafruit_SSD1306) and NT7S's [Etherkit Si5351 library](https://github.com/etherkit/Si5351Arduino) before compiling this sketch. The Upload Procedure for the libraries and sketch is posted [in the Wiki](https://github.com/kp4md/TunaTinS/wiki/Arduino-Upload-Procedure)

## TunaTinS_v2.2 Update Features:

**TunaTinS v2.2 is a significantly innovative update for Arduino Nano VFO sketches.**

Combining frequency calibration into the main Arduino code and storing the correction factor into non-volatile memory greatly simplifies and reduces errors in setting the frequency accuracy of the Si5351. This innovation eliminates the past tedious process that required loading a separate calibration program, calculating the correction factor, and manually editing, recompiling and uploading the code.

This new Arduino Nano code itself calculates, stores and applies the 0.1 ppm crystal correction factor to the VFO program. The user accomplishes all this using the rotary encoder and a frequency counter or an accurate 10 MHz frequency reference like WWV and a receiver. This is a significant improvement over other published Arduino Si5351 VFO sketches.

Variations in the Si5351's quartz crystal tolerance can typically cause an output frequency error of up to 1 or 2 kHz. After calibration, your Si5351 VFO frequency will be accurate to within 0.1 ppm over its entire 4 kHz to 255 MHz range, e.g., within 1 Hz at 10 MHz.

This sketch controls the Tuna Tin S transmitter frequencies and also allows the Si5351 to serve as a low-level signal source (Variable Frequency Oscillator) of very good accuracy continuously variable from 4 kHz through 225 MHz. The code is deeply optimized for memory size and execution speed .

## Overview:

The Tuna Tin S v2.2 is a compact, Arduino-based QRP transmitter with:
1. Si5351 frequency synthesizer
2. OLED display (128×64, Adafruit SSD1306)
3. EC11 (KY-040) rotary encoder with push-button
4. TR switch input for transmit mode
5. In addition to controlling the Tuna Tin S transmitter frequencies, the software allows the Si5351 to serve as a low-level signal source (VFO) continuously variable from 4 kHz through 225 MHz.
6. The software incorporates a frequency calibration mode that can achieve 0.1 ppm accuracy over that frequency range. The calibration factor is stored in non-volatile EEPROM memory.
7. Each of the 15 amateur radio frequency bands defaults to its QRP CW calling frequency upon power up.
8. Each frequency band retains its last used frequency in memory while powered up.
9. The user may define and store a default startup frequency in non-volatile memory.
10. The frequency calibration factor (ppb) and the user-defined default startup frequency are stored in EEPROM memory that retains the data even when the power is off.

## Notes

1. NT7S’s Etherkit Si5351 library replaces the Adafruit Si5351 library in the original sketch.  This Etherkit Si5351 library automates the PLL setup and offers continuous RF spectrum coverage from 4 kHz through 225 MHz.
2. The Si5351 synthesizer covers all 15 amateur radio frequency bands with presets for QRP calling frequencies from 137 kHz (2200m) through 222 MHz (1.25m).  
3. The encoder serves multiple functions:  **Rotation** adjusts a frequency digit or step size.  **A quick tap (<0.5s)** alternates the encoder between digit select and digit adjust modes.  **A medium press (0.5-2s)** toggles between normal operating mode and calibration mode.  **A long press (>2s)** saves the displayed frequency as a user-defined startup frequency (in normal mode) or saves the displayed ppb crystal correction factor (in calibration node) into non-volatile EEPROM.
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
