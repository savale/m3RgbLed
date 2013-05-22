// This program makes the LED on the Galago blink in a "heartbeat" pattern
// ----------

// Header files that contain type definitions for Galago's pins.
#include "LPC13xx.h"
//#include "GalagoAPI.h" 
#include "math.h"
#include "gpio.h"

// Shortcut for accessing all the typedefs instead of the `LPC1300` namespace,
// so that we don't have to write things like `LPC1300::GPIO1Data` all over the place
using namespace LPC1300;
//using namespace Galago;
using namespace Lpc13;

#define PART lpc1343


#define TLC5940_N 10 // if not fast enough, play with the interupt and gsclk

struct rgbLed {
	u16 red;
	u16 green;
	u16 blue;
};

static rgbLed s_rgbLeds[11] =
{ 0, };


struct ledstep 
{
	u8 led[10];
};
static ledstep s_pattern1[25] =
{ {1,1,1,1,1,1,1,1,1,1},
  {1,1,2,1,1,1,1,3,1,1},
  {1,2,3,2,1,1,3,2,3,1},
  {2,3,3,3,2,3,2,2,2,3},
  {3,3,3,3,3,2,2,2,2,2},
  {3,3,1,3,3,2,2,1,2,2},
  {3,1,2,1,3,2,1,3,1,2},
  {1,2,2,2,1,1,3,3,3,1},
  {2,2,2,2,2,3,3,3,3,3},
  {0,0,0,0,0,0,0,0,0,0},
  {1,2,3,1,2,3,1,2,3,1},
  {3,1,2,3,1,2,3,1,2,3},
  {2,3,1,2,3,1,2,3,1,2},
  {1,2,3,1,2,3,1,2,3,1},
  {3,1,2,3,1,2,3,1,2,3},
  {2,3,1,2,3,1,2,3,1,2},
  {1,2,3,1,2,3,1,2,3,1},
  {3,1,2,3,1,2,3,1,2,3},
  {2,3,1,2,3,1,2,3,1,2},
  {1,2,3,1,2,3,1,2,3,1},
  {0,0,3,0,0,0,0,3,0,0},
  {2,0,3,0,1,1,0,3,0,2},
  {2,2,3,1,1,1,1,3,2,2},
  {2,1,3,2,1,1,2,3,1,2},
  {1,1,3,2,2,2,2,3,1,1},
};

const rgbLed s_colors[4] =
{
	{0,0,0},
	{5,0,38},
	{0,20,4},
	{25,8,0},
};


int sleepForInterval(int interval)
{
  int sleepTime = 1 * interval;

  // Perform a bunch of no-op instructions to simulate sleeping  
  for(int i = 0; i < sleepTime; i++)
      __asm volatile("nop"::);
}


int init()
{
	/* Use external 12Mhz clock for 72Mhz speed */ 
	*PLLSourceUpdate = PLLSourceUpdate_Enable;
	*MainClockSourceUpdate = MainClockSourceUpdate_Enable;
	*MainClockSource = MainClockSource_PLLOutput;
	*PLLSource = PLLSource_ExternalClock;
	*MainBusDivider = 1;

/* Max GSCLK speed = 30Mhz */
*ClockOutputSource = ClockOutputSource_MainClock;
*ClockOutputSourceUpdate = ClockOutputSourceUpdate_Enable;
*ClockOutputDivider = 1; // Start with 72Mhz / 4 =  18Mhz





setHigh(*ClockControl, 6);
setHigh(*ClockControl, 7); // enable timer0 clock

	Gpio::Init();
	
//    *Timer0Interrupts = 0b0000; // op 1 zetten om match 0 interrupt te stoppen      
    
 
*Timer0Prescaler = 0; //(4096); // 16 bit prescaler

*Timer0Match0 = 4096;
setHigh(*Timer0MatchControl,0);
setHigh(*Timer0MatchControl,1);
setLow(*Timer0MatchControl,2);
//	*Timer0MatchControl |=  0b011; // 12bit register, interrupt en reset?
	//*Timer0CaptureControl =
//	setHigh(*InterruptEnableClear1,0); //om interrupt te clearen?
//	*InterruptEnableSet1 = Interrupt1_Timer0;
//setHigh(*InterruptEnableClear1,9);

    setLow(*Timer0Control, 0);
    setHigh(*Timer0Control, 1);
    setLow(*Timer0Control, 1);
    setHigh(*Timer0Control, 0);
//	*Timer0Control |= 0b10; // control reset
//	*Timer0Control |= 0b01; // control start / reset


//setHigh(*Timer0Interrupts,0);
    setHigh(*InterruptEnableSet1,9);
//	sleepForInterval(5000);
}

static bool s_isData = false;

