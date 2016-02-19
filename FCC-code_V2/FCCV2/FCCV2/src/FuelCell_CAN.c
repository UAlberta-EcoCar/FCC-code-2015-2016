/*
 * FuelCell_CAN.c
 *
 * Created: 2015-11-20 9:26:57 PM
 *  Author: Reegan
 */ 

#include "asf.h"
#include "conf_can.h"
#include "FuelCell_CAN.h"

typedef struct  
{
	U32                   : 1;
	U32   rtr_bit         : 1;
	U32   ide_bit         : 1;
	U32   id_bit          : 29;
	U32                   : 1;
	U32   rtr_mask_bit    : 1;
	U32   ide_mask_bit    : 1;
	U32   id_mask_bit     : 29;
	Union64 data;
} my_mob_struct;

//allocate an mob

my_mob_struct my_mob;


/* Call Back called by can_drv */
//I think this is what runs when there is an interrupt
void can_out_callback_channel1(U8 handle, U8 event)
{
	//if message sending is complete
	if (can_mob_get_status(1,handle) == CAN_STATUS_COMPLETED)
	{
		can_mob_free(1,handle);
	}
} //i don't want interrupts but this needs to be defined


void CANInit(void)
{
	//start PLL1 for can bus to run off of
	const scif_pll_opt_t opt = {
		.osc = SCIF_GCCTRL_RC8M,     //"The PLLs can take either Oscillator 0, Oscillator 1 or 8MHz RC Oscillator (RC8M) as reference clock"
		.lockcount = 16,      // lockcount in main clock for the PLL wait lock
		.div = 0,             // PLLDIV=0 in the formula
		.mul = 7,             // PLLMUL=9
		.pll_div2 = 0,        // pll_div2 Divide the PLL output frequency by 2 (this settings does not change the FVCO value)
		.pll_wbwdisable = 0,  // pll_wbwdisable 1 Disable the Wide-Bandith Mode (Wide-Bandwith mode allow a faster startup time and out-of-lock time). 0 to enable the Wide-Bandith Mode.
		.pll_freq = 1,        // Set to 1 for VCO frequency range 80-180MHz, set to 0 for VCO frequency range 160-240Mhz. (VCO = 80 MHz in this case)
	};
	
	/* Setup PLL0 on RC8M, mul=9 ,no divisor, lockcount=16, ie. (8Mhzx(PLLMUL+1)*2  = 128 MHz output see page 100 of datasheet */
	scif_pll_setup(SCIF_PLL0, &opt); // lockcount in main clock for the PLL wait lock

	/* Enable PLL0 */
	scif_pll_enable(SCIF_PLL0);

	/* Wait for PLL0 locked */
	scif_wait_for_pll_locked(SCIF_PLL0) ;

	
	/* Setup the generic clock for CAN */
	scif_gc_setup(AVR32_SCIF_GCLK_CANIF,SCIF_GCCTRL_OSC0,AVR32_SCIF_GC_DIV_CLOCK,2);
	//128MHz / 2^(2+1) = 16MHz
	/* Now enable the generic clock */
	scif_gc_enable(AVR32_SCIF_GCLK_CANIF);

	//set pins to CAN-BUS function
	static const gpio_map_t CAN_GPIO_MAP = {
		{AVR32_CANIF_RXLINE_1_1_PIN, AVR32_CANIF_RXLINE_1_1_FUNCTION},
		{AVR32_CANIF_TXLINE_1_1_PIN, AVR32_CANIF_TXLINE_1_1_FUNCTION}
	};
	/* Assign GPIO to CAN. */
	gpio_enable_module(CAN_GPIO_MAP,
	sizeof(CAN_GPIO_MAP) / sizeof(CAN_GPIO_MAP[0]));

	//set timing bits
	CANIF_set_phs1(1,BAUDRATE_PHS1);
	CANIF_set_phs2(1,BAUDRATE_PHS2);
	CANIF_set_pres(1,BAUDRATE_PRES);
	CANIF_set_prs(1,BAUDRATE_PRS);
	CANIF_set_sjw(1,BAUDRATE_SJW);

	//set mode
	CANIF_set_channel_mode(1,CANIF_CHANNEL_MODE_NORMAL);

	//reset
	CANIF_set_reset(1);
	while(CANIF_channel_enable_status(1));
	CANIF_clr_reset(1);

	//set ram pointer to mob
	CANIF_set_ram_add(1,(unsigned long) &my_mob);
	
	//clear those mobs (i don;t know why)
	canif_clear_all_mob(1,1);
	
	//enable channel 1
	CANIF_enable(1);	
	
	//wait for CAN to enable	
	delay_ms(10);  //is it enabling?
	
}

