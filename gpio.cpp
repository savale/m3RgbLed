/**
 * GPIO definitions and initialization for the Galago
 * (c) 2013 Sander van Leeuwen
 *
 */
 
#include "gpio.h"

using namespace LPC1300;

namespace Lpc13 {

void Gpio::Init(void)
{
   /* Set the PIN functions for controlling the tlc5940 */
   *GPIO1Data = 0; // Todo Sale: not needed?
   
   // Todo Sale: remove unneeded and move the PIO knowledge to the header
   *IOConfigPIO1_4 = IOConfigPIO1_4_Function_PIO | IOConfigPIO1_4_PullDown | IOConfigPIO1_4_DigitalMode;
   //*IOConfigPIO0_1 = IOConfigPIO0_1_Function_PIO | IOConfigPIO0_1_PullDown;
   *IOConfigPIO0_1 = IOConfigPIO0_1_Function_CLKOUT;
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
   *IOConfigPIO1_10 = IOConfigPIO1_10_Function_PIO | IOConfigPIO1_10_PullUp | IOConfigPIO1_10_DigitalMode;
   
   /* Set the PIN direction */
   setOutput(GSCLK_DDR, GSCLK_PIN);
   setOutput(SCLK_DDR, SCLK_PIN);
   setOutput(DCPRG_DDR, DCPRG_PIN);
   setOutput(VPRG_DDR, VPRG_PIN);
   setOutput(XLAT_DDR, XLAT_PIN);
   setOutput(BLANK_DDR, BLANK_PIN);
   setOutput(SIN_DDR, SIN_PIN);
   setOutput(LED_DDR, LED_PIN);
   
   /* Set PIN states */
   SetGpioInitState();
}

void Gpio::SetGpioInitState(void)
{
  /* Set the Init PIN states to initialize the tlc5940 */
  setLow(GSCLK_PORT, GSCLK_PIN);
  setLow(SCLK_PORT, SCLK_PIN);
  setLow(DCPRG_PORT, DCPRG_PIN);
  setHigh(VPRG_PORT, VPRG_PIN);
  setLow(XLAT_PORT, XLAT_PIN);
  setLow(SIN_PORT, SIN_PIN);
  setHigh(BLANK_PORT, BLANK_PIN); /* Turns off all LEDS */
    setHigh(LED_PORT, LED_PIN); /* Turns off LED */
}

}