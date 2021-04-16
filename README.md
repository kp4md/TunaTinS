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

Subsequent revisions are posted here.
