################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./04072014main.obj" \
"./07072014main_wdomu.obj" \
"./10072014main.obj" \
"./11072014mainNIEZMIENIAC.obj" \
"./26052014main.obj" \
"./27052014main.obj" \
"./27062014 main.obj" \
"./28062014main_V1.obj" \
"./28062014main_V2.obj" \
"./28062014main_v3.obj" \
"./290520014_v2_main.obj" \
"./29052014main.obj" \
"./30052014main.obj" \
"./ADCW2.obj" \
"./DZIALANIEZMIENIACmain.obj" \
"./ds3231.obj" \
"./fifo.obj" \
"./lcd44780_LP.obj" \
"./main.obj" \
"./mathFunctions.obj" \
"./uart.obj" \
$(GEN_CMDS__FLAG) \
-l"libc.a" \
-l"C:/ti/TivaWare_C_Series-1.0/examples/boards/ek-tm4c123gxl/hello/ccs/../../../../../driverlib/ccs/Debug/driverlib.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
EnergyMeter.out \

EXE_OUTPUTS__QUOTED += \
"EnergyMeter.out" \


# All Target
all: EnergyMeter.out

# Tool invocations
EnergyMeter.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --gcc --define=ccs="ccs" --define=PART_TM4C123GH6PM --define=TARGET_IS_BLIZZARD_RB1 --diag_warning=225 --display_error_number --diag_wrap=off --gen_func_subsections=on --ual -z --stack_size=512 -m"hello_ccs.map" --heap_size=0 -i"D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/lib" -i"D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="EnergyMeter_linkInfo.xml" --rom_model -o "EnergyMeter.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "04072014main.pp" "07072014main_wdomu.pp" "10072014main.pp" "11072014mainNIEZMIENIAC.pp" "26052014main.pp" "27052014main.pp" "27062014 main.pp" "28062014main_V1.pp" "28062014main_V2.pp" "28062014main_v3.pp" "290520014_v2_main.pp" "29052014main.pp" "30052014main.pp" "ADCW2.pp" "DZIALANIEZMIENIACmain.pp" "ds3231.pp" "fifo.pp" "lcd44780_LP.pp" "main.pp" "mathFunctions.pp" "uart.pp" 
	-$(RM) "04072014main.obj" "07072014main_wdomu.obj" "10072014main.obj" "11072014mainNIEZMIENIAC.obj" "26052014main.obj" "27052014main.obj" "27062014 main.obj" "28062014main_V1.obj" "28062014main_V2.obj" "28062014main_v3.obj" "290520014_v2_main.obj" "29052014main.obj" "30052014main.obj" "ADCW2.obj" "DZIALANIEZMIENIACmain.obj" "ds3231.obj" "fifo.obj" "lcd44780_LP.obj" "main.obj" "mathFunctions.obj" "uart.obj" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-"D:/ti/ccsv6/utils/tiobj2bin/tiobj2bin" "EnergyMeter.out" "EnergyMeter.bin" "D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armofd" "D:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.6/bin/armhex" "D:/ti/ccsv6/utils/tiobj2bin/mkhex4bin"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets

