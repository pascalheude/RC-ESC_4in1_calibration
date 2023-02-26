#define MIN_ESC_COMMAND 1000
#define MAX_ESC_COMMAND 2000

extern UNS16 lf_esc_value;
extern UNS16 rf_esc_value;
extern UNS16 lr_esc_value;
extern UNS16 rr_esc_value;

extern void initializeEsc(void);
extern void driveEsc(void);
extern void stopEsc(void);
extern void driveLFEsc(UNS16 command);
extern void driveRFEsc(UNS16 command);
extern void driveLREsc(UNS16 command);
extern void driveRREsc(UNS16 command);