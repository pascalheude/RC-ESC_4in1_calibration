#include <Arduino.h>
#include "standard.h"
#include "mode.h"
#include "drone.h"
#include "esc.h"
#include "mathf.h"
#include "RC-ECS_4in1_calibration.h"
#include "rgb_lcd.h"
#include "spy.h"
 
typedef enum {
    NONE = 0,
    CALIBRATION_MAX,
    CALIBRATION_MIN,
    LF_COMMAND_MIN,
    LF_COMMAND_MIN_END,
    LF_COMMAND_MAX,
    LF_COMMAND_MAX_END,
    RF_COMMAND_MIN,
    RF_COMMAND_MIN_END,
    RF_COMMAND_MAX,
    RF_COMMAND_MAX_END,
    LR_COMMAND_MIN,
    LR_COMMAND_MIN_END,
    LR_COMMAND_MAX,
    LR_COMMAND_MAX_END,
    RR_COMMAND_MIN,
    RR_COMMAND_MIN_END,
    RR_COMMAND_MAX,
    RR_COMMAND_MAX_END,
    TEST
} T_mode;

static UNS16 command;
static UNS32 lo_push_button_time;
static UNS32 ro_push_button_time;
static T_mode mode;
static rgb_lcd lcd;

#define FILTER_PERIOD 250

void initializeMode(void)
{
    command = MIN_ESC_COMMAND;
    lo_push_button_time = 0;
    ro_push_button_time = 0;
    mode = NONE;
    lcd.begin(16, 2);
    lcd.print("NONE");
    lcd.setCursor(0, 1);
    lcd.print("SWITCH ON TX,ESC");
#ifdef SPY
    Serial.println("Mode : NONE");
    Serial.println("Switch on the transmitter");
#endif // SPY
}

// PROCEDURE DE CALIBRAGE DE L'ESC
// Mettre sous tension le drone (récepteur)
// Moteurs commandés à vitesse max
// Mettre sous tension l'emetteur
// Mettre sous tension l'ESC
// Attendre la fin de la chanson des moteurs
// Mettre le toggle gauche en position haute
// Attendre la fin de la chanson des moteurs
// L'ESC est calibré
// Remettre le toggle gauche en position basse

// RECHERCHE DE LA COMMANDE MINI ET MAXI DES MOTEURS
// MINI
// Appuyer sur le bouton LO pour faire augmenter la commande de 2
// jusqu'à ce que le moteur tourne sans saccade
// Noter la commande
// Appuyer sur le bouton RO pour trouver la commande MAXI
// MAXI
// Appuyer sur le bouton LO pour faire décroitre la commande de 2
// jusqu'à ce que le moteur sorte de ce plateau de rotation
// Noter la commande
// Appuyer sur le bouton RO pour passer au moteur suivant (LF, RF, LR, RR)

// TEST
// Appuyer sur le bouton LO pour augmenter la commande de 50 pour chaque moteur
// Appuyer sur le bouton RO pour diminuer la commande de 50 pour chaque moteur
// Appuyer sur le bouton LI pour remettre la commande de chaque moteur à sa valeur mini

