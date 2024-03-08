################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include vendor/common/flash/subdir.mk
-include vendor/common/calibration/subdir.mk
-include vendor/UART_Demo/subdir.mk
-include drivers/flash/subdir.mk
-include drivers/subdir.mk
-include common/subdir.mk
-include boot/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := B85_Driver_Demo
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
LST += \
UART_Demo.lst \

FLASH_IMAGE += \
UART_Demo.bin \

SIZEDUMMY += \
sizedummy \


# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: B85_Driver_Demo.elf secondary-outputs

# Tool invocations
B85_Driver_Demo.elf: $(OBJS) $(USER_OBJS) makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: TC32 C Linker'
	tc32-elf-ld --gc-sections -L"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\chip\B85\drivers\lib" -T ../boot.link -o"B85_Driver_Demo.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

UART_Demo.lst: B85_Driver_Demo.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: TC32 Create Extended Listing'
	tc32-elf-objdump -x -D -l -S B85_Driver_Demo.elf  >"UART_Demo.lst"
	@echo 'Finished building: $@'
	@echo ' '

UART_Demo.bin: B85_Driver_Demo.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Create Flash image (binary format)'
	tc32-elf-objcopy -O binary B85_Driver_Demo.elf  "UART_Demo.bin"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: B85_Driver_Demo.elf makefile objects.mk $(OPTIONAL_TOOL_DEPS)
	@echo 'Invoking: Print Size'
	tc32-elf-size -t B85_Driver_Demo.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(LST)$(SIZEDUMMY) B85_Driver_Demo.elf
	-@echo ' '

post-build:
	-"E:\b85m_sdk_v1.7.0\telink_b85m_driver_sdk\project\tlsr_tc32\B85/../../../tools/tl_check_fw_tool/tl_check_fw.sh"   UART_Demo   B85_Driver_Demo
	-@echo ' '

secondary-outputs: $(LST) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
