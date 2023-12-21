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
The Arduino Nano Upload Procedure is posted at https://groups.io/g/DIYRadio/message/186

The 01 January 2022 sketch release v2.0.0 is posted at https://github.com/kp4md/TunaTinS/releases/tag/v2.0.0-beta and includes the following updates since the 15APR21 release:

1. 630 meter and 6 meter bands have been added. The Si5351 synthesizer now covers all 12 amateur frequency bands from 472 kHz through 50 MHz.
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
