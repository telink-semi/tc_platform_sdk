################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/demo/vendor/common/B85/flash/zb_flash_ctrl.c 

OBJS += \
./vendor/common/flash/zb_flash_ctrl.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/flash/zb_flash_ctrl.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/demo/vendor/common/B85/flash/zb_flash_ctrl.c vendor/common/flash/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


