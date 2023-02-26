#include <Arduino.h>
#include <Servo.h>
#include "standard.h"
#include "esc.h"
#include "mapping.h"
#include "mathf.h"
#include "RC-ECS_4in1_calibration.h"

UNS16 lf_esc_value;
UNS16 rf_esc_value;
UNS16 lr_esc_value;
UNS16 rr_esc_value;

static UNS8 esc_state;
static UNS32 esc_time;
static Servo lf_esc;
static Servo rf_esc;
static Servo lr_esc;
static Servo rr_esc;

void initializeEsc(void)
{
    esc_state = 0;
    esc_time = 0;
    lf_esc_value = MAX_ESC_COMMAND;
    rf_esc_value = MAX_ESC_COMMAND;
    lr_esc_value = MAX_ESC_COMMAND;
    rr_esc_value = MAX_ESC_COMMAND;
    lf_esc.attach(CH1);
    rf_esc.attach(CH2);
    lr_esc.attach(CH3);
    rr_esc.attach(CH4);
    driveEsc();
}

void driveEsc(void)
{
    lf_esc.writeMicroseconds(LIMIT(lf_esc_value, MIN_ESC_COMMAND, MAX_ESC_COMMAND));
    rf_esc.writeMicroseconds(LIMIT(rf_esc_value, MIN_ESC_COMMAND, MAX_ESC_COMMAND));
    lr_esc.writeMicroseconds(LIMIT(lr_esc_value, MIN_ESC_COMMAND, MAX_ESC_COMMAND));
    rr_esc.writeMicroseconds(LIMIT(rr_esc_value, MIN_ESC_COMMAND, MAX_ESC_COMMAND));
}

void stopEsc(void)
{
    lf_esc_value = MIN_ESC_COMMAND;
    lf_esc.writeMicroseconds(MIN_ESC_COMMAND);
    rf_esc_value = MIN_ESC_COMMAND;
    rf_esc.writeMicroseconds(MIN_ESC_COMMAND);
    lr_esc_value = MIN_ESC_COMMAND;
    lr_esc.writeMicroseconds(MIN_ESC_COMMAND);
    rr_esc_value = MIN_ESC_COMMAND;
    rr_esc.writeMicroseconds(MIN_ESC_COMMAND);
}

void driveLFEsc(UNS16 command)
{
    lf_esc_value = command;
    lf_esc.writeMicroseconds(command);
}

void driveRFEsc(UNS16 command)
{
    rf_esc_value = command;
    rf_esc.writeMicroseconds(command);
}

void driveLREsc(UNS16 command)
{
    lr_esc_value = command;
    lr_esc.writeMicroseconds(command);
}

void driveRREsc(UNS16 command)
{
    rr_esc_value = command;
    rr_esc.writeMicroseconds(command);
}