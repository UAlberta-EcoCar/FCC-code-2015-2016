/*
 * FuelCell_PWM.c
 *
 * Created: 2015-11-10 2:33:13 PM
 *  Author: Reegan
 */ 

/* The PWM and Tachometer signals for Fan 4 have not been initialized. The PWM signal 
is being kept unused for unexpected changes to the system in the near future that require
a GPIO connection. The Tachometer signal is being repurposed to read the "Probe disconnect
signal from the CVM board"*/

#include "asf.h"
#include "FuelCell_PWM.h"


#define FAN1_PWM_PIN AVR32_PWM_PWMH_1_PIN // Corresponds to FAN 1
#define FAN1_PWM_FUNCTION AVR32_PWM_PWMH_1_FUNCTION
#define FAN1_PWM_CHANNEL_ID 1

#define FAN2_PWM_PIN AVR32_PWM_PWML_2_PIN // Corresponds to FAN 2
#define FAN2_PWM_FUNCTION AVR32_PWM_PWML_2_FUNCTION
#define FAN2_PWM_CHANNEL_ID 2

#define FAN3_PWM_PIN AVR32_PWM_PWMH_2_PIN // Corresponds to FAN 3
#define FAN3_PWM_FUNCTION AVR32_PWM_PWMH_2_FUNCTION
#define FAN3_PWM_CHANNEL_ID 3

#define FAN5_PWM_PIN AVR32_PWM_PWMH_3_PIN // Corresponds to FAN 5
#define FAN5_PWM_FUNCTION AVR32_PWM_PWMH_3_FUNCTION
#define FAN5_PWM_CHANNEL_ID 5



// Start oscillator and enable PLL0, sourced by RC8M
static void local_start_highfreq_clock(void)
{
	const scif_pll_opt_t opt = {
		.osc = SCIF_GCCTRL_RC8M,     //"The PLLs can take either Oscillator 0, Oscillator 1 or 8MHz RC Oscillator (RC8M) as reference clock"
		.lockcount = 16,      // lockcount in main clock for the PLL wait lock
		.div = 0,             // PLLDIV=0 in the formula
		.mul = 6,             // PLLMUL=6
		.pll_div2 = 1,        // pll_div2 Divide the PLL output frequency by 2 (this settings does not change the FVCO value)
		.pll_wbwdisable = 0,  // pll_wbwdisable 1 Disable the Wide-Bandith Mode (Wide-Bandwith mode allow a faster startup time and out-of-lock time). 0 to enable the Wide-Bandith Mode.
		.pll_freq = 1,        // Set to 1 for VCO frequency range 80-180MHz, set to 0 for VCO frequency range 160-240Mhz. (VCO = 80 MHz in this case)
	};
	
	//already started
	//scif_start_rc8M();

	/* Setup PLL0 on RC8M, mul=13 ,no divisor, lockcount=16, ie. (8Mhzx(PLLMUL+1)*2 / 2 = 56 MHz output see page 100 of datasheet */
	scif_pll_setup(SCIF_PLL0, &opt); // lockcount in main clock for the PLL wait lock

	/* Enable PLL0 */
	scif_pll_enable(SCIF_PLL0);

	/* Wait for PLL0 locked */
	scif_wait_for_pll_locked(SCIF_PLL0) ;
}



// Start PWM generic clock input
static void pwm_start_gc(void)
{
	//PWM GENERIC CLOCK: source:PLL0 56MHz
	scif_gc_setup(AVR32_SCIF_GCLK_PWM,SCIF_GCCTRL_PLL0,AVR32_SCIF_GC_NO_DIV_CLOCK,0);
	// Now enable the generic clock
	scif_gc_enable(AVR32_SCIF_GCLK_PWM);
}

pwm_opt_t pwm_opt;                // PWM option config.


//fan 1 channel configuration
avr32_pwm_channel_t fan1_pwm_channel = {
	{0}, // cmr
	{0}, // cdty
	{0}, // cdtyupd
	{0}, // cprd
	{0}, // cprdupd
	{0}, // ccnt
	{0}, // dt
	{0}};// dtupd  ;  fan channel config.

//fan 2 channel configuration
avr32_pwm_channel_t fan2_pwm_channel = {
	{0}, // cmr
	{0}, // cdty
	{0}, // cdtyupd
	{0}, // cprd
	{0}, // cprdupd
	{0}, // ccnt
	{0}, // dt
	{0}};// dtupd  ;  fan channel config.
	
//fan 3 channel configuration
avr32_pwm_channel_t fan3_pwm_channel = {
	{0}, // cmr
	{0}, // cdty
	{0}, // cdtyupd
	{0}, // cprd
	{0}, // cprdupd
	{0}, // ccnt
	{0}, // dt
	{0}};// dtupd  ;  fan channel config.
	
