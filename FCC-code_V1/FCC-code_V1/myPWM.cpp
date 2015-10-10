#include "myPWM.h"
#include <avr32/io.h>

// MCU has multiple PWM channels I plan on making functions to activate them all individually here
//Currently need 1 signal for servo and signal for fans
//fan PWM signal is 25KHz according to wikipedia
//Servo is 500Hz according to last time I used one.

void PWMInit(void)
{
    //turn PWM on. Page 963 of datasheet
	
	//Unlock user interface by writing the WPCMD field in the WPCR Register.
	AVR32_PWM.wpcr &= ~3;
	
	//Configuration of the clock generator (DIVA, PREA, DIVB, PREB, CLKSEL in the CLK register
	//if required). After writing CLKSEL to a new value, no write in any PWM registers must be
	//attempted before a delay of 2 master clock periods (CLK_PWM). This is the time needed by
	//the PWM to switch the internal clock (CCK).
	
	//configure GCLK_PWM = generic clock 4 //page 118 of data sheet
	//disable generic clock
	AVR32_SCIF.gcctrl[4] = 0;
	//clock source
	AVR32_SCIF.gcctrl[4] |= (2 << AVR32_SCIF_GCCTRL_OSCSEL); //8/1 MHz clock
	//Divider: one to DIVEN and write to DIV
	//100KHz is a nice number
	//8MHz / 100KHz = 80 = 2*(79+1);
	AVR32_SCIF.gcctrl[4] |= (79 << AVR32_SCIF_GCCTRL_DIV);
	AVR32_SCIF.gcctrl[4] |= (1 << AVR32_SCIF_GCCTRL_DIVEN);
	//enable generic clock CEN bit GCCTRL reg
	AVR32_SCIF.gcctrl[4] |= (1 << AVR32_SCIF_GCCTRL_CEN);
	//set PWM to run off general clock
	AVR32_PWM.clk |= (1 << AVR32_PWM_CLK_CLKSEL); //select generic clock
	//do something else for 2 CLK_PWM cycles (not sure what that is)
	
	//need 25KHz
	//need around 500Hz
	
	//Selection of the clock for each channel (CPRE field in the CMRx register)
	
	//Configuration of the waveform alignment for each channel (CALG field in the CMRx register)

	//Selection of the counter event selection (if CALG=1) for each channel (CES field in the CMRx
	//register)

	//Configuration of the output waveform polarity for each channel (CPOL in the CMRx register)

	//Configuration of the period for each channel (CPRD in the CPRDx register). Writing in
	//CPRDx register is possible while the channel is disabled. After validation of the channel, the
	//user must use CPRDUPDx register to update CPRDx as explained below.

	//Configuration of the duty-cycle for each channel (CDTY in the CDTYx register). Writing in
	//CDTYx register is possible while the channel is disabled. After validation of the channel, the
	//user must use CDTYUPDx register to update CDTYx as explained below.

	//Configuration of the dead-time generator for each channel (DTH and DTL in DTx) if enabled
	//(DTE bit in the CMRx register). Writing in the DTx register is possible while the channel is
	//disabled. After validation of the channel, the user must use DTUPDx register to update DTx

	//Selection of the synchronous channels (SYNCx in the SCM register)

	//Selection of the moment when the WRDY bit and the corresponding PDCA transfer request
	//are set (PTRM and PTRCS in the SCM register)

	//Configuration of the update mode (UPDM in the SCM register)

	//Configuration of the update period (UPR in the SCUP register) if needed.

	//Configuration of the comparisons (CMPxV and CMPxM).

	//Configuration of the event lines (ELxMR).

	//Configuration of the fault inputs polarity (FPOL in FMR)

	//Configuration of the fault protection (FMOD and FFIL in FMR, FPV and FPE1)

	//Enable of the interrupts (writing CHIDx and FCHIDx in IER1 register, and writing WRDYE,
	//UNRE, CMPMx and CMPUx in IER2 register)

	//Enable of the channels (writing CHIDx in the ENA register)
	
	//Set GPIO multiplexer
}
void SetPWM(unsigned int PWMInput)
{
	//adjust the PWM duty cycle
}
