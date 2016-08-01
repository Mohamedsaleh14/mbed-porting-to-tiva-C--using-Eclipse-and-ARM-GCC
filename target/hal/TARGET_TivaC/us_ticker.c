#include <stddef.h>
#include "us_ticker_api.h"
#include "core_cm4.h"
#include "system_LM4F.h"
#include "cmsis_nvic.h"


#define EN_CLK_TIMER0					(1<<0)
#define TIMERA_PERIODIC_MODE			(1<<1)
#define TIMERA_COUNT_UP					(1<<4)
#define TIMERA_TIMEOUT_INTERRUPT		(1<<0)
#define TIMERA_ENABLE_BIT				(1<<0)
#define TIMERA_STALL_BIT				(1<<1)
#define TIMERA_CLEAR_INT				(1<<0)

static uint8_t us_ticker_inited = 0;
TIMER0_Type	os_timer;


void us_ticker_init(void)
{
	uint32_t x =0;
	if (us_ticker_inited) return;
	us_ticker_inited = 1;

	SYSCTL->RCGCTIMER = EN_CLK_TIMER0;
	TIMER0->CTL 	  = 0x00;
	TIMER0->CFG 	  = 0x00;
	TIMER0->TAMR 	  = TIMERA_PERIODIC_MODE|TIMERA_COUNT_UP;
	//TIMER0->TAILR 	  = 80;
	TIMER0->IMR		  = TIMERA_TIMEOUT_INTERRUPT;
	TIMER0->CTL		  = TIMERA_ENABLE_BIT|TIMERA_STALL_BIT;
	NVIC_SetVector(TIMER0A_IRQn, (uint32_t)us_ticker_irq_handler);
    NVIC_EnableIRQ(TIMER0A_IRQn);
}

/** Read the current counter
 *
 * @return The current timer's counter value in microseconds
 */
uint32_t us_ticker_read() {
	uint32_t return_ticks;
    if (!us_ticker_inited)
        us_ticker_init();
    return_ticks = TIMER0->TAR;
    return_ticks = (return_ticks/80);
    return return_ticks;
}

/** Disable us ticker interrupt
 *
 */
void us_ticker_disable_interrupt(void)
{
	TIMER0->IMR &= ~TIMERA_TIMEOUT_INTERRUPT;
}

/** Clear us ticker interrupt
 *
 */
void us_ticker_clear_interrupt(void)
{
	TIMER0->ICR = TIMERA_CLEAR_INT;
	NVIC_ClearPendingIRQ(TIMER0A_IRQn);
}

/** Set interrupt for specified timestamp
 *
 * @param timestamp The time in microseconds to be set
 */
void us_ticker_set_interrupt(timestamp_t timestamp)
{
	TIMER0->TAILR 	  = 80*timestamp;
}
