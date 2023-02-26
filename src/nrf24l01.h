typedef struct
{
    UNS32 tx_time;
    UNS8 from_radio_id;
    UNS8 failed_tx_count;
    UNS8 l_joystick_button;
    UNS8 l_x_joystick;
    UNS8 l_y_joystick;
    UNS8 r_joystick_button;
    UNS8 r_x_joystick;
    UNS8 r_y_joystick;
    UNS8 l_potentiometer;
    UNS8 r_potentiometer;
    UNS8 l_toggle_switch;
    UNS8 r_toggle_switch;
    UNS8 lo_push_button;
    UNS8 li_push_button;
    UNS8 ri_push_button;
    UNS8 ro_push_button;
} T_radio_packet;

extern T_radio_packet radio_data;

extern void initializeRadio(void);
extern BOOLEAN readRadio(void);