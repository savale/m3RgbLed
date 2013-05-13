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

/*
#define GSCLK_DDR *GPIO1Dir
#define GSCLK_PORT *GPIO1Data
#define GSCLK_PIN 8
#define SIN_DDR *GPIO0Dir
#define SIN_PORT *GPIO0Data
#define SIN_PIN 1
#define SCLK_DDR *GPIO2Dir
#define SCLK_PORT *GPIO2Data
#define SCLK_PIN 0
#define BLANK_DDR *GPIO0Dir
#define BLANK_PORT *GPIO0Data
#define BLANK_PIN 2
#define DCPRG_DDR *GPIO1Dir
#define DCPRG_PORT *GPIO1Data
#define DCPRG_PIN 9
#define VPRG_DDR *GPIO3Dir
#define VPRG_PORT *GPIO3Data
#define VPRG_PIN 2
#define XLAT_DDR *GPIO0Dir
#define XLAT_PORT *GPIO0Data
#define XLAT_PIN 6
*/
#define TLC5940_N 10

/*
#define setOutput(ddr, pin) ((ddr) |= (1 << (pin)))
#define setLow(port, pin) ((port) &= ~(1 << (pin)))
#define setHigh(port, pin) ((port) |= (1 << (pin)))
#define pulse(port, pin) do { \
setHigh((port), (pin)); \
setLow((port), (pin)); \
} while (0)
#define outputState(port, pin) ((port) & (1 << (pin)))
*/
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

	Gpio::Init();

/*	
	*GPIO1Data = 0;
  	*IOConfigPIO1_4 = IOConfigPIO1_4_Function_PIO | IOConfigPIO1_4_PullDown | IOConfigPIO1_4_DigitalMode;
  	*IOConfigPIO0_1 = IOConfigPIO0_1_Function_PIO | IOConfigPIO0_1_PullDown;
  	*IOConfigPIO0_2 = IOConfigPIO0_2_Function_PIO | IOConfigPIO0_2_PullDown;
  	*IOConfigPIO1_0 = IOConfigPIO1_0_Function_PIO | IOConfigPIO1_0_PullDown | IOConfigPIO1_0_DigitalMode;
  	*IOConfigPIO1_1 = IOConfigPIO1_1_Function_PIO | IOConfigPIO1_1_PullDown | IOConfigPIO1_1_DigitalMode;
  	*IOConfigPIO1_9 = IOConfigPIO1_9_Function_PIO | IOConfigPIO1_9_PullDown;
  	*IOConfigPIO1_8 = IOConfigPIO1_8_Function_PIO | IOConfigPIO1_8_PullDown;
  	*IOConfigPIO2_0 = IOConfigPIO2_0_Function_PIO | IOConfigPIO2_0_PullDown;
  	*IOConfigPIO0_6 = IOConfigPIO0_6_Function_PIO | IOConfigPIO0_6_PullDown;
  	*IOConfigPIO0_9 = IOConfigPIO0_9_Function_PIO | IOConfigPIO0_9_PullDown;
  	*IOConfigPIO3_2 = IOConfigPIO3_2_Function_PIO | IOConfigPIO3_2_PullDown;
  	*IOConfigPIO0_11 = IOConfigPIO0_11_Function_PIO | IOConfigPIO0_11_PullDown | IOConfigPIO0_11_DigitalMode;
  	// init tlc5940
	setOutput(GSCLK_DDR, GSCLK_PIN);
	setOutput(SCLK_DDR, SCLK_PIN);
	setOutput(DCPRG_DDR, DCPRG_PIN);
	setOutput(VPRG_DDR, VPRG_PIN);
	setOutput(XLAT_DDR, XLAT_PIN);
	setOutput(BLANK_DDR, BLANK_PIN);
	setOutput(SIN_DDR, SIN_PIN);
	setLow(GSCLK_PORT, GSCLK_PIN);
	setLow(SCLK_PORT, SCLK_PIN);
	setLow(DCPRG_PORT, DCPRG_PIN);
	setHigh(VPRG_PORT, VPRG_PIN);
	setLow(XLAT_PORT, XLAT_PIN);
	setLow(SIN_PORT, SIN_PIN);
	setHigh(BLANK_PORT, BLANK_PIN);
	*/
	sleepForInterval(5000);
}

void TLC5940_SetGS_And_GS_PWM(void) {
        bool firstCycleFlag = false;
        static const u32 rgbBits = 3 * 12;

        if (outputState(VPRG_PORT, VPRG_PIN)) {
                setLow(VPRG_PORT, VPRG_PIN);
                firstCycleFlag = true;
        }

        u32 GSCLK_Counter = 0;
        u32 Data_Counter = 0;
        setLow(BLANK_PORT, BLANK_PIN);

        for (;;) {
                if (GSCLK_Counter > 4096) {
                        // GSCLK_Counter = 4096
                        setHigh(BLANK_PORT, BLANK_PIN);
                        pulse(XLAT_PORT, XLAT_PIN);
                        if (firstCycleFlag) {
                                pulse(SCLK_PORT, SCLK_PIN);
                                firstCycleFlag = false;
                        }
                        break;
                } else {
                        // GSCLK_Counter < 4096
                        /* Now serialize the data */
                        if ( !(Data_Counter > ((TLC5940_N * 192) - 1) ) )
                        {
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
                        }
                }
                pulse(GSCLK_PORT, GSCLK_PIN);
                GSCLK_Counter++;
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
	for(t = 0; t < 40; t++)
	{
		TLC5940_SetGS_And_GS_PWM();
		sleepForInterval(5000);
	}

u16 i;
u16 j;
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

	for(t = 0; t < 30; t++)
	{
		TLC5940_SetGS_And_GS_PWM();
		sleepForInterval(50);
	}
}
}
for (;;) {
	
	for(i= 0; i < 100; i++)
	{
	 s_rgbLeds[4].blue += 2;
	 s_rgbLeds[4].red += 8;
	 s_rgbLeds[4].green = 0;
	// sleepForInterval(10);
     TLC5940_SetGS_And_GS_PWM();	 
	}
	for(i= 0; i < 100; i++)
	{
	 s_rgbLeds[4].blue -= 2;
	 s_rgbLeds[4].red -= 8;
	 s_rgbLeds[4].green = 0;
	// sleepForInterval(10);
     TLC5940_SetGS_And_GS_PWM();	 
	}
	sleepForInterval(5000);
	
	for(i= 0; i < 100; i++)
	{
	 s_rgbLeds[3].blue += 4;
	 s_rgbLeds[3].red = 0;
	 s_rgbLeds[3].green += 5;
	// sleepForInterval(10);
     TLC5940_SetGS_And_GS_PWM();	 
	}
	for(i= 0; i < 100; i++)
	{
	 s_rgbLeds[3].blue -= 4;
	 s_rgbLeds[3].red = 0;
	 s_rgbLeds[3].green -= 5;
	// sleepForInterval(10);
     TLC5940_SetGS_And_GS_PWM();	 
	}


//gsData[181] = 1;
//gsData[13] = 0;
sleepForInterval(5);
TLC5940_SetGS_And_GS_PWM();

//gsData[181] = 0;
//gsData[13] = 1;
sleepForInterval(5);
//gsData[186] = 0;
TLC5940_SetGS_And_GS_PWM();
}
return 0;
}