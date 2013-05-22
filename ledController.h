/* Control all leds with higher level funtions */


/* sets the maximum update rate for the leds (i.e. for fading) */ 
#define MIN_TIME_BETWEEN_UPDATES_MS    20 

enum Color{
  None,
  One,
  Two,
  Three
};

enum ActionType
{
  Normal,         /* Only uses the imediate color and power */
  OnNextBeat,     /* Changes from color to nextColor and power to nextPower on the next audio beat */
  Timed,          /* Changes from color to nextColor and power to nextPower during the set time (morphing) */
  OnNextBeatTimed /* Changes from color to nextColor and power to nextPower during the set time (morphing) on the next beat */
}


/* LED masks */
#define LED_ALL   0b1111

#define LED_01    0b0001;
#define LED_02    0b0010;
#define LED_03    0b0100;
#define LED_04    0b1000;


/* private */
stuct ledControlState
{
  u16 actionTimeMs;                 /* Time the action is already running */
  bool ledUpdated;                  /* indicates if the led state is update */
};


struct LedControl
{
  Color       color;                /* The color the led will have imediately */
  Color       nextColor;            /* The color the led will have after the action completed */
  u16         powerPercentage;      /* The power the led will have imediately, 0% is off, 100% is full power */
  u16         nextPowerPercentage;  /* The power the led will have after the action completed */
  ActionType  actionType;           /* Type of action */
  u16         durationAction;       /* duration of the action in case of a timed action (miliseconds) */
};


/* Set led brightness: 100% is full, 0% = off */
void setLedControl(ledControl &ledControl, u64 &ledMask);


/* Private */
/* Poll the state of an action */
State getState(void);



/* Actions (only one at a time, must poll the state first) */

/* Keeps the state busy for a specified time */
void waitTime(u16 timeMs);

/* Keeps the state to busy while not detecting a "beat" in the audio */
void waitForNextBeat(void);

/* Keeps the state to busy while not detecting a "beat" in the audio */
void waitForAllCompleted(void);
