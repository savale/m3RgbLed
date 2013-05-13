/**
 * GPIO definitions and initialisation for the Galago
 * (c) 2013 Sander van Leeuwen
 *
 */

#ifndef __LPC13_GPIO_H__
#define __LPC13_GPIO_H__

#include "LPC13xx.h"
#include "GalagoAPI.h"

/* Pin definitions */
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

/* Pin functions */
#define setOutput(ddr, pin) ((ddr) |= (1 << (pin))) // TODO SaLe: use _BIT()?

#define setLow(port, pin) ((port) &= ~(1 << (pin)))

#define setHigh(port, pin) ((port) |= (1 << (pin)))

#define pulse(port, pin) do { \
setHigh((port), (pin)); \
setLow((port), (pin)); \
} while (0)

#define outputState(port, pin) ((port) & (1 << (pin)))

/* GPIO Initialisation */
extern void GpioInit(void);

#endif /* __LPC13_GPIO_H__ */