//fan 5 channel configuration
avr32_pwm_channel_t fan5_pwm_channel = {
	{0}, // cmr
	{0}, // cdty
	{0}, // cdtyupd
	{0}, // cprd
	{0}, // cprdupd
	{0}, // ccnt
	{0}, // dt
	{0}};// dtupd  ;  fan channel config.



void PWMInit(void)
{	
	// Start PLL for PWM
	local_start_highfreq_clock();
	//Fpll = 56 MHz
	
	
	// Start Enable Generic Clock with PLL as source clock
	pwm_start_gc();
	//no divider
	//Fgc = MCK = 56MHz
		
	//set pins to pwm function
	gpio_enable_module_pin(FAN1_PWM_PIN, FAN1_PWM_FUNCTION);
	gpio_enable_module_pin(FAN2_PWM_PIN, FAN2_PWM_FUNCTION);
	gpio_enable_module_pin(FAN3_PWM_PIN, FAN3_PWM_FUNCTION);
	gpio_enable_module_pin(FAN5_PWM_PIN, FAN5_PWM_FUNCTION);
	
	// PWM controller configuration.
	pwm_opt.diva = AVR32_PWM_DIVA_CLK_OFF;
	pwm_opt.divb = AVR32_PWM_DIVB_CLK_OFF;
	pwm_opt.prea = AVR32_PWM_PREA_CCK;
	pwm_opt.preb = AVR32_PWM_PREB_CCK;

	pwm_opt.fault_detection_activated = false;
	pwm_opt.sync_channel_activated    = true;
	pwm_opt.sync_update_channel_mode  = PWM_SYNC_CHANNEL_OFF;
	pwm_opt.sync_channel_select[0]    = false;
	pwm_opt.sync_channel_select[1]    = false;
	pwm_opt.sync_channel_select[2]    = false;
	pwm_opt.sync_channel_select[3]    = false;
	pwm_opt.cksel                     = PWM_CKSEL_GCLK;
	pwm_init(&pwm_opt);

	// Update the period why?
	pwm_update_period_value(10);


	//fan 1 Channel configuration
	fan1_pwm_channel.CMR.dte   = 1;        // Enable Deadtime for complementary Mode
	fan1_pwm_channel.CMR.dthi  = 1;        // Deadtime Inverted on PWMH
	fan1_pwm_channel.CMR.dtli  = 0;        // Deadtime Not Inverted on PWML
	fan1_pwm_channel.CMR.ces   = 0;        // 0/1 Channel Event at the End of PWM Period
	fan1_pwm_channel.CMR.calg  = PWM_MODE_LEFT_ALIGNED;       // Channel mode.
	fan1_pwm_channel.CMR.cpol  = PWM_POLARITY_LOW;            // Channel polarity.
	fan1_pwm_channel.CMR.cpre  = 2;           // Channel prescaler: divide by 2 see page 1036 of datasheet
	fan1_pwm_channel.cdty      = 0;       // Channel duty cycle, should be < CPRD.
	fan1_pwm_channel.cprd      = 1024;       // Channel period.

	//Fpwm = (MCK/prescaler)/period
	//MCK  == 56MHz
	//prescaler == 2
	//period == 1024
	//Fpwm == 27.34KHz


	//fan 2 Channel configuration
	fan2_pwm_channel.CMR.dte   = 1;        // Enable Deadtime for complementary Mode
	fan2_pwm_channel.CMR.dthi  = 1;        // Deadtime Inverted on PWMH
	fan2_pwm_channel.CMR.dtli  = 0;        // Deadtime Not Inverted on PWML
	fan2_pwm_channel.CMR.ces   = 0;        // 0/1 Channel Event at the End of PWM Period
	fan2_pwm_channel.CMR.calg  = PWM_MODE_LEFT_ALIGNED;       // Channel mode.
	fan2_pwm_channel.CMR.cpol  = PWM_POLARITY_LOW;            // Channel polarity.
	fan2_pwm_channel.CMR.cpre  = 2;           // Channel prescaler: divide by 2 see page 1036 of datasheet
	fan2_pwm_channel.cdty      = 0;       // Channel duty cycle, should be < CPRD.
	fan2_pwm_channel.cprd      = 1024;       // Channel period.

	//Fpwm = (MCK/prescaler)/period
	//MCK  == 56MHz
	//prescaler == 2
	//period == 1024
	//Fpwm == 27.34KHz
	
	//fan 3 Channel configuration
	fan3_pwm_channel.CMR.dte   = 1;        // Enable Deadtime for complementary Mode
	fan3_pwm_channel.CMR.dthi  = 1;        // Deadtime Inverted on PWMH
	fan3_pwm_channel.CMR.dtli  = 0;        // Deadtime Not Inverted on PWML
	fan3_pwm_channel.CMR.ces   = 0;        // 0/1 Channel Event at the End of PWM Period
	fan3_pwm_channel.CMR.calg  = PWM_MODE_LEFT_ALIGNED;       // Channel mode.
	fan3_pwm_channel.CMR.cpol  = PWM_POLARITY_LOW;            // Channel polarity.
	fan3_pwm_channel.CMR.cpre  = 2;           // Channel prescaler: divide by 2 see page 1036 of datasheet
	fan3_pwm_channel.cdty      = 0;       // Channel duty cycle, should be < CPRD.
	fan3_pwm_channel.cprd      = 1024;       // Channel period.

	//Fpwm = (MCK/prescaler)/period
	//MCK  == 56MHz
	//prescaler == 2
	//period == 1024
	//Fpwm == 27.34KHz

	//fan 5 Channel configuration
	fan5_pwm_channel.CMR.dte   = 1;        // Enable Deadtime for complementary Mode
	fan5_pwm_channel.CMR.dthi  = 1;        // Deadtime Inverted on PWMH
	fan5_pwm_channel.CMR.dtli  = 0;        // Deadtime Not Inverted on PWML
	fan5_pwm_channel.CMR.ces   = 0;        // 0/1 Channel Event at the End of PWM Period
	fan5_pwm_channel.CMR.calg  = PWM_MODE_LEFT_ALIGNED;       // Channel mode.
	fan5_pwm_channel.CMR.cpol  = PWM_POLARITY_LOW;            // Channel polarity.
	fan5_pwm_channel.CMR.cpre  = 2;           // Channel prescaler: divide by 2 see page 1036 of datasheet
	fan5_pwm_channel.cdty      = 0;       // Channel duty cycle, should be < CPRD.
	fan5_pwm_channel.cprd      = 1024;       // Channel period.

	//Fpwm = (MCK/prescaler)/period
	//MCK  == 56MHz
	//prescaler == 2
	//period == 1024
	//Fpwm == 27.34KHz
	
	pwm_channel_init(FAN5_PWM_CHANNEL_ID, &fan5_pwm_channel);
	pwm_channel_init(FAN3_PWM_CHANNEL_ID, &fan3_pwm_channel);
	pwm_channel_init(FAN2_PWM_CHANNEL_ID, &fan2_pwm_channel);
	pwm_channel_init(FAN1_PWM_CHANNEL_ID, &fan1_pwm_channel); 
	pwm_start_channels((1 << FAN5_PWM_CHANNEL_ID));
	pwm_start_channels((1 << FAN3_PWM_CHANNEL_ID));
	pwm_start_channels((1 << FAN2_PWM_CHANNEL_ID));
	pwm_start_channels((1 << FAN1_PWM_CHANNEL_ID)); 
}


