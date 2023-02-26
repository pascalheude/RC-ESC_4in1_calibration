#include <Arduino.h>
#include "standard.h"
#include "led.h"
#include "mapping.h"
#include "RC-ECS_4in1_calibration.h"

static UNS8 led1_state;
static UNS8 led2_state;
static UNS32 led1_time;
static UNS32 led2_time;

void initializeLed(void)
{
    led1_state = LOW;
    led2_state = LOW;
    led1_time = 0;
    led2_time = 0;
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
}

void blinkLed2(UNS32 time_out)
{
    if ((pit_number - led2_time) > (time_out/PIT_PERIOD))
    {
        led2_time = pit_number;
        if (led2_state == LOW)
        {
            led2_state = HIGH;
        }
        else
        {
            led2_state = LOW;
        }
        digitalWrite(LED2, led2_state);
    }
    else
    {
    }    
}

void switchOffLed1(void)
{
    led1_time = 0;
    led1_state = LOW;
    digitalWrite(LED1, LOW);
}

void switchOnLed1(void)
{
    led1_time = 0;
    led1_state = HIGH;
    digitalWrite(LED1, HIGH);
}