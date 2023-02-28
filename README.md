# RC-ESC_4in1_calibration
Calibration of ESC 4 in 1 LANNRC BLHELI_S 45A 60A

![](images/LANNRC%20BLHELI_S%2045A%2060A%20ESC%204in1.jpg)

ESP32 connected to flight controller [SkyStars-rc F405HD2](https://skystars-rc.com/product/skystars-f405hd2-30-5x30-5mm-betaflight-inav-flight-controller/)
by SBUS

The flight controller is then connected to the ESC 4 in 1 by a 9 pins cable

# Procedure to calibrate the ESC
1. Connect the ESP32, the NRF24L01, ESC and flight controller
2. Switch on the receiver (ESP32)
The motors are then commanded to maximum speed
3. Switch on the transmitter (see [](https://github.com/pascalheude/RC-transmitter))
4. Switch on the ESC
Wait for the end of the little music of the motor
5. Put the left toggle of transmitter to up position
Wait for the end of the little music of the motor
6. Put the left toggle of tramsitter to down position

# Finding the minimum and maximum throttle position
## Mini
1. Push the left outboard button of transitter to increase the motor command by 2
Until the motor rotates without stopping
2. Push the right outboard button to find the maximum
## Maxi
1. Push the left outboard button of transitter to decrease the motor command by 2
Until the motor sound changes
2. Push the right outboard button to find the minimum and maximum throttle for the next motor

# Test
Push the left outboard button of transmitter to decrease the four motor command by 50
Push the right outboard button of transmitter to increase the four motor command by 50
Push the left inboard button of transmitter to reset  the four motor command to 1000
