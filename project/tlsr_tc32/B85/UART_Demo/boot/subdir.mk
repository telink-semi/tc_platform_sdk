################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/boot/cstartup_copy_ramcode.S \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/boot/cstartup_flash.S \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/boot/cstartup_sram.S 

OBJS += \
./boot/cstartup_copy_ramcode.o \
./boot/cstartup_flash.o \
./boot/cstartup_sram.o 


# Each subdirectory must supply rules for building sources it contributes
boot/cstartup_copy_ramcode.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/boot/cstartup_copy_ramcode.S boot/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 CC/Assembler'
	tc32-elf-gcc -DMCU_STARTUP_FLASH -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

boot/cstartup_flash.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/boot/cstartup_flash.S boot/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 CC/Assembler'
	tc32-elf-gcc -DMCU_STARTUP_FLASH -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

boot/cstartup_sram.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/boot/cstartup_sram.S boot/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 CC/Assembler'
	tc32-elf-gcc -DMCU_STARTUP_FLASH -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


