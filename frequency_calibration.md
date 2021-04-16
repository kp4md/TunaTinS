This is the procedure to calibrate the Tuna Tin S to compensate for variation in the Si5351 reference crystal frequency.  Versions 12APR21 and later.

1. Open the sketch in the Arduino IDE, edit the initial value of Calibrate_Offset to zero, const long Calibrate_Offset = 0; upload the sketch to the Arduino and run it. 
2. Set the Tuna Tin S (TTS) band to 30m and the frequency to 10000.00 kHz.  (This procedure will work on any frequency but is very easy using the 10 MHz WWV frequency standard). 
3. Tune in the 10 MHz WWV and TTS Si5351 signals on a well calibrated receiver and measure the true frequency of the TTS signal compared to WWV. 
4. Calculate the integer value of Calibrate_Offset = (TTS Dial Freq/(True Freq - TTS Dial Freq)). In my example, the TTS signal frequency was identified at 10000.74 kHz; therefore, the value of Calibrate_Offset is 10000/0.74 or 13513. 
5. In the Arduino IDE overwrite the zero value of Calibrate_Offset with the new value (const long Calibrate_Offset = 13513; in this example), save the sketch and upload it to the Arduino.
