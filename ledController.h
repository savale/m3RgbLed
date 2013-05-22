/* Control all leds with higher level funtions */

enum Color{
  None,
  One,
  Two,
  Three
};


/* Must wait 
enum State{
  Idle,
  Busy /* busy with wating, fading or waiting for a beat */
};

/* Set led brightness: 100% is full, 0% = off */
void setAllLedPower(u16 percentage);

void setAllLeds(Color color);

/* Poll the state of an action */
State getState(void);



/* Actions (only one at a time, must poll the state first) */

void setAllFadeOut(u16 timeMs);

void setAllFadeIn(u16 timeMs);

void wait(u16 timeMs);

void waitForBeat(void);

