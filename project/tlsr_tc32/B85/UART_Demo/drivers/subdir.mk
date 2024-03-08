################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/adc.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/aes.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/analog.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/audio.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/bsp.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/clock.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/emi.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/flash.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/gpio.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/i2c.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/lpc.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/printf.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/qdec.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/s7816.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/spi.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/timer.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/uart.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/usbhw.c \
E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/watchdog.c 

OBJS += \
./drivers/adc.o \
./drivers/aes.o \
./drivers/analog.o \
./drivers/audio.o \
./drivers/bsp.o \
./drivers/clock.o \
./drivers/emi.o \
./drivers/flash.o \
./drivers/gpio.o \
./drivers/i2c.o \
./drivers/lpc.o \
./drivers/printf.o \
./drivers/qdec.o \
./drivers/s7816.o \
./drivers/spi.o \
./drivers/timer.o \
./drivers/uart.o \
./drivers/usbhw.o \
./drivers/watchdog.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/adc.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/adc.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/aes.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/aes.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/analog.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/analog.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/audio.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/audio.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/bsp.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/bsp.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/clock.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/clock.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/emi.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/emi.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/flash.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/flash.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/gpio.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/gpio.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/i2c.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/i2c.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/lpc.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/lpc.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/printf.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/printf.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/qdec.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/qdec.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/s7816.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/s7816.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/spi.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/spi.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/timer.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/timer.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/uart.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/uart.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/usbhw.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/usbhw.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

drivers/watchdog.o: E:/b85m_sdk_v1.7.0/telink_b85m_driver_sdk/chip/B85/drivers/watchdog.c drivers/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\calibration" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\demo\vendor\common\B85\flash" -I"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\common" -DMCU_CORE_B85=1 -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


