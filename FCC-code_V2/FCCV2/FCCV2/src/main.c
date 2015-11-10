/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application templat
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal  function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include "asf.h"
#include "gpio.h"
#include "pwm4.h"
#include "scif_uc3c.h"
#define SERVO_PWM_PIN AVR32_PWM_PWMH_3_PIN
#define SERVO_PWM_FUNCTION AVR32_PWM_PWMH_3_FUNCTION
#define SERVO_PWM_CHANNEL_ID 3
#define FAN_PWM_PIN AVR32_PWM_PWMH_2_PIN
#define FAN_PWM_FUNCTION AVR32_PWM_PWMH_3_FUNCTION
#define FAN_PWM_CHANNEL_ID 2



// Start oscillator and enable PLL0, sourced by RC8M
static void local_start_highfreq_clock(void)
{
	const scif_pll_opt_t opt = {
		.osc = SCIF_GCCTRL_RC8M,     //"The PLLs can take either Oscillator 0, Oscillator 1 or 8MHz RC Oscillator (RC8M) as reference clock"
		.lockcount = 16,      // lockcount in main clock for the PLL wait lock
		.div = 0,             // PLLDIV=0 in the formula
		.mul = 6,             // PLLMUL=4
		.pll_div2 = 1,        // pll_div2 Divide the PLL output frequency by 2 (this settings does not change the FVCO value)
		.pll_wbwdisable = 0,  // pll_wbwdisable 1 Disable the Wide-Bandith Mode (Wide-Bandwith mode allow a faster startup time and out-of-lock time). 0 to enable the Wide-Bandith Mode.
		.pll_freq = 1,        // Set to 1 for VCO frequency range 80-180MHz, set to 0 for VCO frequency range 160-240Mhz. (VCO = 80 MHz in this case)
	};
	scif_start_rc8M();

	/* Setup PLL0 on RC8M, mul=7 ,no divisor, lockcount=16, ie. (8Mhzx(PLLMUL+1)*2 / 2 = 40 MHz output see page 100 of datasheet */
	scif_pll_setup(SCIF_PLL0, &opt); // lockcount in main clock for the PLL wait lock

	/* Enable PLL0 */
	scif_pll_enable(SCIF_PLL0);

	/* Wait for PLL0 locked */
	scif_wait_for_pll_locked(SCIF_PLL0) ;
}

// Start PWM generic clock input
static void pwm_start_gc(void)
{
	//PWM GENERIC CLOCK: source:PLL0 40MHz   divider: 40Mhz / (2*(199+1)) = 100KHz
	scif_gc_setup(AVR32_SCIF_GCLK_PWM,SCIF_GCCTRL_PLL0,AVR32_SCIF_GC_NO_DIV_CLOCK,0);
	// Now enable the generic clock
	scif_gc_enable(AVR32_SCIF_GCLK_PWM);
}

pwm_opt_t pwm_opt;                // PWM option config.

//servo channel configuration
avr32_pwm_channel_t servo_pwm_channel = {
	{0}, // cmr
	{0}, // cdty
	{0}, // cdtyupd
	{0}, // cprd
	{0}, // cprdupd
	{0}, // ccnt
	{0}, // dt
	{0}};// dtupd  ;  servo channel config.

	//fan channel configuration
	avr32_pwm_channel_t fan_pwm_channel = {
		{0}, // cmr
		{0}, // cdty
		{0}, // cdtyupd
		{0}, // cprd
		{0}, // cprdupd
		{0}, // ccnt
		{0}, // dt
		{0}};// dtupd  ;  fan channel config.


		unsigned int channel_id;

int main(void)
		{
			// Start Main Clock On external 16MHz Oscillator why?
			
			// Start PLL for PWM
			local_start_highfreq_clock();
			
			// Start Enable Generic Clock with PLL as source clock
			pwm_start_gc();

			channel_id = FAN_PWM_CHANNEL_ID;  //what about servo channel?
			
			//set pins to pwm function
			gpio_enable_module_pin(SERVO_PWM_PIN, SERVO_PWM_FUNCTION);
			gpio_enable_module_pin(FAN_PWM_PIN, FAN_PWM_FUNCTION);
			
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

			// Update the period
			pwm_update_period_value(10);



			// With these settings, the output waveform period will be :
			// (56MHz)/20 == 2.8MHz == (MCK/prescaler)/period, with MCK == 56MHz,
			// prescaler == 1, period == 20.

			//fan Channel configuration
			fan_pwm_channel.CMR.dte   = 1;        // Enable Deadtime for complementary Mode
			fan_pwm_channel.CMR.dthi  = 1;        // Deadtime Inverted on PWMH
			fan_pwm_channel.CMR.dtli  = 0;        // Deadtime Not Inverted on PWML
			fan_pwm_channel.CMR.ces   = 0;        // 0/1 Channel Event at the End of PWM Period
			fan_pwm_channel.CMR.calg  = PWM_MODE_LEFT_ALIGNED;       // Channel mode.
			fan_pwm_channel.CMR.cpol  = PWM_POLARITY_LOW;            // Channel polarity.
			fan_pwm_channel.CMR.cpre  = 2;           // Channel prescaler divide by 4. = 25KHz? see page 1036 of datasheet
			fan_pwm_channel.cdty      = 5;       // Channel duty cycle, should be < CPRD.
			fan_pwm_channel.cprd      = 20;       // Channel period.




			pwm_channel_init(FAN_PWM_CHANNEL_ID, &fan_pwm_channel); // Set channel configuration to channel 2
			pwm_start_channels((1 << FAN_PWM_CHANNEL_ID));  // Start channel 2.
			while(1);
		}

		void SERVOUpdate(void)
		{
			//this will probably be needed
			//pwm_update_channel()
		}

		void FANUpdate(void)
		{
			//this will probably be needed
			//pwm_update_channel()
		}