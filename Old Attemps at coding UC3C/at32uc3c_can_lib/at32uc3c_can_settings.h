
#ifndef AT32UC3C_H
#define AT32UC3C_H

//for now only support for one channel at a time
#define CHANNEL_NUM 0 //board uses channel 0
#define NUM_BUFFERS 4
#define NUM_RX 2



//lets start with 250 kbps settings

//GCLK_CANIF = GCLK 1
//AVR32_SCIF.gcctrl[1] |= (0 << AVR32_SCIF_GCCTRL_OSCSEL); //clock source is system clock (120MHz)
//AVR32_SCIF.gcctrl[1] |= (1 << AVR32_SCIF_GCCTRL_DIVEN)|(9 << AVR32_SCIF_GCCTRL_DIV); //clock divider enabled. //clock = source clock /(2*(DIV+1). //(DIV is 8 bits max)
//Pgclk_canif = 1/(12000000hz)
//CANCGF.PRES = 47
//TQ = (CANCGF.PRES+1)xPgclk_canif
//TQ = 4(10)^-6
//AVR32_CANIF[1].cancfg |= (47 << AVR32_CANIF_CANCFG_PRS); //47 + 1 prescaler
//SYNC_SEG = 1
//PROP_SEG = 1
//PHASE_SEG1 = 2+1 = 3 
//AVR32_CANIF[1].cancfg |= (2 << AVR32_CANIF_CANCFG_PHS1);
//PHASE_SEG2 = 2+1 = 3
//AVR32_CANIF[1].cancfg |= (2 << AVR32_CANIF_CANCFG_PHS1);

//combine everything
#define GCLK_CANIF_Setting ((0 << AVR32_SCIF_GCCTRL_OSCSEL)|(1 << AVR32_SCIF_GCCTRL_DIVEN)|(9 << AVR32_SCIF_GCCTRL_DIV))
#define CANCFG_Setting ((2 << AVR32_CANIF_CANCFG_PHS1)|(2 << AVR32_CANIF_CANCFG_PHS1)|(47 << AVR32_CANIF_CANCFG_PRS))













#endif