int pwm_update_duty_cycle(unsigned int channel_id, const avr32_pwm_channel_t *pwm_channel);

//I modified pwm_update_channel(unsigned int channel_id, const avr32_pwm_channel_t *pwm_channel) to make this
//not sure what the lines i commented out do
int pwm_update_duty_cycle(unsigned int channel_id, const avr32_pwm_channel_t *pwm_channel)
{
	volatile avr32_pwm_t *pwm = &AVR32_PWM;

	if (channel_id > AVR32_PWM_LINES_MSB)
	return PWM_INVALID_INPUT;

	AVR32_PWM.isr1;                                    // Aknowledgement and clear previous register state.
	while (!(AVR32_PWM.isr1 & (1 << channel_id)));     // Wait until the last write has been taken into account.
	pwm->channel[channel_id].cdtyupd= pwm_channel->cdtyupd; // Channel update Duty Cycle
	//pwm->channel[channel_id].cprdupd= pwm_channel->cprdupd; // Channel update Period
	//pwm->channel[channel_id].dtupd= pwm_channel->dtupd;     // Channel update Dead Time

	return PWM_SUCCESS;
}

unsigned int FANSpeed;
int FANUpdate(int duty_cycle)
{
	if(duty_cycle < 1)
	{
		FANSpeed = 0;
	}
	else if (duty_cycle > 1023)
	{
		FANSpeed = 1023;
	}
	else
	{
		FANSpeed = duty_cycle;
	}
	fan1_pwm_channel.cdtyupd = FANSpeed;
	fan2_pwm_channel.cdtyupd = FANSpeed;
	fan3_pwm_channel.cdtyupd = FANSpeed;
	fan5_pwm_channel.cdtyupd = FANSpeed;
	
	pwm_update_duty_cycle(FAN1_PWM_CHANNEL_ID,&fan1_pwm_channel)
	pwm_update_duty_cycle(FAN2_PWM_CHANNEL_ID,&fan2_pwm_channel)
	pwm_update_duty_cycle(FAN3_PWM_CHANNEL_ID,&fan3_pwm_channel)
	pwm_update_duty_cycle(FAN5_PWM_CHANNEL_ID,&fan5_pwm_channel)
	return;
}

unsigned int get_FANSpeed(void)
{
	return(FANSpeed);
}