void manageMode(void)
{
    switch(mode)
    {
        case NONE :
            if (F_one_reception_ok)
            {
                lcd.clear();
                lcd.print("CALIBRATION MAX");
                lcd.setCursor(0, 1);
                lcd.print("L TOGGLE UP");
                mode = CALIBRATION_MAX;
#ifdef SPY
                Serial.println("Mode : CALIBRATION_MAX");
                Serial.println("1-Switch on the ESC");
                Serial.println("2-Wait for the end of motor music");
                Serial.println("3-Put L toggle up");
#endif // SPY
            }
            else
            {
            }
            break;
        case CALIBRATION_MAX :
            if (l_toggle_switch == true)
            {
                lcd.clear();
                lcd.print("CALIBRATION MIN");
                lcd.setCursor(0, 1);
                lcd.print("L TOGGLE DOWN");
                mode = CALIBRATION_MIN;
                stopEsc();
#ifdef SPY
                Serial.println("Mode : CALIBRATION_MIN");
                Serial.println("1-Wait for the end of motor music");
                Serial.println("2-Put L toggle down to check LF motor minimum speed");
#endif // SPY
            }
            else
            {
            }
            break;
        case CALIBRATION_MIN :
            if (l_toggle_switch == false)
            {
                lcd.clear();
                lcd.print("LF COMMAND MIN");
                lcd.setCursor(1, 1);
                command = MIN_ESC_COMMAND;
                lcd.print(command);
                driveLFEsc(command);
                mode = LF_COMMAND_MIN;
#ifdef SPY
                Serial.println("Mode : LF_COMMAND_MIN");
                Serial.println("Push LO button to increase by 2 the LF motor command");
                Serial.println("Push RO button when finish");
#endif // SPY
            }
            else
            {
            }
            break;
        case LF_COMMAND_MIN :
            if (lo_push_button == true)
            {
                if (lo_push_button_time == 0)
                {
                    lo_push_button_time = pit_number;
                }
                else if ((pit_number - lo_push_button_time) > (FILTER_PERIOD/PIT_PERIOD))
                {
                    lo_push_button_time = 0;
                    command += 2;
                    driveLFEsc(command);
                    lcd.setCursor(1,1);
                    lcd.print(command);
                }
                else
                {
                }
            }
            else if (ro_push_button == true)
            {
                lo_push_button_time = 0;
                lcd.clear();
                lcd.print("LF COMMAND MAX");
                lcd.setCursor(1, 1);
                command = MAX_ESC_COMMAND;
                lcd.print(command);
                mode = LF_COMMAND_MIN_END;
#ifdef SPY
                Serial.println("Mode : LF_COMMAND_MAX");
                Serial.println("Push LO button to decrease by 2 the LF motor command");
                Serial.println("Push RO button when finish");
#endif // SPY
            }
            else
            {
                lo_push_button_time = 0;
            }
            break;
        case LF_COMMAND_MIN_END :
            if (ro_push_button == false)
            {
                driveLFEsc(command);
                mode = LF_COMMAND_MAX;
            }
            else
            {
            }
            break;
        case LF_COMMAND_MAX :
            if (lo_push_button == true)
            {
                if (lo_push_button_time == 0)
                {
                    lo_push_button_time = pit_number;
                }
                else if ((pit_number - lo_push_button_time) > (FILTER_PERIOD/PIT_PERIOD))
                {
                    lo_push_button_time = 0;
                    command -= 2;
                    driveLFEsc(command);
                    lcd.setCursor(1,1);
                    lcd.print(command);
                }
                else
                {
                }
            }
            else if (ro_push_button == true)
            {
                lo_push_button_time = 0;
                lcd.clear();
                lcd.print("RF COMMAND MIN");
                lcd.setCursor(1, 1);
                command = MIN_ESC_COMMAND;
                lcd.print(command);
                mode = LF_COMMAND_MAX_END;
#ifdef SPY
                Serial.println("Mode : RF_COMMAND_MIN");
                Serial.println("Push LO button to increase by 2 the RF motor command");
                Serial.println("Push RO button when finish");
#endif // SPY
            }
            else
            {
                lo_push_button_time = 0;
            }
            break;
        case LF_COMMAND_MAX_END :
            if (ro_push_button == false)
            {
                driveLFEsc(command);
                mode = RF_COMMAND_MIN;
            }
            else
            {
            }
            break;
        case RF_COMMAND_MIN :
            if (lo_push_button == true)
            {
                if (lo_push_button_time == 0)
                {
                    lo_push_button_time = pit_number;
                }
                else if ((pit_number - lo_push_button_time) > (FILTER_PERIOD/PIT_PERIOD))
                {
                    lo_push_button_time = 0;
                    command += 2;
                    driveRFEsc(command);
                    lcd.setCursor(1,1);
                    lcd.print(command);
                }
                else
                {
                }
            }
            else if (ro_push_button == true)
            {
                lo_push_button_time = 0;
                lcd.clear();
                lcd.print("RF COMMAND MAX");
                lcd.setCursor(1, 1);
                command = MAX_ESC_COMMAND;
                lcd.print(command);
                mode = RF_COMMAND_MIN_END;
#ifdef SPY
                Serial.println("Mode : RF_COMMAND_MAX");
                Serial.println("Push LO button to decrease by 2 the RF motor command");
                Serial.println("Push RO button when finish");
#endif // SPY
            }
            else
            {
                lo_push_button_time = 0;
            }
            break;
        case RF_COMMAND_MIN_END :
            if (ro_push_button == false)
            {
                driveRFEsc(command);
                mode = RF_COMMAND_MAX;
            }
            else
            {
            }
            break;
        case RF_COMMAND_MAX :
            if (lo_push_button == true)
            {
                if (lo_push_button_time == 0)
                {
                    lo_push_button_time = pit_number;
                }
                else if ((pit_number - lo_push_button_time) > (FILTER_PERIOD/PIT_PERIOD))
                {
                    lo_push_button_time = 0;
                    command -= 2;
                    driveRFEsc(command);
                    lcd.setCursor(1,1);
                    lcd.print(command);
                }
                else
                {
                }
            }
            else if (ro_push_button == true)
            {
                lo_push_button_time = 0;
                lcd.clear();
                lcd.print("LR COMMAND MIN");
                lcd.setCursor(1, 1);
                command = MIN_ESC_COMMAND;
                lcd.print(command);
                mode = RF_COMMAND_MAX_END;
#ifdef SPY
                Serial.println("Mode : LR_COMMAND_MIN");
                Serial.println("Push LO button to increase by 2 the LR motor command");
                Serial.println("Push RO button when finish");
#endif // SPY
            }
            else
            {
                lo_push_button_time = 0;
            }
            break;
        case RF_COMMAND_MAX_END :
            if (ro_push_button == false)
            {
                driveRFEsc(command);
                mode = LR_COMMAND_MIN;
            }
            else
            {
            }
            break;
        case LR_COMMAND_MIN :
            if (lo_push_button == true)
            {
                if (lo_push_button_time == 0)
                {
                    lo_push_button_time = pit_number;
                }
                else if ((pit_number - lo_push_button_time) > (FILTER_PERIOD/PIT_PERIOD))
                {
                    lo_push_button_time = 0;
                    command += 2;
                    driveLREsc(command);
                    lcd.setCursor(1,1);
                    lcd.print(command);
                }
                else
                {
                }
            }
            else if (ro_push_button == true)
            {
                lo_push_button_time = 0;
                lcd.clear();
                lcd.print("LR COMMAND MAX");
                lcd.setCursor(1, 1);
                command = MAX_ESC_COMMAND;
                lcd.print(command);
                mode = LR_COMMAND_MIN_END;
#ifdef SPY
                Serial.println("Mode : LR_COMMAND_MAX");
                Serial.println("Push LO button to decrease by 2 the LR motor command");
                Serial.println("Push RO button when finish");
#endif // SPY
            }
            else
            {
                lo_push_button_time = 0;
            }
            break;
        case LR_COMMAND_MIN_END :
            if (ro_push_button == false)
            {
                driveLREsc(command);
                mode = LR_COMMAND_MAX;
            }
            else
            {
            }
            break;
        case LR_COMMAND_MAX :
            if (lo_push_button == true)
            {
                if (lo_push_button_time == 0)
                {
                    lo_push_button_time = pit_number;
                }
                else if ((pit_number - lo_push_button_time) > (FILTER_PERIOD/PIT_PERIOD))
                {
                    lo_push_button_time = 0;
                    command -= 2;
                    driveLREsc(command);
                    lcd.setCursor(1,1);
                    lcd.print(command);
                }
                else
                {
                }
            }
            else if (ro_push_button == true)
            {
                lo_push_button_time = 0;
                lcd.clear();
                lcd.print("RR COMMAND MIN");
                lcd.setCursor(1, 1);
                command = MIN_ESC_COMMAND;
                lcd.print(command);
                mode = LR_COMMAND_MAX_END;
#ifdef SPY
                Serial.println("Mode : RR_COMMAND_MIN");
                Serial.println("Push LO button to increase by 2 the RR motor command");
                Serial.println("Push RO button when finish");
#endif // SPY
            }
            else
            {
                lo_push_button_time = 0;
            }
            break;
        case LR_COMMAND_MAX_END :
            if (ro_push_button == false)
            {
                driveLREsc(command);
                mode = RR_COMMAND_MIN;
            }
            else
            {
            }
            break;
        case RR_COMMAND_MIN :
            if (lo_push_button == true)
            {
                if (lo_push_button_time == 0)
                {
                    lo_push_button_time = pit_number;
                }
                else if ((pit_number - lo_push_button_time) > (FILTER_PERIOD/PIT_PERIOD))
                {
                    lo_push_button_time = 0;
                    command += 2;
                    driveRREsc(command);
                    lcd.setCursor(1,1);
                    lcd.print(command);
                }
                else
                {
                }
            }
            else if (ro_push_button == true)
            {
                lo_push_button_time = 0;
                lcd.clear();
                lcd.print("RR COMMAND MAX");
                lcd.setCursor(1, 1);
                command = MAX_ESC_COMMAND;
                lcd.print(command);
                mode = RR_COMMAND_MIN_END;
#ifdef SPY
                Serial.println("Mode : RR_COMMAND_MAX");
                Serial.println("Push LO button to decrease by 2 the RR motor command");
                Serial.println("Push RO button when finish");
#endif // SPY
            }
            else
            {
                lo_push_button_time = 0;
            }
            break;
        case RR_COMMAND_MIN_END :
            if (ro_push_button == false)
            {
                driveRREsc(command);
                mode = RR_COMMAND_MAX;
            }
            else
            {
            }
            break;
        case RR_COMMAND_MAX :
            if (lo_push_button == true)
            {
                if (lo_push_button_time == 0)
                {
                    lo_push_button_time = pit_number;
                }
                else if ((pit_number - lo_push_button_time) > (FILTER_PERIOD/PIT_PERIOD))
                {
                    lo_push_button_time = 0;
                    command -= 2;
                    driveRREsc(command);
                    lcd.setCursor(1,1);
                    lcd.print(command);
                }
                else
                {
                }
            }
            else if (ro_push_button == true)
            {
                lo_push_button_time = 0;
                lcd.clear();
                lcd.print("TEST");
                lcd.setCursor(1, 1);
                command = MIN_ESC_COMMAND;
                lcd.print(command);
                mode = RR_COMMAND_MAX_END;
#ifdef SPY
                Serial.println("Mode : TEST");
                Serial.println("Push LI button to drive all 4 motors to minimum speed");
                Serial.println("Push LO button to decrease all 4 motors speed by 50");
                Serial.println("Push RO button to increase all 4 motors speed by 50");
                Serial.println("Switch off ESC and transmitter to end calibration procedure");
#endif // SPY
            }
            else
            {
                lo_push_button_time = 0;
            }
            break;
        case RR_COMMAND_MAX_END :
            if (ro_push_button == false)
            {
                driveRREsc(command);
                mode = TEST;
            }
            else
            {
            }
            break;
        case TEST :
            if (li_push_button == true)
            {
                lo_push_button_time = 0;
                ro_push_button_time = 0;
                command = MIN_ESC_COMMAND;
                driveLFEsc(command);
                driveRFEsc(command);
                driveLREsc(command);
                driveRREsc(command);
                lcd.setCursor(1,1);
                lcd.print(command);
            }
            else if (lo_push_button == true)
            {
                ro_push_button_time = 0;
                if (lo_push_button_time == 0)
                {
                    lo_push_button_time = pit_number;
                }
                else if ((pit_number - lo_push_button_time) > (FILTER_PERIOD/PIT_PERIOD))
                {
                    lo_push_button_time = 0;
                    command = LIMIT(command - 50, MIN_ESC_COMMAND, MAX_ESC_COMMAND);
                    driveLFEsc(command);
                    driveRFEsc(command);
                    driveLREsc(command);
                    driveRREsc(command);
                    lcd.setCursor(1,1);
                    lcd.print(command);
                }
                else
                {
                }
            }
            else if (ro_push_button == true)
            {
                lo_push_button_time = 0;
                if (ro_push_button_time == 0)
                {
                    ro_push_button_time = pit_number;
                }
                else if ((pit_number - ro_push_button_time) > (FILTER_PERIOD/PIT_PERIOD))
                {
                    ro_push_button_time = 0;
                    command = LIMIT(command + 50, MIN_ESC_COMMAND, MAX_ESC_COMMAND);
                    driveLFEsc(command);
                    driveRFEsc(command);
                    driveLREsc(command);
                    driveRREsc(command);
                    lcd.setCursor(1,1);
                    lcd.print(command);
                }
                else
                {
                }
            }
            else
            {
                lo_push_button_time = 0;
                ro_push_button_time = 0;
            }
            if (F_no_reception)
            {
                lcd.clear();
                lcd.print("NONE");
                mode = NONE;
#ifdef SPY
                Serial.println("Mode : NONE");
                Serial.println("Switch on the transmitter");
#endif // SPY
            }
            else
            {
            }
            break;
   }
}