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
  u16         powerPercentage;      /* The power the led will have imediately, 0% is off, 100% is full power */
  ActionType  actionType;           /* Type of action */
  struct next{                      /* In case there is an action selected that needs a next state */
    Color       nextColor;            /* The color the led will have after the action completed */
    u16         nextPowerPercentage;  /* The power the led will have after the action completed */
    u16         actionDuration;       /* duration of the action in case of a timed action (miliseconds) */
  };
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


/* some functions with default actions */
ledControl ledOff(void);
ledControl ledOn(Color color, u16 power);


/* RGB color defines */
#define yellow (4095, 4095, 0)
#define green (0, 4095, 0)

/* 
 * Example creation of a pattern 
 */

setColor(ColorOne, yellow); /* define color one to yellow */
setColor(ColorTwo, green); /* define color two to green */

/* custom action */
LedControl nextStep = {
  color=None; /* led off */
  nextColor=One; /* yellow */
  powerPercentage=0; /* Start with led off */
  nextPowerPercentage=100; /* Ends with full power on */
  actionType=OnNextBeatTimed; /* a fade in action that starts on the next audio beat */
  actionDuration = 200; /* duration of the fade */
}

/* start of the pattern */
setLedControl(ledOff, LED_ALL); /* Start pattern with turning off all leds */

/* wait three audio beats */
waitForNextBeat();
waitForNextBeat();
waitForNextBeat();

/* With all leds off and fade them on during 200ms on the next audio beat (100%, yellow) */
setLedControl(nextStep, LED_ALL);
waitForAllCompleted(void); /* wait for the action to be completed */

setLedControl(ledOff, LED_ALL); /* Turn off all leds again */

/* wait three audio beats again */
waitForNextBeat();
waitForNextBeat();
waitForNextBeat();

/* With all leds off and fade them on during 200ms on the next audio beat (100%, yellow) */
setLedControl(nextStep, LED_ALL);
waitForAllCompleted(void); /* wait for the action to be completed */

/* Now start with a timed pattern (led power 50%) */
setLedControl(ledOn(one, 50), LED_01 & LED_02); /* Set led1 and 2 to yellow */
waitTime(500);                                  /* wait half a second */
setLedControl(ledOff, LED_ALL);                 /* All leds off */
setLedControl(ledOn(two, 50), LED_03 & LED_04 & LED_01); /* Set led3, led4 and led 1 to green */
waitTime(1000);                                  /* wait a second */

/* Now morph the led3, led4 and led 1 from green to yellow */

/* first set the next step */
nextStep = {
  color=Two; /* led gree */
  nextColor=One; /* yellow */
  powerPercentage=50; /* half power*/
  nextPowerPercentage=50; /* Ends with half power */
  actionType=Timed; /* a color morph action */
  actionDuration = 500; /* duration of the morph is half a second */
}

/* do the morph */
setLedControl(nextStep, LED_03 & LED_04 & LED_01);
waitForAllCompleted(void); /* wait for the action to be completed */

setLedControl(ledOff, LED_ALL); /* Turn off all leds again */

