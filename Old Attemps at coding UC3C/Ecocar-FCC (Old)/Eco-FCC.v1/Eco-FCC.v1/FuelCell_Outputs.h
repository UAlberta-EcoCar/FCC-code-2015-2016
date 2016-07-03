#ifndef FuelCell_Outputs
#define FuelCell_Outputs

void SetDigitalOut(unsigned int GPIOPort, unsigned int GPIOPin);
void OutputsInit(void);
void SetFAN_OP(unsigned int FanSetting);
void SetSRV_OP(unsigned int ServoSetting);
void LED0(unsigned int LEDInput);
void LED1(unsigned int LEDInput);
void LED2(unsigned int LEDInput);
void LED3(unsigned int LEDInput);
void OP1(unsigned int Input);
void OP2(unsigned int Input);
void OP3(unsigned int Input);
void OP4(unsigned int Input);
void OP5(unsigned int Input);
void OP6(unsigned int Input);
void OP7(unsigned int Input);
void OP8(unsigned int Input);
void OP9(unsigned int Input);
void OP10(unsigned int Input);

#endif