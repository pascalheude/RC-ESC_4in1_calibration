#include <Arduino.h>
#include <Servo.h>
#include "standard.h"
#include "drone.h"
#include "mapping.h"
#include "esc.h"
#include "led.h"
#include "mathf.h"
#include "mode.h"
#include "not.h"
#include "nrf24l01.h"
#include "RC-ECS_4in1_calibration.h"
#include "spy.h"

BOOLEAN F_one_reception_ok;
BOOLEAN F_no_reception;
BOOLEAN l_toggle_switch;
BOOLEAN r_toggle_switch;
BOOLEAN lo_push_button;
BOOLEAN li_push_button;
BOOLEAN ro_push_button;

static UNS32 receive_time;

#define MIN_RADIO_COMMAND (REAL32)0.0f
#define MAX_RADIO_COMMAND (REAL32)255.0f

void initializeDrone(void)
{
    F_one_reception_ok = false;
    F_no_reception = false;
    l_toggle_switch = false;
    r_toggle_switch = false;
    lo_push_button = false;
    li_push_button = false;
    ro_push_button = false;
    receive_time = 0;
}

void manageDrone(void)
{
    if (readRadio())
    {
        F_one_reception_ok = true;
        F_no_reception = false;
        receive_time = pit_number;
        l_toggle_switch = NOT(radio_data.l_toggle_switch);
        r_toggle_switch = NOT(radio_data.r_toggle_switch);
        lo_push_button = NOT(radio_data.lo_push_button);
        li_push_button = NOT(radio_data.li_push_button);
        ro_push_button = NOT(radio_data.ro_push_button);
    }
    else
    {
        if ((pit_number - receive_time) > (5000/PIT_PERIOD))
        {
            F_no_reception = true;
        }
        else
        {
        }
    }
    manageMode();
}