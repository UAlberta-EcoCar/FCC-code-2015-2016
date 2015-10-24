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
../CLKconfig.cpp \
../FuelCell_Outputs.cpp \
../FCC-code_V1.cpp \
../FuelCell_Inputs.cpp \
../myADC.cpp \
../myPWM.cpp \
../mySerial.cpp \
../myTIMER.cpp


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
CLKconfig.o \
FuelCell_Outputs.o \
FCC-code_V1.o \
FuelCell_Inputs.o \
myADC.o \
myPWM.o \
mySerial.o \
myTIMER.o

OBJS_AS_ARGS +=  \
CLKconfig.o \
FuelCell_Outputs.o \
FCC-code_V1.o \
FuelCell_Inputs.o \
myADC.o \
myPWM.o \
mySerial.o \
myTIMER.o

C_DEPS +=  \
CLKconfig.d \
FuelCell_Outputs.d \
FCC-code_V1.d \
FuelCell_Inputs.d \
myADC.d \
myPWM.d \
mySerial.d \
myTIMER.d

C_DEPS_AS_ARGS +=  \
CLKconfig.d \
FuelCell_Outputs.d \
FCC-code_V1.d \
FuelCell_Inputs.d \
myADC.d \
myPWM.d \
mySerial.d \
myTIMER.d

OUTPUT_FILE_PATH +=FCC-code_V1.elf

OUTPUT_FILE_PATH_AS_ARGS +=FCC-code_V1.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler

















./%.o: .././%.cpp
	@echo Building file: $<
	@echo Invoking: AVR32/GNU C++ Compiler : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-g++.exe$(QUOTE) -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\UC3C_DFP\1.0.42\include"  -O1 -ffunction-sections -masm-addr-pseudos -g3 -Wall -mpart=uc3c164c -c -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
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

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR32/GNU Linker : 4.4.7
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-g++.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="FCC-code_V1.map" -Wl,--start-group  -Wl,--end-group -L"C:\Users\Reegan\Documents\GitHub"  -Wl,--gc-sections --rodata-writable -Wl,--direct-data -mpart=uc3c164c  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature "FCC-code_V1.elf" "FCC-code_V1.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "FCC-code_V1.elf" "FCC-code_V1.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objdump.exe" -h -S "FCC-code_V1.elf" > "FCC-code_V1.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "FCC-code_V1.elf" "FCC-code_V1.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr32\avr32-gnu-toolchain\bin\avr32-size.exe" "FCC-code_V1.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "FCC-code_V1.elf" "FCC-code_V1.a" "FCC-code_V1.hex" "FCC-code_V1.lss" "FCC-code_V1.eep" "FCC-code_V1.map" "FCC-code_V1.srec" "FCC-code_V1.usersignatures"
	