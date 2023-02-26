#include <Arduino.h>
#include <NRFLite.h>
#include "standard.h"
#include "nrf24l01.h"
#include "led.h"
#include "mapping.h"
#include "RC-ECS_4in1_calibration.h"
#include "spy.h"

#define RADIO_ID 0 // Reveiver radio ID. The transmitter will send to this ID

T_radio_packet radio_data;

static NRFLite radio(Serial);

void initializeRadio(void)
{
    BOOLEAN F_init_radio_ok;

    // By default, 'init' configures the radio to use a 2MBPS bitrate on channel 100 (channels 0-125 are valid).
    // Both the RX and TX radios must have the same bitrate and channel to communicate with each other.
    // You can run the 'ChannelScanner' example to help select the best channel for your environment.
    // You can assign a different bitrate and channel as shown below.
    //   radio.init(RADIO_ID, PINradio_CE, PINradio_CSN, NRFLite::BITRATE2MBPS, 100) // THE DEFAULT
    //   radio.init(RADIO_ID, PINradio_CE, PINradio_CSN, NRFLite::BITRATE1MBPS, 75)
    //   radio.init(RADIO_ID, PINradio_CE, PINradio_CSN, NRFLite::BITRATE250KBPS, 0)
    F_init_radio_ok = radio.init(RADIO_ID, CE, CSN);
    if (F_init_radio_ok == false)
    {
#ifdef SPY
        Serial.println("Cannot communicate with radio");
#endif // SPY
        switchOnLed1();
        while(true); // Wait here forever.
    }
    else
    {
    }
}

BOOLEAN readRadio(void)
{
    if (radio.hasData())
    {
        radio.readData(&radio_data);
#ifdef SPY
        String msg;
        msg += radio_data.tx_time;
        msg += " ms: radio #";
        msg += radio_data.from_radio_id;
        msg += ", ";
        msg += radio_data.failed_tx_count;
        msg += ", ";
        msg += radio_data.l_joystick_button;
        msg += ", ";
        msg += radio_data.l_x_joystick;
        msg += ", ";
        msg += radio_data.l_y_joystick;
        msg += ", ";
        msg += radio_data.r_joystick_button;
        msg += ", ";
        msg += radio_data.r_x_joystick;
        msg += ", ";
        msg += radio_data.r_y_joystick;
        msg += ", ";
        msg += radio_data.l_potentiometer;
        msg += ", ";
        msg += radio_data.r_potentiometer;
        msg += ", ";
        msg += radio_data.l_toggle_switch;
        msg += ", ";
        msg += radio_data.r_toggle_switch;
        msg += ", ";
        msg += radio_data.lo_push_button;
        msg += ", ";
        msg += radio_data.li_push_button;
        msg += ", ";
        msg += radio_data.ri_push_button;
        msg += ", ";
        msg += radio_data.ro_push_button;
        Serial.println(msg);
#endif // SPY
        return(true);
    }
    else
    {
        return(false);
    }
}