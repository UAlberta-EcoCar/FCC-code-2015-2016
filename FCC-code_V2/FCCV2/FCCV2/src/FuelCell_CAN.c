/*
 * FuelCell_CAN.c
 *
 * Created: 2015-11-20 9:26:57 PM
 *  Author: Reegan
 */ 

#include "asf.h"
#include "conf_can.h"
#include "FuelCell_CAN.h"

/* Local allocation for MOB buffer in HSB_RAM */
#if defined (__GNUC__) && defined (__AVR32__)
volatile can_msg_t mob_ram_ch0[NB_MOB_CHANNEL] __attribute__ ((section (".hsb_ram_loc")));
#elif defined (__ICCAVR32__)
volatile __no_init can_msg_t mob_ram_ch0[NB_MOB_CHANNEL] @0xA0000000;
#endif

/* Allocate one mob for TX */
can_mob_t fc_tx_mob;
//make a message object
can_msg_t fc_tx_msg;


/** Define the number of message received on CAN Channel 0 */
volatile U8 nb_message_received_on_channel1 = 0;


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

	
	/* Disable all interrupts. */
	Disable_global_interrupt();

	/* Initialize interrupt vectors. */
	INTC_init_interrupts();

	/* Initialize channel 1 */
	can_init(1, ((uint32_t)&mob_ram_ch0[1]),CANIF_CHANNEL_MODE_NORMAL,can_out_callback_channel1);

	/* Enable all interrupts. */
	Enable_global_interrupt();
}

U8 can_send_message(CanMessage * p_message)
{
	//initialize channel 0
	can_init(1, ((uint32_t)&mob_ram_ch0[1]), CANIF_CHANNEL_MODE_NORMAL,can_out_callback_channel1);
	
	fc_tx_mob.handle = can_mob_alloc(1); //get handle of a free mob
	
	fc_tx_msg.id = p_message->id;
	
	fc_tx_msg.data = p_message->data;

	/* Check return if no mob are available */
	//16 mobs should not be full
	if (fc_tx_mob.handle==CAN_CMD_REFUSED) 
	{
		while(1);
	}
	
	fc_tx_mob.dlc = p_message->length;
	fc_tx_mob.req_type = p_message->RTransR;
	fc_tx_mob.can_msg = &fc_tx_msg;
	
	can_tx(1, fc_tx_mob.handle, fc_tx_mob.dlc, fc_tx_mob.req_type,fc_tx_mob.can_msg);
	return(fc_tx_mob.handle);  //return the handle of the mob used so that it can be freed later
}

U8 check_if_message_sent(U8 handle)
{
	U8 status = can_mob_get_status(1,handle);
	if (status == CAN_STATUS_COMPLETED) //message has sent
	{
		//clear mob status
		can_clear_status(1,handle);
		//free mob
		can_mob_free(1,handle);
		return(0);
	}
	else if (status == CAN_STATUS_NOT_COMPLETED)
	{
		//message hasn't sent yet but everything is fine
		return(0);
	}
	else //CAN_STATUS_ERROR
	{
		//do something to report error
		return(1);
	}
}