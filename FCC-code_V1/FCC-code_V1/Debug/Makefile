################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../CLKconfig.c \
../FCC-code_V1.cpp \
../FuelCell_Inputs.c \
../myADC.c \
../myPWM.c \
../mySerial.c \
../myTIMER.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
CLKconfig.o \
FCC-code_V1.o \
FuelCell_Inputs.o \
myADC.o \
myPWM.o \
mySerial.o \
myTIMER.o

OBJS_AS_ARGS +=  \
CLKconfig.o \
FCC-code_V1.o \
FuelCell_Inputs.o \
myADC.o \
myPWM.o \
mySerial.o \
myTIMER.o

C_DEPS +=  \
CLKconfig.d \
FCC-code_V1.d \
FuelCell_Inputs.d \
myADC.d \
myPWM.d \
mySerial.d \
myTIMER.d

C_DEPS_AS_ARGS +=  \
CLKconfig.d \
FCC-code_V1.d \
FuelCell_Inputs.d \
myADC.d \
myPWM.d \
mySerial.d \
myTIMER.d

OUTPUT_FILE_PATH +=libFCC-code_V1.a

OUTPUT_FILE_PATH_AS_ARGS +=libFCC-code_V1.a

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler















./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR32/GNU C++ Compiler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR32 GCC\Native\3.4.1067\avr32-gnu-toolchain\bin\avr32-gcc.exe$(QUOTE)  -x c -DDEBUG  -O1 -ffunction-sections -masm-addr-pseudos -g3 -Wall -mpart=uc3c2512c -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR32/GNU C++ Compiler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR32 GCC\Native\3.4.1067\avr32-gnu-toolchain\bin\avr32-g++.exe$(QUOTE) -DDEBUG  -O1 -ffunction-sections -masm-addr-pseudos -g3 -Wall -mpart=uc3c2512c -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)


$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP)
	@echo Building target: $@
	@echo Invoking: AVR32/GNU Archiver : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Atmel Toolchain\AVR32 GCC\Native\3.4.1067\avr32-gnu-toolchain\bin\avr32-ar.exe$(QUOTE) -r  -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) 
	@echo Finished building target: $@
	
	




# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "libFCC-code_V1.elf" "libFCC-code_V1.a" "libFCC-code_V1.hex" "libFCC-code_V1.lss" "libFCC-code_V1.eep" "libFCC-code_V1.map" "libFCC-code_V1.srec" "libFCC-code_V1.usersignatures"
	