void IRQ_Timer16_0(void)
{   
				*Timer0Interrupts = 1; // clears the interupt
	setHigh(LED_PORT, LED_PIN);
	u8 firstCycleFlag = 0;
	static const u32 rgbBits = 3 * 12;
	static u8 xlatNeedsPulse = 0;
	setHigh(BLANK_PORT, BLANK_PIN);
	if (outputState(VPRG_PORT, VPRG_PIN)) {
		setLow(VPRG_PORT, VPRG_PIN);
		firstCycleFlag = 1;
	}
	if (xlatNeedsPulse) {
		pulse(XLAT_PORT, XLAT_PIN);
		xlatNeedsPulse = 0;
	}
	if (firstCycleFlag)
	{
		pulse(SCLK_PORT, SCLK_PIN);
	}
	setLow(BLANK_PORT, BLANK_PIN);
	// Below this we have 4096 cycles to shift in the data for the next cycle
	u32 Data_Counter = 0;


	for (;;) {
		if ( !(Data_Counter > ((TLC5940_N * 192) - 1) ) && s_isData )
		{
								setLow(LED_PORT, LED_PIN);
			u32 invertedDataCounter = (TLC5940_N * 192) - 1 - Data_Counter;
            u32 ledValue = 0;
            u32 ledNumber = (invertedDataCounter / rgbBits);

                                struct rgbLed currentLed = s_rgbLeds[ledNumber];
                                u32 ledModulo = (invertedDataCounter % rgbBits);
                                u32 ledValueModulo = (invertedDataCounter % 12);
                                if( ledModulo < 12 )
                                {
                                        ledValue = currentLed.red;
                                }
                                else if ( ledModulo < 24 )
                                {
                                        ledValue = currentLed.green;
                                }
                                else
                                {
                                        ledValue = currentLed.blue;
                                }
                                //ledValue = 1;
                                bool bit = ((ledValue >> ledValueModulo )  & 0x01);
                                if(bit)
                                {
                                        setHigh(SIN_PORT, SIN_PIN);
                                }
                                else
                                {
                                        setLow(SIN_PORT, SIN_PIN);
                                }
                                
                                pulse(SCLK_PORT, SCLK_PIN);
                                Data_Counter++;
        } else {
        	s_isData = false;
							xlatNeedsPulse = 1;
							break;
						}
					}
				}

int main(void) {
init();
	u16 t;
u32 p;
for(p =0; p <11; p++)
{
	s_rgbLeds[p].red = 0;
	s_rgbLeds[p].green = 0;
	s_rgbLeds[p].blue = 0;
}
	for(t = 0; t < 4; t++)
	{		
		sleepForInterval(500000);
	}

u16 i;
u16 j;

for (j= 0; j < 10; j++) {
	
	for(i= 0; i < 10; i++)
	{
	 s_rgbLeds[j].blue += 2;
	 s_rgbLeds[j].red += 8;
	 s_rgbLeds[j].green = 0;
	sleepForInterval(10000);
	s_isData = true;
	}
		for(i= 0; i < 10; i++)
	{
	 s_rgbLeds[j].blue -= 2;
	 s_rgbLeds[j].red -= 8;
	 s_rgbLeds[j].green = 0;
	sleepForInterval(10000);
	s_isData = true;
	}
}
for (j= 8; j > 0; j--) {
	
	for(i= 0; i < 10; i++)
	{
	 s_rgbLeds[j].blue += 4;
	 s_rgbLeds[j].red = 0 ;
	 s_rgbLeds[j].green += 8;
	sleepForInterval(10000);
	s_isData = true;
	}
		for(i= 0; i < 10; i++)
	{
	 s_rgbLeds[j].blue -= 4;
	 s_rgbLeds[j].red = 0;
	 s_rgbLeds[j].green -= 8;
	sleepForInterval(10000);
	s_isData = true;
	}
}




for (;;) {
for(i = 0; i < 25; i++)
{
	ledstep step = s_pattern1[i];
	
	// set the leds
	for(j = 0; j < 10; j++)
	{
		rgbLed colorWeWant = s_colors[step.led[j]];

		 s_rgbLeds[j].blue = colorWeWant.blue;
		 s_rgbLeds[j].red = colorWeWant.red;
	 	 s_rgbLeds[j].green = colorWeWant.green;
	}

s_isData = true;
sleepForInterval(1000000);

}
}
for (;;) {
	
	for(i= 0; i < 100; i++)
	{
	 s_rgbLeds[4].blue += 2;
	 s_rgbLeds[4].red += 8;
	 s_rgbLeds[4].green = 0;
	// sleepForInterval(10);
	s_isData = true;
	}
	for(i= 0; i < 100; i++)
	{
	 s_rgbLeds[4].blue -= 2;
	 s_rgbLeds[4].red -= 8;
	 s_rgbLeds[4].green = 0;
	// sleepForInterval(10);
	s_isData = true;
	}
	sleepForInterval(5000);
	
	for(i= 0; i < 100; i++)
	{
	 s_rgbLeds[3].blue += 4;
	 s_rgbLeds[3].red = 0;
	 s_rgbLeds[3].green += 5;
	// sleepForInterval(10);	 
	s_isData = true;
	}
	for(i= 0; i < 100; i++)
	{
	 s_rgbLeds[3].blue -= 4;
	 s_rgbLeds[3].red = 0;
	 s_rgbLeds[3].green -= 5;
	 s_isData = true;
	// sleepForInterval(10);	 
	}

}
return 0;
}