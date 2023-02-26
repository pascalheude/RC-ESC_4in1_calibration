#include <Arduino.h>
#include "standard.h"
#include "data_ini.h"
#include "drone.h"
#include "esc.h"
#include "led.h"
#include "mode.h"
#include "nrf24l01.h"

void initializeData(void)
{
    initializeDrone();
    initializeEsc();
    initializeLed();
    initializeMode();
    initializeRadio();
}