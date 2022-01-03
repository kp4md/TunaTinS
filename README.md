# TunaTinS
Arduino code for Bob Fontana AK3Y's Tuna Tin S QRPp CW Transmitter

This code is used with the Tuna Tin S, a low power amateur radio radiotelegraphy (CW) transmitter designed by Bob Fontana AK3Y.
The full circuit description and construction details are published in December 2020 QST Journal http://www.arrl.org/qst.
The original code is accessible to ARRL members in the http://www.arrl.org/qst-in-depth archive.

The transmitter is comprised of two circuits: 
an Arduino Nano microcontroller driving an Si5351 frequency synthesizer module with an OLED digital frequency readout and
an IRF510 RF amplifier with a low pass filter.  The typical power output is 0.5 watts.
A low pass filter determines the usable operating frequency range, one of the several high frequency bands from 1.8 through 28 MHz.

The original code set up the Si5351 module on the default frequency.  
An encoder switch allowed selection of frequency and tuning steps within one band.

The 01 January 2022 sketch release v2.0.0 is posted at https://github.com/kp4md/TunaTinS/releases/tag/v2.0.0-beta
This release includes the following updates:

1. 630 meter and 6 meter bands have been added. The Si5351 synthesizer now covers all amateur frequency bands from 472 kHz through 50 MHz.
2. 45 lines have been eliminated from the 15APR2021 release by moving the band constants into arrays and cleaning up the code. The compiled program size is now 23204 bytes.
3. New PLL setup constants improve frequency tracking on the 5 MHz, 10 MHz and 24 MHz bands.

Please contact our team via https://groups.io/g/DIYRadio or mailto://contact@n6na.org if you wish to:
1. Report a bug
2. Discuss the current state of the code
3. Submit a fix
4. Propose new features

Thank you!
