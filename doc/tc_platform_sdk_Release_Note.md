## V3.1.0

### Version
* SDK Version: tc_platform_sdk V3.1.0
* Chip Version
  - B80:  TLSR8208/TLSR8373
  - B80B: TLSR8208H
  - B85:  TLSR825x/TLSR8359
  - B87:  TLSR827x/TLSR8355
  - TC321X(A0)
* Hardware EVK Version
  - B80:    C1T261A30_V1_1
  - B80B:   C1T321A30_V1_0
  - B85:    C1T139A30_V1_2
  - B87:    C1T197A30_V1_1
  - TC321X: C1T357A20_V1_1

* Toolchain Version
  - B80, B80B, B85, B87, TC321X(A0): TC32 ELF GCC4.3 ( IDE: [Telink IDE](https://wiki.telink-semi.cn/wiki/IDE-and-Tools/IDE-for-TLSR8-Chips/)/ [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip) )

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

### Bug Fixes
* **gpio**
  * (TC321X) Fixed risc3 irq non-interrupt response issue.(merge_requests/@679)
  * (TC321X) Fixed wrong enumeration of PF3 pin in GPIO_PinTypeDef.(merge_requests/@696)
  * (TC321X) Fixed the 4mA function exception on pins PB4~7 and PC0~7 in the gpio_set_data_strength function, and optimized the interface in software.(merge_requests/@696)
* **sys**
  * (B85)Solves the problem that when the system clock is greater than 24M and the power supply is low in a high temperature environment, a code transfer error may occur when the reboot is called soon after awakening, resulting in chip crash.(merge_requests/@700)

### BREAKING CHANGES
* **pll**
  * (B87)Customers cannot use the bit that uses the PM_ANA_REG_POWER_ON_CLR_BUF0[bit2] (0x3a[2]) flag to indicate whether a restart caused by a PLL exception occurred.(merge_requests/@691)

### Features
* **BQB_EMI_Demo**
  * (TC321X) Modified the BQB configuration scheme to adapt to BDT/BQB_Tool.(merge_requests/@684)
* **rf**
  * (TC321X) Update the values in the RF_PowerTypeDef structure and remove the TX power: 	RF_POWER_P11p25dBm,RF_POWER_P11p00dBm,RF_POWER_P10p50dBm,RF_POWER_P10p25dBm level.(merge_requests/@684)
* **flash**
  * (TC321X) flash P25Q16SU test passed, open flash driver. (merge_requests/@602)
  * (TC321X) flash GD25LD40EGEG test passed, open flash driver.(merge_requests/@655) 
  
### Refactoring
* **rf**
  * (TC321X) Updated the RF configuration and adjusted the AGC table accordingly to ensure the accuracy of RSSI measurements.(merge_requests/@699)

### 版本
* SDK 版本: tc_platform_sdk V3.1.0
* 芯片版本
  - B80:  TLSR8208/TLSR8373
  - B80B: TLSR8208H
  - B85:  TLSR825x/TLSR8359
  - B87:  TLSR827x/TLSR8355
  - TC321X(A0)
* 硬件评估板版本
  - B80:    C1T261A30_V1_1
  - B80B:   C1T321A30_V1_0
  - B85:    C1T139A30_V1_2
  - B87:    C1T197A30_V1_1
  - TC321X: C1T357A20_V1_1

* 工具链版本
  - B80, B80B, B85, B87, TC321X(A0): TC32 ELF GCC4.3 ( IDE: [Telink IDE](https://wiki.telink-semi.cn/wiki/IDE-and-Tools/IDE-for-TLSR8-Chips/)/ [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip) )

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

### Bug Fixes
* **gpio**
  * (TC321X) 修复risc3 irq 无中断响应的问题。(merge_requests/@679)
  * (TC321X) 修复了 GPIO_PinTypeDef 中 PF3 引脚枚举错误的问题。(merge_requests/@696)
  * (TC321X) 修复了 gpio_set_data_strength 函数中 PB4~7 和 PC0~7 引脚的 4mA 功能异常问题，并对该接口进行了软件优化。(merge_requests/@696)
* **sys** 
  * (B85)解决了在高温环境下，供电比较低时，系统时钟大于24M时，唤醒后很快调用reboot时，可能发生搬代码错误，导致芯片死机的问题。(merge_requests/@700)

### BREAKING CHANGES
* **pll**
  * (B87)占用PM_ANA_REG_POWER_ON_CLR_BUF0[bit2]（0x3a[2]）标志是否发生过PLL异常导致的重启，客户不能使用这个bit。(merge_requests/@691)

### Features
* **BQB_EMI_Demo**
  * (TC321X) 修改了BQB的配置方案以适配BDT/BQB_Tool。(merge_requests/@684)
* **rf**
  * (TC321X) 更新RF_PowerTypeDef结构体中的值,并删除TX power:RF_POWER_P11p25dBm,RF_POWER_P11p00dBm,RF_POWER_P10p50dBm,RF_POWER_P10p25dBm 档位。(merge_requests/@684)
* **flash**
  * (TC321X) flash P25Q16SU测试通过，开放flash驱动。(merge_requests/@602)
  * (TC321X) flash GD25LD40EGEG测试通过，开放flash驱动。(merge_requests/@655) 

### Refactoring
* **rf**
  * (TC321X) 更新 RF 配置并同步调整 AGC table，以确保 RSSI 测量值的准确性。(merge_requests/@699)

---
## V3.0.1

### Version
* SDK Version: tc_platform_sdk V3.0.1
* Chip Version
  - TLSR8208(B80) (A1/A4), TLSR8208 B(B80B) (A0/A1), B85, B87,TC321X(A0)
* Hardware EVK Version
  - B80:  C1T261A30_V1_1
  - B80B: C1T321A30_V1_0
  - B85:  C1T139A30_V1_2
  - B87:  C1T197A30_V1_1
  - TC321X:  C1T357A20_V1_1

* Toolchain Version
  - TLSR8208(B80) (A1/A4), TLSR8208 B(B80B) (A0/A1), B85, B87,TC321X(A0): TC32 ELF GCC4.3 ( IDE: [Telink IDE](https://wiki.telink-semi.cn/wiki/IDE-and-Tools/IDE-for-TLSR8-Chips/)/ [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip) )

### Bug Fixes

* **calibration**
  * (B80/B80B/B85/B87/TC321X) Modified the logic of judging frequency_offset_value in user_calib_freq_offset interface to ensure that it is consistent with the logic of RF frequency calibration value written to flash by the chip fixture frequency calibration value, in order to solve the problem that the chip fixture frequency calibration value does not take effect. This issue affects all SDKs and applications that require RF frequency calibration.

### BREAKING CHANGES

* N/A

### Features

* N/A

### Refactoring

* N/A

### Performance Improvements

* N/A

### 版本
* SDK 版本: tc_platform_sdk V3.0.1
* 芯片版本
  - TLSR8208(B80) (A1/A4), TLSR8208 B(B80B) (A0/A1), B85, B87,TC321X(A0)
* 硬件评估板版本
  - B80:  C1T261A30_V1_1
  - B80B: C1T321A30_V1_0
  - B85:  C1T139A30_V1_2
  - B87:  C1T197A30_V1_1
  - TC321X:  C1T357A20_V1_1

* 工具链版本
  - TLSR8208(B80) (A1/A4), TLSR8208 B(B80B) (A0/A1), B85, B87,TC321X(A0): TC32 ELF GCC4.3 ( IDE: [Telink IDE](https://wiki.telink-semi.cn/wiki/IDE-and-Tools/IDE-for-TLSR8-Chips/)/ [TelinkIoTStudio_V2024.8](https://wiki.telink-semi.cn/tools_and_sdk/Tools/IoTStudio/TelinkIoTStudio_V2024.8.zip) )


### Bug Fixes

* **calibration**
  * (B80/B80B/B85/B87/TC321X) 修改了user_calib_freq_offset接口中判断frequency_offset_value的逻辑，使其保证和芯片夹具频偏校准值写入flash中的RF频偏校准值逻辑一致，以解决芯片夹具频偏校准值不生效的问题。此问题影响所有需要进行RF频偏校准的SDK和应用。

### BREAKING CHANGES

* N/A

### Features

* N/A

### Refactoring

* N/A

### Performance Improvements

* N/A

---

## V3.0.0

### Version

* SDK version: tc_platform_sdk V3.0.0
* This version of SDK supports TLSR8208 (B80) (A1/A4), TLSR8208 B version (B80B) (A0/A1), B85, B87,TC321X(A0) chips.
* The default configuration of LEDs and KEYs match the following hardware revisions:
*	B80	 	C1T261A30_V1_1
*	B80B	C1T321A30_V1_0
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2
*	TC321X  C1T357A20_V1_1

### BREAKING CHANGES

* **usb**
  * (B80/B80B/B85/B87) swire_through_usb_dp_en() and swire_through_usb_dp_dis() were reorganized as dp_through_swire_en(bool dp_through_swire).(merge_requests/@569)
* **gpio**
  * (B80/B80B):Fixed risc spelling errors involving function names such as: gpio_get_new_risc_irq_status, gpio_clr_new_risk_irq_status, gpio_set_new_risc_irq_mask, gpio_clr_new_risk_irq_mask.(merge_requests/@588)
  
### Bug Fixes

* **rf**
  * (B80/B80B/B85/B87) Fixed the issue where calling the reset_baseband function caused other functions to be turned on by default.(merge_requests/@581)
  * (B80/B80B)Fix the issue of inaccurate RSSI values.(merge_requests/@652)
* **gpio**
  * (B80/B80B/B85/B87) Fix gpio_get_irq_status() return value wrong type issue.(merge_requests/@594)
* **pm**
  * (B80/B80B/B85/B87)Fix the issue where BIT(7) of the analog register 0x44 is mistakenly set to 1 in abnormal conditions when the wake-up source is zero, causing the cpu_sleep_wakeup_32k_rc() function to return an abnormal value.(merge_requests/@623)

### Features

* **emi**
  * (B80/B80B/B85/B87)Add Adaptive Testing for Devices with PA Module.(merge_requests/@643)
* **stimer**
  * (B85)Add stimer mode.(merge_requests/@653)

### Refactoring
* **clock**
  * (B85)Restore system clock source back to 24M rc when entering cpu_wakeup_init to prevent abnormal reboots which may occur due to multiple calls to cpu_wakeup_init.(merge_requests/@562)
* **flash**
  * (B80)ZG25WD40B flash test passed, open flash driver.(merge_requests/@578)
  * (B80)Modify otp_block enum name error definition.(merge_requests/@578)
  * (B87)No longer supports flash TH25Q16UB.(merge_requests/@610)
  * (B80/B80B)flash P25Q40SU changed to P25D40SU.(merge_requests/@610)
* **rf**
  * (B80/B80B/B85/B87)Optimize the logic for obtaining calibration values in the user_calib_freq_offset interface.(merge_requests/@652)
* **BQB_EMI_Demo**
  * (B80/B80B/B85/B87)Turn off flash encryption.(merge_requests/@595)

### Performance Improvements

* N/A

### Note
  * (B80/B80B/B85/B87) Add usb_set_pin() interface, turn off dp_through_swire by default in cstartup.S. In USB application can choose to enable or disable dp_through_swire function, for compatibility usb_set_pin_en() is defined as usb_set_pin(1) and comments were added to cstartup.S.(merge_requests/@569)


### BREAKING CHANGES

* **usb**
  * (B80/B80B/B85/B87) swire_through_usb_dp_en() 和 swire_through_usb_dp_dis() 被重新整合为 dp_through_swire_en(bool dp_through_swire)。(merge_requests/@569)
* **gpio**
  * (B80/B80B):修复risc拼写错误问题，涉及函数名有：gpio_get_new_risc_irq_status、gpio_clr_new_risk_irq_status、gpio_set_new_risc_irq_mask、gpio_clr_new_risk_irq_mask。(merge_requests/@588)
  
### Bug Fixes

* **rf**
  * (B80/B80B/B85/B87) 修复调用 reset_baseband 函数引起其他功能默认打开的问题。(merge_requests/@581)
  * (B80/B80B)修复获取的RSSI不准问题。(merge_requests/@652)
* **gpio**
  * (B80/B80B/B85/B87)修复gpio_get_irq_status()返回值类型错误问题。(merge_requests/@594)
* **pm** 
  * (B80/B80B/B85/B87)修复模拟寄存器0x44在唤醒源为零的异常情况下，bit7会被误置为1导致cpu_sleep_wakeup_32k_rc()函数返回值异常的问题(merge_requests/@623)
    
### Features

* **emi**
  * (B80/B80B/B85/B87)添加带有PA模块的设备自适应测试功能。(merge_requests/@643)

### Refactoring

* **clock**
  * (B85)在进入cpu_wakeup_init时先将系统时钟源切换为24M rc，以防多次调用cpu_wakeup_init可能导致的异常reboot。(merge_requests/@562)
* **flash**
  * (B80)ZG25WD40B flash测试通过，开放flash驱动。(merge_requests/@578)
  * (B80)修改otp_block枚举名错误定义。(merge_requests/@578)
  * (B87)不再支持flash TH25Q16UB。(merge_requests/@610)
  * (B80/B80B)flash P25Q40SU 变更为 P25D40SU。(merge_requests/@610)
* **stimer**
  * (B85)增加了stimer mode。(merge_requests/@653)
* **rf**
  * (B80/B80B/B85/B87)优化user_calib_freq_offset接口校准值获取逻辑。(merge_requests/@652)
* **BQB_EMI_Demo**
  * (B80/B80B/B85/B87)关闭flash加密。(merge_requests/@595)

### Performance Improvements

* N/A

### Note
* (B80/B80B/B85/B87) 新增usb_set_pin()接口，在cstartup.S中默认关闭dp_through_swire功能。在USB应用中可以选择使能或不使能dp_through_swire功能，为了兼容性usb_set_pin_en()定义为usb_set_pin(1)，并在cstartup.S添加相应注释。(merge_requests/@569)

## V2.0.0

### Version

* SDK version: tc_platform_sdk V2.0.0
* This version of SDK supports TLSR8208 (B80) (A1/A4), TLSR8208 B version (B80B) (A0/A1), B85, B87 chips.
* The default configuration of LEDs and KEYs match the following hardware revisions:
*	B80	 	C1T261A30_V1_1
*	B80B	C1T321A30_V1_0
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2

### BREAKING CHANGES

* **flash**
  * (B80/B80B/B85/B87):In order to implement the bin firmware protection in driver sdk, we delete the flash_lock/flash_unlock swtich implementation scheme maintained by flash_demo, and for the sake of code simplicity and easy maintenance, we add the flash hal layer to implement the scheme with structure, add hal_flash.c/.h, common.c/.h and flash_comomon.c/.h file of each chip driver, and add the bin firmware protection function in the main of each demo.
* **sys**
  * (B80/B80B/B87)Change the xtal parameter to the cap parameter in the cpu_wakeup_init function. This parameter is used to configure whether the internal capacitor needs to be disabled. If the development board has already welded the capacitor on the outside, then the internal capacitor needs to be turned off; If the development board does not weld the capacitor on the outside, then the internal capacitor needs to be opened. If the corresponding operation is not possible, there may be two capacitors in parallel, or no capacitors. This has a great impact on the starting time and frequency of crystal vibration, which may cause reset and crash.
* **irq**
  * (B80/B80B/B85/B87)Rename function irq_clr_src2 to irq_clr_sel_src and change the related comment.

### Bug Fixes

* **spi_demo**
  * (B85/B87)Solved the issue of incorrect irq counts in the SPI slave demo when the master is not a single transmission (with both read and write), and updated the irq handler and comments.(merge_requests/@530)
* **pm**
  * (B80B) Solved the problem of high current in suspend mode and deep mode in the A0 version of the chip due to the damage of the transistor inside the chip caused by the pull-up abnormality of the USB DP pin (the A1 version of the chip does not have this problem).(merge_requests/@550)
* **USB**
  * (B80B)Fix USB RESET_STATUS not cleared resulting in non-repeatable tests.(merge_requests/@518)
* **flash**
  * (B80/B80B): Fix the definition of Flash_VoltageDef (VDD_F voltage with no load).(merge_requests/@548)

### Features

* **flash**
  * (B87)add comments to support two-supply flash GD25LD10E(Comparison has been made between the same mid flash GD25LD10C that the driver already exists, and it is confirmed that the flash driver does not need to be updated).
* **cstartup.S**
  * (B80/B80B/B85)Add the function that sets the watchdog capture_tick and starts it in cstartup.S.
* **pwm**
  * (B80/B80B/B85/B87)Add pwm_multi_start() interface to support pwm enabling multiple channels simultaneously.
* **usb**
  * (B80/B80B/B85/B87)Add swire_through_usb_dp_en() to enable swire communication via USB DP IO and swire_through_usb_dp_dis() to disable swire communication via USB.
* **spi**
  * (B80/B80B)Add spi dma full duplex interface: spi_master_write_read_dma_full_duplex.
* **flash**
  * (B80): add new flash ZG25WD40B(Untested)/GD25WD10EGIG.
  * (B80B): add new flash GD25WD10EGIG.
  * (B87): add new flash TH25Q16UB.  
* **calibration**
  * (B80): Add operation to flash ZG25WD40B to increase VDD_F to 2.25V maximum.

### Refactoring

* **watchdog**
  * (B80/B80B/B85/B87) Fixed comment error in the capture_tick formula of the wd_set_interval_ms() function.
* **pm**
  * (B80/B80B/B85/B87)Update the early wakeup time of SUSPEND sleep mode which impacted by add pm_wait_xtal_ready(), meanwhile adjusted the nop number of pm_wait_xtal_ready() to make sure the delay time is 40us.
* **USB_Demo**
  * (B80/B80B/B85/B87) Call interface irq_clr_sel_src() to clear the USB reset flag bit.
  * (B80/B80B/B85/B87) Call interface irq_get_src() to get the interrupt status.
* **spi**
  * (B80/B80B)Remove spi dma interface (spi_master_write_dma/ spi_master_write_read_dma) duplicate assignment operation to reg_dma_rdy0_h register (reg_dma_rdy0_h|= FLD_DMA_READY_9;) and SPI_MODE_ WRITE_AND_READ comment modification.
* **ALL_Demo**
    * (B80/B80B/B85/B87)Added common.c and common.h, put the definition of LED and KEY in common.h, defined platform_init interface in common.c, put the public initialization content in platform_init interface, and added the initialization content to set up pull-up for sws to prevent probabilistic crash.
    * (B80/B80B/B85/B87)In order to prevent gpio_set_func related functions from operating on a different set of GPIOs, change all demos where the function parameter is the logical or of multiple LEDs to a single LED parameter.
    * (B80/B80B/B85/B87)To clear up the warnings caused by warning flag -Wstrict-prototypes, the following demo files has been modified by adding a void parameter to the function without parameters:
      - ADC_Demo, AES_Demo, AUDIO_Demo, BQB_EMI_Demo, DUT_Demo, Debug_Demo, Display_Demo, Flash_Demo, GPIO_Demo, I2C_Demo, IO_Swire_Demo, IR_LEARN_Demo, Keyscan_Demo, LPC_Demo, MDEC_Demo, OTP_Demo, PKE_Demo, PM_Demo, PWM_Demo, QDEC_Demo, RF_AOA_Demo, RF_Demo, SPI_Demo, SRAM_Test, Swire_Demo, TIMER_Demo, TRNG_Demo, Test_Demo, UART_Demo, USB_Demo, s7816_Demo.

### Performance Improvements

* N/A

### Note

* N/A

### BREAKING CHANGES

* **flash**
  * (B80/B80B/B85/B87):为了driver sdk实现bin固件保护,删除flash_demo维护的flash_lock/flash_unlock swtich实现方案,为代码简洁以及方便维护,改为添加flash hal层以结构体实现方案,添加hal_flash.c/.h、common.c/.h和各个芯片driver的flash_comomon.c/.h文件,并在各个demo的main中添加bin固件保护功能.
* **sys**
  * (B80/B80B/B87)在cpu_wakeup_init函数中修改xtal参数为cap参数，该参数用于配置是否需要关闭内部电容。如果开发板已经在外部焊接了电容，那么就需要关掉内部电容；如果开发板没有在外部焊接电容，那么就需要打开内部电容。如果不能对应操作的话，就可能会出现两个电容并联，或者没有电容的情况。这样对晶振的起振时间和晶振的频率影响很大，可能会造成复位、死机等情况发生。
* **irq**
  * (B80/B80B/B85/B87)将函数 irq_clr_src2 重命名为 irq_clr_sel_src 并修改相关注释。

### Bug Fixes

* **spi_demo**
  * (B85/B87)解决了spi slave demo中，master 非单一传输情况下（有读有写），irq计数不正确的问题，更新了中断处理程序和注释。(merge_requests/@530)
* **pm**
  * (B80B)解决了芯片 A0 版本因 USB DP 引脚上拉异常导致芯片内部晶体管损坏引起的 suspend 模式和 deep 模式电流偏大的问题（芯片 A1 版本没有此问题）。(merge_requests/@550)
* **USB**
  * (B80B)修复 USB RESET_STATUS 未清除导致不能重复检测的问题。(merge_requests/@518)
* **flash**
  * (B80/B80B): 修复Flash_VoltageDef的档位定义（VDD_F无负载时的电压）。(merge_requests/@548)
  
### Features

* **flash**
  * (B87)添加注释以支持二供flash GD25LD10E(已对比驱动已有的同mid flash GD25LD10C，确认该flash驱动不需要更新)。
* **cstartup.S**
  * (B80/B80B/B85)在 cstartup.S 添加设置并使能 watchdog 功能。
* **pwm**
  * (B80/B80B/B85/B87)添加pwm_multi_start()接口以支持pwm同时使能多个通道。
* **usb**
  * (B80/B80B/B85/B87)新增swire_through_usb_dp_en()使能通过USB DP IO进行swire通信的功能，新增swire_through_usb_dp_dis()关闭通过USB进行swire通信的功能。
  * **spi**
  * (B80/B80B)添加 spi dma 全双工接口：spi_master_write_read_dma_full_duplex。
* **flash**
  * (B80): 添加新flash ZG25WD40B(未测试)/GD25WD10EGIG。
  * (B80B): 添加新flash GD25WD10EGIG。
  * (B87): 添加新flash TH25Q16UB。
* **calibration**
  * (B80): 添加对flash ZG25WD40B调高VDD_F至最高档2.25V的操作。

### Refactoring

* **watchdog**
  * (B80/B80B/B85/B87) 修复了 wd_set_interval_ms() 函数 capture_tick 计算公式的注释错误。
* **pm**
  * (B80/B80B/B85/B87)更新了由于新增pm_wait_xtal_ready()接口带来的SUSPEND模式下提前唤醒时间的变化，同时校准了pm_wait_xtal_ready()接口中的nop数量确保延时时间为40us。
* **USB_Demo**
  * (B80/B80B/B85/B87) 调用接口 irq_clr_sel_src() 清除 USB 复位标志位。
  * (B80/B80B/B85/B87) 调用接口 irq_get_src() 获取中断状态。
* **spi**
  * (B80/B80B)删除spi dma接口(spi_master_write_dma/ spi_master_write_read_dma)对reg_dma_rdy0_h寄存器重复赋值操作(reg_dma_rdy0_h|= FLD_DMA_READY_9;)和SPI_MODE_WRITE_AND_READ注释修改。
* **ALL_Demo**
  * (B80/B80B/B85/B87)添加了 common.c 和 common.h, 将 LED、KEY 的定义放在 common.h,在 common.c 定义 platform_init 接口，将公共的初始化内容放 platform_init 接口中，初始化内容新增了给 sws 设置上拉，以防概率性死机问题。
  * (B80/B80B/B85/B87)为了防止 gpio_set_func 相关函数操作的不是同一组 GPIO 导致的功能异常，将所有 demo 中函数参数为多个 LED 逻辑或的参数修改为单个 LED 参数。
  * (B80/B80B/B85/B87)为了处理由警告选项 -Wstrict-prototypes 带来的编译警告, 以下 demo 文件已经通过向无参函数声明中传入 void 进行了修改:
    - ADC_Demo, AES_Demo, AUDIO_Demo, BQB_EMI_Demo, DUT_Demo, Debug_Demo, Display_Demo, Flash_Demo, GPIO_Demo, I2C_Demo, IO_Swire_Demo, IR_LEARN_Demo, Keyscan_Demo, LPC_Demo, MDEC_Demo, OTP_Demo, PKE_Demo, PM_Demo, PWM_Demo, QDEC_Demo, RF_AOA_Demo, RF_Demo, SPI_Demo, SRAM_Test, Swire_Demo, TIMER_Demo, TRNG_Demo, Test_Demo, UART_Demo, USB_Demo, s7816_Demo。

### Performance Improvements

* N/A

### Note

* N/A

## V1.9.0

### Version

* SDK version: telink_b85m_platform_sdk V1.9.0.
* This version of SDK supports B80(A1), B80B, B85, B87 chips.
* The default configuration of LEDs and KEYs match the following hardware revisions:
*	B80	 	C1T261A30_V1_1
*	B80B	C1T321A30_V1_0
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2

### BREAKING CHANGES
* N/A

### Bug Fixes

* **s7816**
  * (B80B)the s78161 pin_clk cannot output clock properly, resulting in the ic card cold reset failure.(merge_requests/@512)
  * (B80B/B80)s7816_set_clk_pin: the pin_7816_clk enumeration type is changed to GPIO_PinTypeDef.(merge_requests/@512)
* **spi**
  * (B80B)Fix the problem that dma read/write function can't be used because the dma channel of spi is not enabled, related interfaces involved: spi_tx_dma_en(), spi_rx_dma_en(), spi_tx_dma_dis(), spi_rx_dma_dis().(merge_requests/@513)

### Features
* N/A
  
### Refactoring
* **spi**
   * (B80/B80B)Remove duplicate software operation (reg_dma_rdy0_h |= FLD_DMA_READY_9) in spi_master_write_dma_plus() interface.

### Performance Improvements

* N/A

### Note

* N/A

### 版本

* SDK版本: telink_b85m_platform_sdk V1.9.0。
* 此版本SDK支持B80(A1)、 B80B、 B85、B87芯片。
* LED和KEY的默认配置匹配以下硬件版本:
*	B80	 	C1T261A30_V1_1
*	B80B	C1T321A30_V1_0
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2

### BREAKING CHANGES
* N/A

### Bug Fixes

* **s7816**
  * (B80B)解决s78161 pin_clk无法正常输出时钟，导致无法正常冷复位ic卡;(merge_requests/@512)
  * (B80B/B80)s7816_set_clk_pin: pin_7816_clk枚举类型修改为GPIO_PinTypeDef;(merge_requests/@512)
* **spi**
  * (B80B) 修复spi的dma通道未使能导致dma读写函数不能使用问题，涉及的相关接口：spi_tx_dma_en()，spi_rx_dma_en()，spi_tx_dma_dis()，spi_rx_dma_dis()。(merge_requests/@513)

### Features
* N/A
  
### Refactoring
* **spi**
   * (B80/B80B)删除spi_master_write_dma_plus()接口里的重复软件操作(reg_dma_rdy0_h |= FLD_DMA_READY_9)。

### Performance Improvements

* N/A

### Note

* N/A

## V1.8.0

### Version
* SDK version: telink_b85m_driver_sdk V1.8.0.
* This version of SDK supports B80(A1), B80B, B85, B87 chips.
* The default configuration of LEDs and KEYs match the following hardware revisions:
*	B80	 	C1T261A30_V1_1
*	B80B	C1T321A30_V1_0
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2

### BREAKING CHANGES

* **(B80)**
    * To distinguish b80 from b80b, b80's libdriver.a is renamed libdriver_b80.a.  

### Bug Fixes

* N/A

### Features

* **(B80B)**
    * PM/ADC/AES/BQB/Debug/Display/DUT/FLASH/GPIO/I2C/IR_LEARN/KEYSCAN/OTP/PWM/QDEC/RF/S7816/SPI/TIMER/UART/USB    

### Refactoring

* **BQB_EMI_Demo**
  * (B80)Add macro configuration to uart related interfaces in the demo.
* **uart**
  * (B80)Adjusting the position of preprocessing instructions.

### Performance Improvements

* N/A

### Note

* N/A

### 版本

* SDK版本: telink_b85m_driver_sdk V1.8.0。
* 此版本SDK支持B80(A1)、 B80B、 B85、B87芯片。
* LED和KEY的默认配置匹配以下硬件版本:
*	B80	 	C1T261A30_V1_1
*	B80B	C1T321A30_V1_0
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2

### BREAKING CHANGES

* **(B80)**
    * 为了区分b80和b80b，b80的libdriver.a更名为libdriver_b80.a。

### Bug Fixes

* N/A

### Features

* **(B80B)**
    * PM/ADC/AES/BQB/Debug/Display/DUT/FLASH/GPIO/I2C/IR_LEARN/KEYSCAN/OTP/PWM/QDEC/RF/S7816/SPI/TIMER/UART/USB

### Refactoring

* **BQB_EMI_Demo**
  * (B80)demo中uart相关接口添加宏配置。
* **uart**
  * (B80)调整预处理指令位置。

### Performance Improvements

* N/A

### Note

* N/A

## V1.7.0

### Version

* SDK version: telink_b85m_driver_sdk V1.7.0
* This version of SDK supports B80(A1), B85, B87 chips.
* The default configuration of LEDs and KEYs match the following hardware revisions:
*	B80	 	C1T261A30_V1_1
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2
### BREAKING CHANGES

* **gpio**
  * (B80) Changed member name of enumeration gpio_group_irq_e.
* **rf**  
  * (B80) Change the interface tx_settle_adjust() to rf_set_tx_settle_time(). (with the same functionality)
* **pm**
  * (B80) Delete the interface pm_set_vdd_1v2().

### Bug Fixes

* **uart**
  * (B80/B85/B87)Fixed the problem of data sending error due to the fact that after the uart_send_byte() function exits, the data memory defined inside the function has been freed before the DMA has finished handling the data.(merge_requests/@467)
* **xtal** 
  * (B85)It solves the problem that when the crystal oscillator is abnormal, the program crashes and the software cannot reset by itself.(merge_requests/@485)
* **pm**
  * (B87)Fixed an issue where the 32k calibration value was inaccurate for a period of time immediately after suspend wake up, and immediately going back to sleep at this time could cause a crash.(merge_requests/@471)
* **rf**
  * (B80)Fixed the issue with not being able to correctly configure and retrieve access codes for pipes 2 to 5 in the rf_acc_code_set() and rf_acc_code_get() interfaces.(merge_requests/@475)
* **USB_Demo** 
  * (B80/B85/B87)Fixed a bug in USB enumeration where the device returned too much data when the host request length was less than the actual descriptor length.(merge_requests/@478)
  * (B80/B85/B87)Fixed a bug where the HID descriptor returned too much data when the host requested an interface-specific descriptor.(merge_requests/@478)

### Refactoring

* **emi**
  * (B80/B85/B87)Add defines the length of the tx payload using the EMI TX-PKT-PAYLOAD macro.
* **license** 
    * Update license format information.
* **UART_Demo**
  * (B80/B85/B87) Modified uart demo folder structure, virtual folder is used in the project and linked to the local folder UART_DEMO/UART_V1.0/UART_DEMO via relative path.
* **S7816_Demo**
  * (B80/B85/B87) Modified s7816 demo folder structure, virtual folder is used in the project and linked to the local folder s7816_DEMO/s7816_V1.0/s7816_DEMO via relative path.
* **USB_Demo**
    * (B80/B85/B87)CDC endpoints are configurable and default to USB_EDP_CDC_IN and USB_EDP_CDC_OUT.The CDC sends data using the endpoint busy bit instead of the delay_us to determine if the data is sent to completion.
    * (B80/B85/B87)Use usb_init() instead of usb_init_interrupt() to initialize the USB. (usbhw_set_eps_en() is required to enable the used endpoints if using the usb_init() interface).

### Features

* **rf**
  * (B80)New interface added:rf_set_rx_sync_threshold(); 	rf_set_tx_wait_time();rf_set_rx_wait_time();
  * (B80)Add PTX and PRX related interfaces:rf_set_ptx_init_pid();rf_set_prx_init_pid();rf_set_ptx_prx_ack_en();rf_set_ptx_prx_ack_dis();rf_ptx_prx_config();rf_start_ptx();rf_start_prx();rf_set_ptx_retry();
* **cstartup_flash.S**
  * (B87)Add the function that sets the watchdog capture_tick and starts it in cstartup_flash.S.

### BREAKING CHANGES

* **gpio**
  * (B80) 修改了枚举gpio_group_irq_e的成员名称。	
* **rf**
  * (B80)将接口tx_settle_adjust()改为rf_set_tx_settle_time()。(功能相同)
* **pm**
  * (B80)删除了pm_set_vdd_1v2()接口。

### Bug Fixes

* **uart**
  * (B80/B85/B87)修复了由于 uart_send_byte() 函数退出后，DMA 还未将数据搬运完成前，函数内部定义的数据内存已经被释放，导致数据发送出错问题。(merge_requests/@467)
* **xtal** 
  * (B85)解决了晶振异常时，程序死机，不能自行软件复位的问题。(merge_requests/@485)
* **pm** 
  * (B87)解决了suspend唤醒后刚开始一段时间32k校准值不准，此时立即再进入睡眠可能会导致死机的问题。(merge_requests/@485)
* **rf**  
  * (B80)修复rf_acc_code_set()和rf_acc_code_get()接口中无法正确配置和获取pipe2到pipe5 access code的错误(merge_requests/@481)
* **USB_Demo** 
  * (B80/B85/B87)修复了 USB 枚举时，host 请求长度小于实际描述符长度时，device 返回过多数据的 bug。(merge_requests/@478)
  * (B80/B85/B87)修复了主机请求接口特殊描述符时， HID 描述符返回数据过多的 bug。(merge_requests/@478)

### Refactoring

* **emi**
  * (B80/B85/B87)增加通过EMI_TX_PKT_PAYLOAD宏定义tx payload长度。
* **license** 
    * 更新 license 格式信息。
* **UART_Demo**
  * (B80/B85/B87)修改了uart demo文件夹结构，工程中采用虚拟文件夹，并通过相对路径链接到本地文件夹UART_DEMO/UART_V1.0/UART_DEMO。
* **S7816_Demo**
  * (B80/B85/B87)修改了s7816 demo文件夹结构，工程中采用虚拟文件夹，并通过相对路径链接到本地文件夹s7816_DEMO/s7816_V1.0/s7816_DEMO。
* **USB_Demo**
    * (B80/B85/B87)CDC 端点可配置，默认为 USB_EDP_CDC_IN 和 USB_EDP_CDC_OUT。CDC 发送数据时使用端点忙位而不是 sleep_us 来确定数据是否发送完毕。
    * (B80/B85/B87)使用 usb_init() 取代 usb_init_interrupt()来初始化 USB。(如果使用 usb_init() 接口，需要使用 usbhw_set_eps_en() 来使能用到的端点)。

### Features

* **rf**
  * (B80)新增接口:rf_set_rx_sync_threshold();rf_set_tx_wait_time();rf_set_rx_wait_time();
  * (B80)新增PTX,PRX相关接口:rf_set_ptx_init_pid();rf_set_prx_init_pid();rf_set_ptx_prx_ack_en();rf_set_ptx_prx_ack_dis();rf_ptx_prx_config();rf_start_ptx();rf_start_prx();rf_set_ptx_retry();
* **cstartup_flash.S**
  * (B87)在cstartup_flash.S添加设置并使能watchdog功能。
## V1.6.0

### Version

* SDK version: telink_b85m_driver_sdk V1.6.0.
* This version of SDK supports B80(A1), B85, B87 chips.
* The default configuration of LEDs and KEYs match the following hardware revisions:
*	B80	 	C1T261A30_V1_1
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2

### BREAKING CHANGES

* **sys**
  * (B87)Delete the soft_start_dly_time interface and add the pm_set_wakeup_time_param interface instead.
  * (B85/B87)Occupies DEEP_ANA_REG0[bit1] (0x3a[1]) to mark whether restart caused by abnormal start-up has occurred, the customer cannot use this bit.
* **audio**
  * (B85/B87)Enumeration member FLD_RIGTH_SHAPING_EN is changed to FLD_RIGHT_SHAPING_EN.
* **usb** 
  * (B80/B85/B87)Change the register reg_usb_ep8_send_thre to reg_usb_ep8_send_thres.
* **system** 
  * (B80/B85/B87)Change the register member FLD_SYSTEM_32K_TIMER_SYCN_TRIG to FLD_SYSTEM_32K_TIMER_SYNC_TRIG.
* **i2c** 
  * (B80/B85/B87)Function interface void reset_i2c_moudle() changed to void reset_i2c_module().
* **spi** 
  * (B80/B85/B87)Function interface void reset_spi_moudle() changed to void reset_spi_module().
* **pm** 
  * (B85/B87)Delete the cpu_get_32k_tick interface.If you want to read a 32K tick value, use pm_get_32k_tick().

### Bug Fixes

* **timer**
  * (B80/B87/B85)timer0_set_mode/timer1_set_mode/timer2_set_mode/timer_start/timer_stop:in addition to the corresponding functions, the interrupt flags of time0/timer1/timer2 are also incorrectly cleared on the preceding interfaces. this problem occurs in the following application scenarios: when multiple timers work at the same time and the interrupt time is very close, the interrupt interface processes the timer that requests the interrupt first. if the above interface is called, the interrupt flag of other timers will be mistakenly cleared, resulting in no response to other timers' interruption. at present, it is modified to only set the corresponding function.(merge_requests/@356)
* **watchdog**
  * (B80/B87/B85)wd_set_interval_ms/wd_start/wd_stop:in addition to the corresponding functions, the interrupt flags of time0/timer1/timer2 are also mistakenly cleared on the above interfaces. At present, they are modified to set only corresponding functions.(merge_requests/@356)
* **pm**
  * (B87/B85)cpu_stall_wakeup_by_timer0/cpu_stall_wakeup_by_timer1/cpu_stall_wakeup_by_timer2:add setting timer0/timer1/timer2 mode0 to prevent these interfaces from working until timer0/timer1/timer2 is not mode0.(merge_requests/@356)
  * (B80/B85/B87)Solved the problem that the sleep time is incorrect due to a small probability that the total interruption time of the shutdown is not timely.(merge_requests/@361)
  * (B80/B85/B87)Eliminates the risk that STATUS_GPIO_ERR_NO_ENTER_PM may be set to 1 in the return value of cpu_sleep_wakeup.(merge_requests/@438)
  * (B85/B87)Resolved the risk that cpu_sleep_wakeup_32k_rc()/cpu_sleep_wakeup_32k_xtal() could crash due to misread 32K tick. (The digital reading 32K tick inside these two functions has been replaced with reading using analog registers).(merge_requests/@450)
  * (B85)Solved the problem of disconnection caused by 32k calibration value not being updated in time in high and low temperature environments.(merge_requests/@450)
* **systemclk**
  * (B80)The problem that the definition of system_clk_type does not match the enumeration SYS_CLK_TypeDef is solved.(merge_requests/@378)
* **sys**
  * (B85/B87)Solve the problem that the crystal oscillator stability flag fails to cause the crash. If the start-up is abnormal, it will restart. Use DEEP_ANA_REG0[bit1] to check whether the restart caused by the abnormal start-up has occurred. (It has been implemented in the driver layer, and does not require special calls from the application layer.)(merge_requests/@365)
* **adc**
  * (B80)Fixed bug that GPIO sampling was not calibrated.(merge_requests/@385)
* **gpio**
  * (b80/b85/b87)Fixed an issue in the gpio_set_interrupt()/gpio_set_interrupt_risc0()/gpio_set_interrupt_risc1() interface that could mistakenly clear other interrupt status bits.(merge_requests/@449)
  * (b80)Fixed an issue in the gpio_set_src_irq() interface that could mistakenly clear other interrupt status bits.(merge_requests/@449)
* **uart**
  * (b80/b85/b87)uart_clear_parity_error(): Fixed the problem that when clearing uart clear rx status, other uart status bits may be operated by mistake.(merge_requests/@410)
* **irq**
  * (b80/b85/b87)irq_clr_src2(): Fixed the problem that when clearing the specified IRQ source, other IRQ source bits may be operated by mistake.(merge_requests/@410)
* **usb**
  * (b80/b85/b87)usb_handle_irq(): Fixed the problem that when clearing USB reset status, other status bits may be operated by mistake.(merge_requests/@410)
* **printf**
  * (b80/b85/b87)sprintf(): Modify the original default 8-byte printing using %x format to the adaptive number of bytes.(merge_requests/@413)
* **adc**  
  * (B80/B85/B87)Tighten the illegal value judgment in the function user_calib_adc_vref().(merge_requests/@416)
* **otp**
  * (B80)otp_set_deep_standby_mode():Fixed an issue where OTP power down may fail.(merge_requests/@423)
* **clock**
  * (B85)rc_24m_cal() is added to the clock_init() function to solve the problem of possible probabilistic crash.(merge_requests/@451)
* **calibration**
  * (B80)Fixed an issue where the adc calibration interface returned the wrong calibration value when the calibration value was a negative number.(merge_requests/@446)
  * (B80)ADC calibration rule for flash has not been determined yet. Therefore, follow ADC calibration rule of OTP temporarily.(merge_requests/@463)
* **adc**  
  * (B85/B87)Fix the problem that the calibration value adc_gpio_calib_vref is overwritten when the vref voltage is 1.2V after calling adc_set_ref_voltage() to switch the vref voltage to 0.6V or 0.9V, and add a note to remind that switching back to 1.2V again, adc_vref and adc_vref_offset are not not be re-assigned.(merge_requests/@454)
* **EMI_Demo**
  * (B80/B85/B87)Fixed occasional packet sending anomaly in anti-noise test.(merge_requests/@404)
  * (B80/B85/B87)Fixed RSSI value fetching exception issue in emi code.(merge_requests/@447)
* **AUDIO_Demo**
  * (B85/B87)fix the problem that in AUDIO_AMIC_TO_BUF demo, the used fifo channel is inconsistent with the default enabled fifo channel in audio_amic_init().(merge_requests/@433)
* **DUT_Demo**
  * (B80)Fixed the problem that DUT_Demo OTP program could not run in SRAM.(merge_requests/@441)

### Refactoring

* **swire**
  * (B80/B85/B87) Removed Swire.h file.
* **clock**
  * (B85)Add a note explaining the limitations of the use of 48M calibration.
* **license** 
    * Adjusted APACHE License for files related
* **adc**
  * (B80/B85/B87)Add comments to adc_vref.
* **rf**
  * (b80/b85/b87):Clear rf_drv.h warnings in the project.
  * (b80/b85/b87):Simplified the implementation of rf_update_internal_cap.
* **all**
  * (B80/B85/B87)Normalizes the naming of content related to private schemas.
  * (b80/b85/b87):Clear all typos and warnings in the project.
* **flash**
  * (B80/B85/B87)Add the comment "Location to be updated when adding a new flash" in flash.c to prevent incomplete updates when adding a flash.
  * (B80/B85/B87)Change the flash_write_status interface to a weak definition. The application layer can redefine the interface according to its requirements.
* **otp**
  * (B80)Add the following comment to the otp related interface: chip version A0/A1/A2/A3 can't read/write OTP in RF RX state, A4 chip doesn't have this limitation. The value of the variable g_chip_version can be read after calling cpu_wakeup_init().
* **calibration**
  * (B80)There will be no 1V2 calibration values in the chip, so the user_calib_vdd_1v2() function interface is removed。
 * **PM_Demo** 
  * (B80/B85/B87)Optimize and distinguish the flexible use process of functions in IDLE_TIMER_WAKEUP and similar modes by macros.
* **GPIO_Demo**
  * (b80/b85/b87)Refactored gpio demo.
  
### Features

* **pm**
  * (B85/B87)Added function interface pm_set_wakeup_time_param.
* **clock**  
  * (B85/B87) XTAL_TypeDef added EXTERNAL_XTAL_EXTERNAL_CAP_24M, deleted EXTERNAL_XTAL_48M.
* **flash**
  * (B87)add flash P25Q80U.
* **usb**
  * (B80/B85/B87)add set config and get config support when disable auto config
* **uart**
  * (B85/B87/B80)uart_gpio_set:tx and rx are configured at the same time, change it to be configurable independently.
* **rf**
  * (B80/B85/B87):Added function interface rf_set_irq_mask/rf_clr_irq_mask/rf_get_irq_status/rf_clr_irq_status; Added enumeration definition rf_irq_e.
* **flash**
  * (B80/B85/B87)In each flash, add the interface flash_get_lock_block_midxxx to return which area is currently locked in flash 
  (also in midxxx_lock_block_e, Note When flash write protection is used, only the midxxx_lock_block_e enumeration value 
  can be selected, so that the values returned by the flash_get_lock_block_midxxx interface are in midxxx_lock_block_e).
  * (B80/B85/B87)In flash.h, add a new enumeration flash_mid_e, which can be used instead of using flash mid directly, 
  and change the return type of flash_read_mid() interface to flash_mid_e to increase readability.
* **adc**
  * (B80)Added interfaces adc_update_vref_calib_value_ft_cp() and adc_update_vref_calib_value_flash() for determining whether the range of ft/cp and flash calibration values are legal.
* **pm**
  * (B85/B87)Added lpc wakeuo function:support lpc wakeup function in interfaces of cpu_sleep_wakeup_32k_rc(),cpu_sleep_wakeup_32k_xtal(),cpu_long_sleep_wakeup_32k_rc(),cpu_long_sleep_wakeup_32k_xtal()
* **flash**
  * (b87)Add comments to support new flash GD25LD40E and new flash GD25LD80E.
  (Comparison has been made between the same mid flash GD25LD40C and GD25LD80C that the driver already exists, and it is confirmed that the driver does not need to be updated.)
* **gpio**
  * (b80)Add a new gpio interrupt (gpio2risc2).
* **clock**
  * (B80/B85/B87)Provide clock_init_calib_24m_rc_cfg() and cpu_wakeup_init_calib_32k_rc_cfg() interfaces for users to configure whether or not to calibrate 24m and 32k rc.
* **Audio_Demo**
  * (B87/B85)Added SDM output 1Hz and 10Hz sine wave demo  
* **Debug_Demo**
  * (B80/B85/B87)add the use of sprintf function. 
* **FLASH_Demo**
  * (B80/B85/B87)Maintain two interfaces flash_lock/flash_unlock (determining whether a flash is locked or unlocked) for the application layer 
  to enumerate all the flashes, so that the application layer does not miss any due to the subsequent addition of new flashes. 
* **SDK VERSION**  
  * (B80/B85/B87):Add sdk version at the end of bin file.

### Performance Improvements

* **emi**
  * (B87/B80/B85)Improve the sensitivity performance of emi programs at 24M multiple frequency points.
* **bqb**
  * (B87/B80/B85)Improve the sensitivity performance of bqb programs at 24M multiple frequency points.

### Note

* N/A

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b85m_driver_sdk V1.6.0。
* 此版本SDK支持B80(A1)、B85、B87芯片。
* LED和KEY的默认配置匹配以下硬件版本:
*	B80	 	C1T261A30_V1_1
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2

### BREAKING CHANGES

* **sys**
  * (B87)删除soft_start_dly_time接口，添加pm_set_wakeup_time_param接口进行替代。
  * (B85/B87)占用DEEP_ANA_REG0[bit1]（0x3a[1]）标志是否发生过起振异常导致的重启，客户不能使用这个bit。
* **audio**
  * (B85/B87)枚举成员FLD_RIGTH_SHAPING_EN改为FLD_RIGHT_SHAPING_EN。
* **usb** 
  * (B80/B85/B87)寄存器reg_usb_ep8_send_thre改为reg_usb_ep8_send_thres。
* **system** 
  * (B80/B85/B87)寄存器成员FLD_SYSTEM_32K_TIMER_SYCN_TRIG改为FLD_SYSTEM_32K_TIMER_SYNC_TRIG。
* **i2c** 
  * (B80/B85/B87)函数接口void reset_i2c_moudle()改为void reset_i2c_module()。
* **spi** 
  * (B80/B85/B87)函数接口void reset_spi_moudle()改为void reset_spi_module()。
* **pm** 
  * (B85/B87)删除cpu_get_32k_tick接口，如果想要读32K的tick值，可以使用pm_get_32k_tick()。

### Bug Fixes

* **timer**
  * (B80/B87/B85)timer0_set_mode/timer1_set_mode/timer2_set_mode/timer_start/timer_stop::以上这些接口除了设置了对应的功能以外，还误清了timer0、timer1、timer2的中断标志位。在如下应用场景下会有问题：当使用多个timer同时工作，并且产生的中断时间很接近时，中断接口中先处理第一个请求中断的timer时，调用了以上接口的话，将其他timer的中断标志位误清，导致其他timer中断没有响应，目前将其进行修改为只设置对应功能。(merge_requests/@356)
* **watchdog**
  * (B80/B87/B85)wd_set_interval_ms/wd_start/wd_stop:以上这些接口除了设置了对应的功能以外，还误清了timer0、timer1、timer2的中断标志位，目前将其进行修改为只设置对应功能。(merge_requests/@356)
* **pm**
  * (B87/B85)cpu_stall_wakeup_by_timer0/cpu_stall_wakeup_by_timer1/cpu_stall_wakeup_by_timer2:添加设置timer0/timer1/timer2 mode0功能，避免在调用这些接口之前，timer0/timer1/timer2不是mode0，这些接口不能工作。(merge_requests/@356)
  * (B80/B85/B87)解决了因关总中断时间不及时，小概率地导致的睡眠时间不对的问题。(merge_requests/@361)
  * (B80/B85/B87)规避了cpu_sleep_wakeup的返回值中STATUS_GPIO_ERR_NO_ENTER_PM 可能会被异常置为1的风险。(merge_requests/@438)
  * (B85/B87)解决了cpu_sleep_wakeup_32k_rc()/cpu_sleep_wakeup_32k_xtal()可能因为读错32K tick引发死机的风险（已将这两个函数内部，数字读32K tick的方式替换成模拟方式读32Ktick）。(merge_requests/@450)
  * (B85)解决了在高温和低温环境下，因32k校准值没有及时更新，导致的断联的问题。(merge_requests/@450)
* **systemclk** 
  * (B80)解决了system_clk_type的定义与枚举SYS_CLK_TypeDef不匹配的问题。(merge_requests/@378)
* **sys** 
  * (B85/B87)解决晶振稳定标志位失灵导致死机的问题。 起振异常则重启，通过DEEP_ANA_REG0[bit1]查询是否发生过起振异常导致的重启。（已在驱动层实现，不需要应用层特殊调用。）(merge_requests/@365)
* **adc**
  * (B80)修复了GPIO采样未进行校准的问题。(merge_requests/@385)
* **gpio**
  * (b80/b85/b87)修复了gpio_set_interrupt()/gpio_set_interrupt_risc0()/gpio_set_interrupt_risc1()接口中可能误清其他中断状态位的问题。(merge_requests/@449)
  * (b80)修复了gpio_set_src_irq()接口中可能误清其他中断状态位的问题。(merge_requests/@449)
* **uart**
  * (b80/b85/b87)uart_clear_parity_error(): 修复了清除uart clear rx状态时，可能误操作其他uart状态位的问题。(merge_requests/@410)
* **irq**
  * (b80/b85/b87)irq_clr_src2(): 修复了清除指定的中断源时，可能误操作其他中断源的状态位的问题。(merge_requests/@410)
* **usb**
  * (b80/b85/b87)usb_handle_irq(): 修复了清除usb reset状态位时，可能误操作其他状态位的问题。(merge_requests/@410)
* **printf**
  * (b80/b85/b87)sprintf(): 修改使用%x格式，将原先默认8字节打印设置为自适应字节数打印。(merge_requests/@413)
* **adc**
  * (B80/B85/B87)将函数user_calib_adc_vref()中的非法值判断加严。(merge_requests/@416)
* **otp**
  * (B80)otp_set_deep_standby_mode()：修复了 OTP power down 时可能会失败的问题。(merge_requests/@423)
* **clock**
  * (B85)在clock_init函数中加入rc_24m_cal()解决可能概率性死机的问题.(merge_requests/@451)
* **calibration**
  * (B80)修复了当校准值是负数时，adc校准接口返回错的校准值的问题。(merge_requests/@446)
  * (B80)flash的ADC校准规则没有确定。因此，暂时遵循OTP的ADC校准规则。(merge_requests/@463)
* **adc**  
  * (B85/B87)修复调用adc_set_ref_voltage()切换vref电压为0.6V或0.9V后，vref电压为1.2V时的校准值adc_gpio_calib_vref被覆盖的问题，并添加注释提醒：再次切换回1.2V，adc_vref和adc_vref_offset并不会重新赋值。(merge_requests/@454)
* **EMI_Demo** 
  * (B80/B85/B87)解决了在抗干扰测试过程中概率性出现发包异常的问题。(merge_requests/@404)
  * (B80/B85/B87)修复了emi代码中RSSI值获取异常问题。(merge_requests/@447)
* **AUDIO_Demo**
  * (B85/B87)修复AUDIO_AMIC_TO_BUF demo中，使用的fifo通道和audio_amic_init()中默认使能的fifo通道不一致问题。(merge_requests/@433)
* **DUT_Demo**
  * (B80)修复 DUT_Demo OTP 程序无法在 SRAM 运行的问题。(merge_requests/@441)

### Refactoring

* **swire**
  * (B80/B85/B87)删除了Swire.h文件。
* **clock**
  * (B85)添加注释，说明48M校准的使用限制。
* **license** 
    * 调整了相关文件的APACHE许可。
* **adc**
* (B80/B85/B87)对adc_vref添加注释。

* **rf**
* (b80/b85/b87):清除工程里rf_drv.h宏定义警告。
* (b80/b85/b87):简化rf_update_internal_cap内部分实现。
* **all**
  * (B80/B85/B87)规范化与私有模式相关内容的命名。
  * (b80/b85/b87):清除工程里的所有错别字和警告。
* **flash** 
  * (B80/B85/B87)在flash.c中添加"新增flash时需要更新的位置"注释 ，防止新增flash时更新不完整。
  * (B80/B85/B87)将接口flash_write_status改为弱定义，应用层可以根据自己的需求重新定义该接口。
* **otp**
  * (B80)otp的相关接口添加如下注释：芯片版本A0/A1/A2/A3不能在RF RX状态下读写OTP, A4芯片没有这个限制。可调用cpu_wakeup_init()之后读g_chip_version变量的值。
* **calibration**
  * (B80)芯片中不会有1V2的校准值，所以删除了user_calib_vdd_1v2()函数接口。
* **PM_Demo** 
  * (B80/B85/B87)通过宏定义优化区分不同芯片对IDLE_TIMER_WAKEUP及相似模式下的函数灵活使用过程。
* **GPIO_Demo**
  * (b80/b85/b87)重构了gpio demo.

### Features

* **pm**
  * (B80/B85/B87)新增函数接口pm_set_wakeup_time_param。
* **clock**
  * (B80/B85/B87)XTAL_TypeDef新增EXTERNAL_XTAL_EXTERNAL_CAP_24M，删除EXTERNAL_XTAL_48M。
* **flash**
  * (B87)新增flash P25Q80U。
* **usb**
  * (B80/B85/B87)USB 手动配置模式下，支持获取配置命令和设置配置命令
* **uart**
  * (B80/B85/B87)uart_gpio_set:tx和rx会同时配置,修改为可以单独配置。
* **rf**
  * (B80/B85/B87):新增函数接口rf_set_irq_mask/rf_clr_irq_mask/rf_get_irq_status/rf_clr_irq_status；新增枚举定义rf_irq_e。
* **flash**
  * (B80/B85/B87)给所有flash驱动添加flash_get_lock_block_midxxx()接口，返回flash当前锁的是哪一块区域
   （同时在midxxx_lock_block_e中，添加注释说明flash写保护时只能选择midxxx_lock_block_e的枚举值，
    使得flash_get_lock_block_midxxx接口返回的值都在midxxx_lock_block_e中）。
  * (B80/B85/B87)在flash.h中，新增枚举flash_mid_e，可以使用枚举值代替直接使用flash mid，同时将flash_read_mid()接口的返回类型更改为flash_mid_e，增加可读性。
* **adc**
  * (B80)添加了判断ft/cp和flash校准值范围是否合法的接口adc_update_vref_calib_value_ft_cp()和adc_update_vref_calib_value_flash()。
* **pm**
  * (B85/B87)新增了lpc唤醒功能：在接口cpu_sleep_wakeup_32k_rc()，cpu_sleep_wakeup_32k_xtal()，cpu_long_sleep_wakeup_32k_rc()，cpu_long_sleep_wakeup_32k_xtal()中新增了lpc唤醒功能
* **flash**
  * (b87)添加注释以支持新增flash GD25LD40E和GD25LD80E。（已对比驱动已有的同mid flash GD25LD40C和GD25LD80C，确认驱动不需要更新。）
* **flash**
  * (b85)添加注释以支持新增flash GD25LD40E和GD25LD80E。（已对比驱动已有的同mid flash GD25LD40C和GD25LD80C，确认驱动不需要更新。）
* **gpio**
  * (b80)添加新增的一路gpio中断(gpio2risc2)
* **clock**
  * (B80/B85/B87)新增clock_init_calib_24m_rc_cfg()和cpu_wakeup_init_calib_32k_rc_cfg()接口供用户配置是否要校准24m及32k RC 。
* **Audio_Demo**
  * (B85/B87)增加了SDM输出1Hz和10Hz sine wave demo
* **Debug_Demo**
  * (B80/B85/B87)增加对sprintf函数的使用方法。
* **FLASH_Demo**
  * (B80/B85/B87)给应用层维护两个接口flash_lock/flash_unlock（判断flash是否上锁还是解锁），将所有的flash列举出来，避免由于后续新增flash，应用层有遗漏。
* **SDK版本**  
  * (B80/B85/B87):bin文件末尾添加SDK版本.

### Performance Improvements

* **emi**
  * (B87/B80/B85)提升emi程序在24M倍数频点的sensitivity性能。
* **bqb**
  * (B87/B80/B85)提升bqb程序在24M倍数频点的sensitivity性能。

### Note

* N/A

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

## V1.5.0

### Version

* SDK version: telink_b85m_driver_sdk V1.5.0.
* This version of SDK supports B80(A1), B85, B87 chips.
* The default configuration of LEDs and KEYs match the following hardware revisions:
*	B80	 	C1T261A30_V1_1
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2

### BREAKING CHANGES

* **watchdog**
  * (B80)When the chip is powered on, a 32K watchdog is enabled by default. In the previous version of the driver, this function is disabled in the cpu_wakeup_init function. In this version of the driver, cpu_wakeup_init does not handle,need the application, according to the use of their own needs.
* **PM**
  * (B80)delete the pm_set_32k_watchdog_interval interface and add a new 32K watchdog interface to replace it.
  * (B80)Delete the soft_start_dly_time interface and add the pm_set_wakeup_time_param interface instead.
  * (B80)Occupies DEEP_ANA_REG0[bit1] (0x3a[1]) to mark whether restart caused by abnormal start-up has occurred, the customer cannot use this bit. 

### Bug Fixes

* **BQB_Demo**
  * (B80/B85/B87)When testing the RX, when switching from 2M to S8 mode, the CMW500 showed a prompt of PER 100%. After restart, it still has no effect. It needs to be powered on again to return to normal.
* **cstartup_otp.S**
  * (B80)Fixed the abnormal function of the otp program when ALL_SRAM_CODE=1.
* **pm**
  * (B80)Fixed the problem that the flash power-on time does not meet the time required by the flash datasheet after deep/deep retention wakes up (this may have a probabilistic exception, but it has not been tested so far).
* **sys**
  * (B80)Solve the problem that the crystal oscillator stability flag fails to cause the crash. If the start-up is abnormal, it will restart. Use DEEP_ANA_REG0[bit1] to check whether the restart caused by the abnormal start-up has occurred. (It has been implemented in the driver layer, and does not require special calls from the application layer.)
* **aoa**
  * (B85/B87)Fix the problem that the rf_aoa_aod_get_pkt_rssi and rf_aoa_aod_hdinfo_offset functions get abnormal results.
### Refactoring

* **ALL_Demo**
  * (B80/B85/B87)All demos use the led and key of the telink development board by default, and delete the useless definitions of SW1 and SW2.
* **cstartup_flash.S**
  * (B80)Optimized code: removed flash wake-up code.
* **cstartup_otp.S**
  * (B80)Optimized code: Removed the code for multi-address startup.
* **emi.c**
  * (B85/B87/B80)Move emi.c out of the library file and make it an open file.
* **adc.c**
  * (B85)Move the code for configuring the sampling rate in the 'adc_base_init' and 'adc_vbat_init' interfaces to 'adc_init', Reduce repetitive code.
  
### Features

* **GPIO_Demo**
  * (B80/B85/B87)Added the function of triggering interrupts by using the buttons that come with the development board.
* **Vendor/common/calibration**
  * (B80)add interface(user_calib_vdd_1v2):
  *      1.vdd_1v2 needs to be calibrated only if the chip is running otp programs and does not move all programs to ram. to distinguish whether the otp program is moving all programs to ram, add the macro OTP_ALL_SRAM_CODE; 
  *      2.The calibration principle of vdd_1v2 is as follows: if there is a calibration value in otp, use the calibration value in otp if there is no calibration value, use VDD_1V2_1V1;
  * (B80)Added interface 'user_read_otp_value_calib'. The OTP package chip is used to calibrate various parameters, and this time the ADC calibration section has been updated.
  * (B80)Added interface 'user_read_flash_value_calib'. The FLASH package chip is used to calibrate various parameters, this time the ADC calibration part is updated.
  * (B80)Added interface 'user_calib_adc_vref'. Used to read the ADC calibration value and pass it to the ADC module.
* **PM**
  * (B80)add interface(pm_set_vdd_1v2): used to adjust vdd_1v2, cannot be called at will, must follow the rules of user_calib_vdd_1v2 interface.
  * (B80)cpu_sleep_wakeup_32k_rc/cpu_sleep_wakeup_32k_xtal: 
  *      1. the previous version of the driver, suspend wake, 32k watchdog closed, the current version of the driver, no processing; 
  *      2. If the wakeup source does not contain the timer wakeup source, close the 32K watchdog;
  * (B80)Added function interface pm_set_wakeup_time_param.
  * (B80)Added long sleep interface cpu_long_sleep_wakeup.
* **WATCHDOG**
  * (B80)add 32k watchdog interface(wd_32k_start/wd_32k_stop/wd_32k_set_interval_ms),and add case in time_demo;
* **rf**
  * (B87)Add the frequency point setting function rf_set_channel_k_step with K as the step
* **clock**
  * (B80) XTAL_TypeDef added EXTERNAL_XTAL_EXTERNAL_CAP_24M, deleted EXTERNAL_XTAL_48M.
* **Zb_flash**
  * (B85/B87):add zb_cam_check api for zb flash cam check.
* **flash**
  * (B80)add flash P25Q40SU/P25D09U.
* **adc**
  * (B80)Added interface 'adc_set_gpio_calib_vref' and interface 'adc_set_vbat_calib_vref' for calibrating reference voltage.
  * (B85)Added interface 'adc_sample_and_get_result_manual_mode', for single sampling.
  * (B85)Added the configuration of sampling rate 192KHz.
* **SDK VERSION**  
  * (B80/B85/B87):Add sdk version at the end of bin file.
  * (B80/B85/B87)：Added 'tl_Check_Fw.sh' script, print SDK version information after compiling the code.
* **.S**
  * (B85/B87):Add SRAM_ SIZE Macro Definition.
* **.link**
  * (B80/B85/B87):Add assertions to determine the approximate range of RAM.
### Performance Improvements

* N/A

### Note

* N/A

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b85m_driver_sdk V1.5.0。
* 此版本SDK支持B80(A1)、B85、B87芯片。
* LED和KEY的默认配置匹配以下硬件版本:
*	B80	 	C1T261A30_V1_1
*	B85	 	C1T139A30_V1_2
*	B87  	C1T197A30_V1_2

### BREAKING CHANGES
* **WATCHDOG**
  * (B80)芯片上电，默认会有个32k watchdog是打开的，之前版本的驱动，在cpu_wakeup_init函数中关闭该功能，现在这个版本的驱动，改为cpu_wakeup_init不做处理，需要上层应用，根据使用需求自行处理;
* **PM**
  * (B80)删除pm_set_32k_watchdog_interval接口，添加新32k watchdog接口进行替代;
  * (B80)删除soft_start_dly_time接口，添加pm_set_wakeup_time_param接口进行替代。
  * (B80)占用DEEP_ANA_REG0[bit1]（0x3a[1]）标志是否发生过起振异常导致的重启，客户不能使用这个bit。
   
### Bug Fixes

* **BQB_Demo**
  * (B80/B85/B87)在测试RX时，从2M切到S8模式，CMW500出现出现PER 100%的提示，restart之后依旧没有作用，需要重新上电后恢复正常。
* **cstartup_otp.S**
  * (B80)修复了ALL_SRAM_CODE=1时的otp程序功能异常问题。
* **pm**
  * (B80)修复了deep/deep retention唤醒后，flash上电时间不满足flash datasheet要求的时间的问题（这个可能会出现概率性异常，但是目前没有测试到）。
* **sys** 
  * (B80)解决晶振稳定标志位失灵导致死机的问题。 起振异常则重启，通过DEEP_ANA_REG0[bit1]查询是否发生过起振异常导致的重启。（已在驱动层实现，不需要应用层特殊调用。）
* **aoa**
  * (B85/B87)修复rf_aoa_aod_get_pkt_rssi和rf_aoa_aod_hdinfo_offset函数获取结果异常的问题.
  
### Refactoring

* **ALL_Demo**
  * (B80/B85/B87)所有的demo默认使用telink开发板的led和key，同时删掉了无用的SW1、SW2的定义。
* **cstartup_flash.S**
  * (B80)优化代码：删除了flash唤醒代码。
* **cstartup_otp.S**
  * (B80)优化代码：删除了多地址启动的代码。
* **emi.c**
  * (B85/B87/B80)将emi.c从库文件中移出，改为开放性文件。
* **adc.c**
  * (B85)将'adc_base_init'和'adc_vbat_init'接口中配置采样率的代码移动至'adc_init'中，减少重复代码。

### Features

* **GPIO_Demo**
  * (B80/B85/B87)新增使用开发板自带的按键来触发中断的功能。
* **Vendor/common/calibration**
  * (B80)新增函数接口user_calib_vdd_1v2：
  *     1.只有在如下情况下才需要校准vdd_1v2，芯片跑OTP程序并且不会将程序全搬到ram中时，为了区分跑Otp程序是否将程序全搬到ram中，添加宏OTP_ALL_SRAM_CODE; 
  *     2.vdd_1v2的校准原则是：如果otp中有校准值，使用OTP中的校准值，如果没有校准值使用VDD_1V2_1V1挡位;
  * (B80)增加了接口'user_read_otp_value_calib'。OTP封装芯片用于校准各种参数，这次更新了ADC校准部分。
  * (B80)增加了接口'user_read_flash_value_calib'。FLASH封装芯片用于校准各种参数，这次更新了ADC校准部分。
  * (B80)增加了接口'user_calib_adc_vref'。用于读取ADC校准值并传给ADC模块。
* **PM**
  * (B80)新增函数接口pm_set_vdd_1v2：用于调整vdd_1v2，不可随意调用，必须遵循user_calib_vdd_1v2接口的规则.
  * (B80)cpu_sleep_wakeup_32k_rc/cpu_sleep_wakeup_32k_xtal接口：
  *      1.之前版本的驱动，suspend唤醒之后，将32k watchdog关闭，现在这个版本的驱动，没有做处理;
  *      2.如果唤醒源不包含timer唤醒源的话，将32k watchdog关闭;
  * (B80)新增函数接口pm_set_wakeup_time_param。
  * (B80)增加长睡眠接口cpu_long_sleep_wakeup。
* **WATCHDOG**
  * (B80)添加32k watchdog接口(wd_32k_start/wd_32k_stop/wd_32k_set_interval_ms),并在timer_demo中添加接口使用case;
* **rf**
  * (B87)添加以KHz为步进的频点设置函数rf_set_channel_k_step。
* **clock**
  * (B80)XTAL_TypeDef新增EXTERNAL_XTAL_EXTERNAL_CAP_24M，删除EXTERNAL_XTAL_48M。
* **Zb_flash**
  * (B85/B87):增加了zb_cam_check接口用于校验zb flash cam value。
* **flash**
  * (B80)新增flash P25Q40SU/P25D09U。
* **adc**
  * (B80)增加了接口'adc_set_gpio_calib_vref'和接口'adc_set_vbat_calib_vref',用于校准参考电压。
  * (B85)增加了接口'adc_sample_and_get_result_manual_mode'，用于单次采样。
  * (B85)增加了采样率192KHz的配置。
* **SDK版本**  
  * (B80/B85/B87):bin文件末尾添加SDK版本.
  * (B80/B85/B87)：增加了'tl_check_fw.sh'脚本,编译完代码后打印sdk版本信息。
* **.S**
  * (B85/B87):增加SRAM_ SIZE宏定义。

* **.link**
  * (B80/B85/B87):增加断言判断RAM大致范围。
### Performance Improvements

* N/A

### Note

* N/A

## V1.4.0

### Version

* SDK version: telink_b85m_driver_sdk V1.4.0.
* This version of SDK supports B80(A1), B85 and B87 chips.
* When B80 was in V1.3.0, only A0 chips were tested, and when B80 was in V1.4.0, only A1 chips were tested.
* According to the chip revision instructions, A0 and A1 do not need special software processing for the chip version. In fact, V1.3.0 and V1.4.0 can be used for A0 and A1 chips.
* For the subsequent software update version, only A1 version chips will be verified, and only A1 will be marked for the chip version. It is recommended to use the marked chip version.

### BREAKING CHANGES

* **FLASH**
  * (b85)Modify midxxxx_otp_block_e and midxxxx_lock_otp_e element name of mid011460C8/mid136085/mid1360eb to change the unit K to B to avoid misleading OTP size.
* **ADC**
  * (B80)Integrate the complete configuration sampling IO operation from the adc_base_init interface to the adc_base_pin_init interface, so that the adc_base_pin_init interface can directly switch the sampling IO without calling the adc_base_init interface again. Modified the parameter type 'adc_input_pin_def_e pin'.
* **PM**
  * (B80)Removed interfaces cpu_stall_wakeup_by_timer0, cpu_stall_wakeup_by_timer1, cpu_stall_wakeup_by_timer2, cpu_stall.Use the new interface cpu_stall_wakeup instead, the usage method can refer to pm demo.
* **cstartup_sram.S** 
  * (B80)Delete the cstartup_sram.S file and merge the functions into the cstartup_flash.S and cstartup_otp.S files.

### Bug Fixes

* **cstartup_xx.S**
  * (B80)Fixed an issue that could cause the program to crash after waking up from retention under certain code structures or optimization options.
* **RF**
  * (B87/B80)Fixed the inaccurate problem that rf_set_tx_modulation_index and rf_set_rx_modulation_index set MI to values other than 0.32 and 0.50, and added new gears as required.
* **OTP**
  * (B80)if the otp read-write function interface is used to read and write data from the following start address range: 0x3ff4/0x3ff8, an exception is triggered, which has been corrected. the otp of this version can read and write data from the start address range: 0x0000 to 0x3ffc.
* **PM**
  * (B80)Fix the problem that the retention return sleep flag pmParam.is_deepretn_back is invalid;.
* **EMI_Demo**
  * (B85/B87/B80)Added usb interface initialization after gpio_shutdown to solve the problem that some chips that only support Swire (through-usb) communication cannot communicate through Swire after downloading the EMI program.

### Refactoring

* **BQB_Demo**
  * (B85/B87/B80)Modified the writing method of obtaining the size of the flash capacity, which is consistent with the description of the flash capacity in flash.h.
* **EMI_Demo**
  * (B85/B87/B80)Added the interface read_calibration_flash for reading calibration parameters.
  * (B85/B87/B80)Modified the writing method of obtaining the size of the flash capacity, which is consistent with the description of the flash capacity in flash.h.
* **Vendor/common/calibration**
  * (B85/B87)Adjust user_calib_adc_vref and user_calib_freq_offset to be externally callable interfaces.
* **ADC**
  * (B85/B87)Integrate the complete configuration sampling IO operation from the adc_base_init interface to the adc_base_pin_init interface, so that the adc_base_pin_init interface can directly switch the sampling IO without calling the adc_base_init interface again.
  * (B85/B87)Integrate the complete configuration sampling IO operation from the adc_vbat_init interface to the adc_vbat_pin_init interface, so that the adc_vbat_pin_init interface can directly switch the sampling IO without calling the adc_vbat_init interface again.
  * (B80)Fixed that C5N in ADC_InputNchTypeDef is A3N, and C5P in ADC_InputPchTypeDef is A3P.
  * (B85)Move the code for configuring the sampling rate in the 'adc_base_init' and 'adc_vbat_init' interfaces to 'adc_init', reduce repetitive code.
* **ADC_Demo**
  * (B80/B87/B85)Fixed ADC_Demo default mode is ADC_BASE_MODE.
* **PM**
  * (B80)Add the ALL_SRAM_CODE macro definition to the cstartup_flash.S and cstartup_otp.S files. When the program is downloaded to flash or otp, it provides the choice of whether to move all programs to sram when the program is powered on for the first time;
* **License**
  * (B80/B87/B85)update license of related files from TELINK_BSD to TELINK_APACHE.
  
### Features

* **UART_Demo**
  * (B85/B87/B80)Added GPIO simulation UART RX function
* **FLASH**
  * (B80)Add flash GD25LD10C and GD25LD40C, delete flash P25D40L.
* **TIMER**
  * (B80)Added interface timer_set_irq_mask, timer_clr_irq_mask.
* **PM**
  * (B80)Added interface cpu_stall_wakeup.Interface usage can refer to PM_demo.
* **PM_Demo**
  * (B80)Added demo of using cpu_stall_wakeup interface, including using timer/stimer/RF/PAD to wake up.
* **ADC**
  * (B85)Added interface 'adc_sample_and_get_result_manual_mode', for single sampling.
  * (B85)Added the configuration of sampling rate 192KHz.
  * (B80)Added interface 'adc_set_gpio_calib_vref' and interface 'adc_set_vbat_calib_vref' for calibrating reference voltage.
* **CALIBRATION**
  * (B80)Added interface 'user_read_otp_value_calib'. The OTP package chip is used to calibrate various parameters. This time, the ADC calibration part and the frequency offset calibration part are updated.
  * (B80)Added interface 'user_read_flash_value_calib'. The FLASH package chip is used to calibrate various parameters. This time, the ADC calibration part and the frequency offset calibration part are updated.
  * (B80)Added interface 'user_calib_adc_vref'. Used to read the ADC calibration value and pass it to the ADC module. 
* **BQB_EMI_Demo(B85m)**
  * Synchronize frequency offset calibration address with calibration.h.

### Performance Improvements

* **OTP**
  * (B80)Code optimization, ramcode saves 56 bytes;
  * (B80)The otp_set_auto_pce_tcs interface must be called. after optimization, the otp_set_auto_pce_tcs interface can not use;

### Note
  *	(B80)PB0/PB3 defaults to high level when powered on. If you use this pin as ADC sampling, you need to wait for tens of milliseconds after IO is configured as GPIO before sampling is normal. 
  *      Therefore, you can try to use other available pins when designing products.
  * (B80)note the following two point need to noticed when using PB0, PB1, PB3, PD4 and PF0 GPIO ports:
  *  		1. These pins are not recommend to use as wake-up source;
  *  		2. Since these pins are output functions by default, even if they are configured with pull-up/pull-down retention,
  *  		when deep/deep Retention is invoked, they can't maintain high/low level and an abnormal level will occur.
  *  		Therefore, these pins can't be used in applications where a certain level state needs to be maintained all the time.


<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b85m_driver_sdk V1.4.0。
* 此版本SDK支持B80(A1)、B85、B87芯片。
* B80在V1.3.0版本的时候，只有A0芯片，只测试了A0，在V1.4.0版本的时候，我们只测试了A1版本的芯片。
* 根据芯片的改版说明，A0、A1是不需要软件针对芯片版本做特殊处理的，其实V1.3.0和V1.4.0都是可以用于A0和A1芯片的，
* 针对后续软件更新版本，只会对A1版本芯片进行验证，芯片版本标注也只会标注A1，建议按照标注的芯片版本进行使用。

### BREAKING CHANGES

* **FLASH**
  * (b85)修改mid分别为mid011460C8/mid136085/mid1360eb的midxxxx_otp_block_e和midxxxx_lock_otp_e元素名,将单位K改为B,避免误导OTP的大小。
* **ADC**
  * (B80)将完整的配置采样IO操作从adc_base_init接口整合到adc_base_pin_init接口，方便adc_base_pin_init接口直接切换采样的IO，无需再次调用adc_base_init接口。修改了传参类型'adc_input_pin_def_e pin'。
* **PM**
  * (B80)删除了接口cpu_stall_wakeup_by_timer0、cpu_stall_wakeup_by_timer1、cpu_stall_wakeup_by_timer2、cpu_stall。使用新接口cpu_stall_wakeup替代，使用方法可以参考pm demo。
* **cstartup_sram.S** 
  * (B80)删除cstartup_sram.S文件，功能合并至cstartup_flash.S和cstartup_otp.S文件内。 
   
### Bug Fixes

* **Cstartup_xx.S**
  * (B80)修复了在某些代码结构或者优化选项下，可能导致程序retention醒来后死机的问题
* **RF**
  * (B87/B80)修复了rf_set_tx_modulation_index，rf_set_rx_modulation_index设置MI为0.32，0.50以外值不准确的问题，同时根据需求添加新的挡位。
* **OTP**
  * (B80)如果使用OTP读写函数接口对如下起始地址范围进行读写：0x3ff4/0x3ff8，会触发异常，现已修正，该版本OTP可读写起始地址范围为0x0000~0x3ffc;
* **PM**
  * (B80)修复retention回来睡眠标志位pmParam.is_deepretn_back无效的问题;
* **EMI_Demo**
  * (B85/B87/B80)在gpio_shutdown之后添加了usb接口初始化的操作，以解决某些仅支持Swire(through-usb)通信方式的芯片在下载EMI程序之后无法进行Swire通信的问题。
	
### Refactoring

* **BQB_Demo**
  * (B85/B87/B80)修改了获取flash容量大小的写法，与flash.h中关于flash容量的描述保持一致。
* **EMI_Demo**
  * (B85/B87/B80)增加了读校准参数的接口read_calibration_flash。
  * (B85/B87/B80)修改了获取flash容量大小的写法，与flash.h中关于flash容量的描述保持一致。
* **Vendor/common/calibration**
  * (B85/B87)调整user_calib_adc_vref和user_calib_freq_offset为可被外部调用的接口。
* **ADC**
  * (B85/B87)将完整的配置采样IO操作从adc_base_init接口整合到adc_base_pin_init接口，方便adc_base_pin_init接口直接切换采样的IO，无需再次调用adc_base_init接口。
  * (B85/B87)将完整的配置采样IO操作从adc_vbat_init接口整合到adc_vbat_pin_init接口，方便adc_vbat_pin_init接口直接切换采样的IO，无需再次调用adc_vbat_init接口。
  * (B80)修正ADC_InputNchTypeDef中的C5N为A3N，而ADC_InputPchTypeDef中的 C5P 为 A3P。
  * (B85)将'adc_base_init'和'adc_vbat_init'接口中配置采样率的代码移动至'adc_init'中，减少重复代码。
* **ADC_Demo**
  * (B80/B87/B85)修正ADC_Demo默认模式为ADC_BASE_MODE。
* **PM**
  * (B80)在cstartup_flash.S和cstartup_otp.S文件中添加ALL_SRAM_CODE宏定义，当程序下载到flash或otp时，提供程序第一次上电是否将所有程序搬到sram中的选择；
* **license**
  * (B80/B87/B85)将相关文件的许可证从 TELINK_BSD 更新为 TELINK_APACHE
  

### Features

* **UART_Demo**
  * (B85/B87/B80)新增GPIO模拟UART RX功能
* **FLASH**
  * (B80)增加 flash GD25LD10C 和 GD25LD40C, 删除 flash P25D40L.
* **Timer**
  * (B80)新增接口timer_set_irq_mask、timer_clr_irq_mask。
* **PM**
  * (B80)新增接口cpu_stall_wakeup。接口使用可以参考PM_demo 。
* **PM_Demo**
  * (B80)新增cpu_stall_wakeup接口的使用demo，包括使用timer/stimer/RF/PAD唤醒。
* **ADC**
  * (B85)增加了接口'adc_sample_and_get_result_manual_mode'，用于单次采样。
  * (B85)增加了采样率192KHz的配置。
  * (B80)增加了接口'adc_set_gpio_calib_vref'和接口'adc_set_vbat_calib_vref',用于校准参考电压。
* **CALIBRATION**
  * (B80)增加了接口'user_read_otp_value_calib'。OTP封装芯片用于校准各种参数，这次更新了ADC校准部分和频偏校准部分。
  * (B80)增加了接口'user_read_flash_value_calib'。FLASH封装芯片用于校准各种参数，这次更新了ADC校准部分和频偏校准部分。
  * (B80)增加了接口'user_calib_adc_vref'。用于读取ADC校准值并传给ADC模块。
* **BQB_EMI_Demo**
  * (B85/B87/B80)与calibration.h同步频偏值校准值地址。

### Performance Improvements
* **OTP**
  * (B80)代码优化，ramcode节省56个字节;
  * (B80)之前otp读接口和clock_init中的otp_set_auto_pce_tcs接口存在关联关系，otp_set_auto_pce_tcs接口必须调用才可以，优化之后，otp_set_auto_pce_tcs接口调用不调用都可以;
  
### Note
* **ADC**
  * (B80)PB0/PB3上电默认为高电平，若使用该管脚作为ADC采样，需要在IO配置为GPIO后等几十毫秒，才能采样正常,所以产品设计的时候可以尽量考虑使用其他可用管脚。
  * (B80)注意在使用 PB0、PB1、PB3、PD4 和 PF0 GPIO 端口时需要注意以下两点：
  * 		1. 这些引脚不推荐用作唤醒源；
  *			2. 由于这些管脚默认为输出功能，即使配置了上拉/下拉保持，当调用deep/deep Retention时，它们不能保持高/低电平，会出现电平异常。
  *		   	   当调用 deep/deep Retention 时，它们无法保持高/低电平，会出现异常电平。因此，这些引脚不能用于需要一直保持某种电平状态的应用。
  
  

### Version

* SDK version: telink_b85m_driver_sdk V1.3.0.
* This version of SDK supports B80(A0), B85, and B87 chips.

### Bug Fixes

* **PM**
	* (B85) Fixed the problem that the actual sleep time is inaccurate when the 32k rc long sleep function is used, when the set sleep time is less than 2ms. (merge_requests/@123)
	* (B85/B87) Solve the problem that the wake-up source cannot be obtained in deep mode. (merge_requests/@235)
* **TIMER(B85/B87)**
	* Modify the timer0_gpio_init and timer1_gpio_init functions to fix the problem that the gpio interrupt will be accidentally touched when the gpio trigger and gpio width modes are used. (merge_requests/@117)
* **UART(B85/B87)**
	* Modify the function implementation of uart_init/uart_init_baudrate/uart_set_rts to avoid possible exceptions caused by register writing 0 status. (merge_requests/@122)
* **S7816(B85/B87)**
	* Modify the function implementation of s7816_set_clk to avoid possible exceptions caused by register writing 0 status. (merge_requests/@122)
* **SWIRE(B85/B87)**
	* Modify the function implementation of swire_set_slave_id to avoid possible exceptions caused by register writing 0 status. (merge_requests/@122)
* **IR_LEARN(B85/B87)**
	* Modify the function implementation of ir_learn_set_pin/ir_learn_set_mode/ir_learn_set_timeout to avoid possible abnormalities caused by register writing 0 status. (merge_requests/@122)
* **QDEC(B85/B87)**
	* The qdec_clear_conuter interface cannot realize the function of clearing the count. Delete it, add the qdec_reset interface, and use the qdec_reset interface to reset qdec and clear the qdec count value. (merge_requests/@108)
* **RF**
	* (B85/B87) Fix the problem that IO may be in an unstable state during the setting of rf_rffe_set_pin in B85 and B87. (merge_requests/@129)
	* (B87) Fixed the problem of getting the timestamp error from the received package through RF_ZIGBEE_PACKET_TIMESTAMP_GET/RF_TPLL_PACKET_TIMESTAMP_GET/RF_SB_PACKET_TIMESTAMP_GET and rf_aoa_aod_get_pkt_timestamp. (merge_requests/@205)
	* (B85/B87) Fixed the problem of abnormal packet receiving caused by calling rf_access_code_comm or rf_longrange_access_code_comm within 5us after calling rf_drv_init(RF_MODE_LR_S2_500K) or rf_drv_init(RF_MODE_LR_S8_125K). (merge_requests/@205)
* **GPIO(B85/B87)**
	* To realize the shutdown function, IO must be set to GPIO function, the original configuration is wrong. (merge_requests/@185)
	* Fixed the problem caused by error bit operation in the gpio_shutdown function, which will mistakenly shut down the input function of other pins in the same group of the selected pin. (merge_requests/@215)
* **PRINTF(B85/B87)**
	* Solved the problem that the printf function cannot print negative numbers. (merge_requests/@199)
* **EMI_Demo(B80/B85/B87)**
	* Added usb interface initialization after gpio_shutdown to solve the problem that some chips that only support Swire (through-usb) communication cannot communicate through Swire after downloading the EMI program.

### Features

* **(B80)**
    * ADC/AES/PWM/I2C/IR_LEARN/S7816/SPI/UART/USB/BQB/FLASH/KEYSCAN/RF/TIMER
* **ADC(B85/B87)**
	* Add two-point calibration setting offset interface adc_set_gpio_two_point_calib_offset for ADC GPIO sampling. (merge_requests/@224)
	* Modify the formula for calculating voltage in the interface adc_sample_and_get_result. Both single-point calibration and two-point calibration use this interface to obtain voltage values. (merge_requests/@224)
* **PKE(B87)**
	* The ECDH algorithm and ECDSA algorithm have increased the cases where the curves are 160, 192 and 224.
* **BQB(B85/B87)**
	*Modified the BQB configuration scheme to adapt to BDT/BQB_Tool.
* **vendor/common(B85/B87)/flash**
	* The new interface in the zb_cam_modify.c file is used to modify the cam value of the zb flash.
* **CLOCK(B85/B87)**
	* Three options of 4M, 6M and 8M have been added to the system clock configuration.
* **EMI(B85/B87)**
	* Added support for Private_1M and Private_2M modes.
* **BQB_EMI_Demo(B85/B87)**
	* Added a new PA configuration protocol to set the PA control pin.
	* Added the CLOSE_INTERNAL_CAP macro to choose whether to close the internal capacitor.
* **PM**
	* (B85) Add the long sleep interface for external 32K crystal oscillator-cpu_long_sleep_wakeup_32k_xtal. (merge_requests/@123)
	* (B85/B87) Added interface pm_set_power_cfg() to set whether to turn off the power of audio/zb/usb before sleep. By default, the power of the three modules is turned off to save power. (merge_requests/@132)
* **FLASH(B85/B87)**
	* Modify the process of TH25D40HB to SST. (merge_requests/@114)
	* Flash has added the function flash_get_vdd_f_calib_value() to read the vendor, and the vendor enumeration type flash_vendor_e. (merge_requests/@103)
	* For pointers to functions flash_read_page and flash_write_page, add function flash_change_rw_func() to re-assign them. (merge_requests/@103)
	* Optimize the demo.
* **S7816(B85/B87)**
	* Add gpio_set_irq_mask/gpio_clr_irq_mask/gpio_get_irq_status/gpio_clr_irq_status function interface. (merge_requests/@121)
* **RF(B85/B87)**
	* ATE test adds private 2M related test. (merge_requests/@136)
	* Added rf_set_rx_settle_time function to set the time as a parameter value. (merge_requests/@129)
	* The energy range between 3db and -5db in vbat mode is added for internal testing. These energies are forbidden to be used by customers when developing products.
* **DUT_Demo(B80)(B85)(B87)**
	* Users can develop fixture test programs according to their own needs.
### Known issues

* N/A

### BREAKING CHANGES

* **ADC(B85/B87)**
	* Delete variable adc_vref_cfg and interface adc_calib_vref_enable().
* **FLASH(B85/B87)**
	* VDDF calibration and ADC calibration were previously placed in the driver function cpu_wakeup_init(). Now they are taken out and placed in the upper user_read_flash_value_calib() function. Frequency offset calibration is also added to the user_read_flash_value_calib() function. Note: If you use the new version of the SDK, you need to call user_read_flash_value_calib() after cpu_wakeup_init(), and you must use it in this way, otherwise, abnormal functions or abnormalities may occur.
	* Delete ADC calibration enable adc_vref_cfg.adc_calib_en and use ADC calibration directly.
* **S7816(B87)**
	* Modify the meaning of the s7816_set_clk parameter, and the incoming parameter represents the frequency division number.
* **PKE(B87)**
	* Delete pke_get_irq_status, pke_clr_irq_status, pke_set_irq_mask, pke_clr_irq_mask, pke_set_microcode, pke_set_exe_cfg, pke_opr_start, pke_check_rt_code, pke_set_operand_width and pke_calc interface.
* **RF(B85/B87)**
	* Modify the second parameter type of rf_wire2_init function to GPIO_PinTypeDef type.
	* Rename the functions rf_wire2_init, rf_wire3_init to rf_2wire_pta_init, rf_3wire_pta_init; and modify the relevant comments.
	* The settling time of the function tx_settle_adjust is parameter +1, now it is changed to the same setting time as the parameter.
	* Some functions of the aoa algorithm have been renamed and the parameters have been modified, but the essence has not changed. For example, get_input_angle_for_polygon_new is changed to raw_data_to_angle_with_z and theta angle is obtained by passing parameters; get_input_angle_for_polygon is changed to raw_data_to_angle_no_z.

### Refactoring

* **SPI(B85/B87)**
	* The operation of clearing 0 and then writing the corresponding value is modified to a write once to avoid possible abnormalities caused by the state when the register is written to 0.
* **WATCHDOG(B85/B87)**
	* The operation of clearing 0 and then writing the corresponding value is modified to a write once to avoid possible abnormalities caused by the state when the register is written to 0.
* **QDEC(B85/B87)**
	* Change the parameter type char of the qdec_set_debouncing function to an enumerated type.
	* Delete the total_count global variable in qdec.c, so that the calculation of total_count in the qdec_get_count_value function is placed on the upper level.
* **FLASH(B85/B87)**
	* Put flash_read_page and flash_write_page in the retention section.
* **RF(B85/B87)**
	* Modify the implementation of rf_wire3_init.
	* Rewrite AOA-related operations into functions and add them to the rf driver.
* **vender/common(B87)**
	* Adjusted the code structure for taking ADC calibration value to distinguish vbat calibration value from gpio calibration value.
* **license**
    * update license of related files from TELINK_BSD to TELINK_APACHE

### Performance Improvements

* **GPIO(B85/B87)**
	* Modify the return type of the gpio_read function to bool type.
* **UART(B85/B87)**
	* Adjust the position of dma_chn_en and place it in the corresponding uart_send_dma/uart_recbuff_init interface.


<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b85m_driver_sdk V1.3.0。
* 此版本SDK支持B80(A0)、B85、B87芯片。

### Bug Fixes

* **PM**
	* (B85)修复了当使用32k rc长睡眠函数时，当设置的睡眠时间小于2ms时，实际睡眠时间不准的问题。(merge_requests/@123)
	* (B85/B87)解决deep模式下不能获取唤醒源的问题。(merge_requests/@235)
* **TIMER(B85/B87)**
	* 修改timer0_gpio_init和timer1_gpio_init函数，修复了使用gpio trigger和gpio width模式会误触gpio中断的问题。(merge_requests/@117)
* **UART(B85/B87)**
	* 修改uart_init/uart_init_baudrate/uart_set_rts的函数实现，避免存在寄存器写0状态可能造成的异常。(merge_requests/@122)
* **S7816(B85/B87)**
	* 修改s7816_set_clk的函数实现，避免存在寄存器写0状态可能造成的异常。(merge_requests/@122)
* **SWIRE(B85/B87)**
	* 修改swire_set_slave_id的函数实现，避免存在寄存器写0状态可能造成的异常。(merge_requests/@122)
* **IR_LEARN(B85/B87)**
	* 修改ir_learn_set_pin/ir_learn_set_mode/ir_learn_set_timeout的函数实现，避免存在寄存器写0状态可能造成的异常。(merge_requests/@122)
* **QDEC(B85/B87)**
	* qdec_clear_conuter接口不能实现清除计数的功能，将其删除，添加qdec_reset接口，使用qdec_reset接口来复位qdec,清除qdec计数值。(merge_requests/@108)
* **RF**
	* (B85/B87)修复B85，B87中rf_rffe_set_pin设置过程中可能会造成IO处于不定态的问题。(merge_requests/@129)
	* (B87)修复了通过RF_ZIGBEE_PACKET_TIMESTAMP_GET/RF_TPLL_PACKET_TIMESTAMP_GET/RF_SB_PACKET_TIMESTAMP_GET以及rf_aoa_aod_get_pkt_timestamp从收到的包中获取timestamp错误的问题。(merge_requests/@205)
	* (B85/B87)修复了调用rf_drv_init(RF_MODE_LR_S2_500K)或者rf_drv_init(RF_MODE_LR_S8_125K)之后，5us内调用rf_access_code_comm或者rf_longrange_access_code_comm造成收包异常的问题。(merge_requests/@205)
* **GPIO(B85/B87)**
	* 实现shutdown功能必须要将IO设为GPIO功能，原来的配置有误。(merge_requests/@185)
	* 修正了gpio_shutdown函数中由于错误位运算而引起的问题,会错误关闭被选中pin的同组其他pin的输入功能。(merge_requests/@215)
* **PRINTF(B85/B87)**
	* 解决了printf函数不能打印负数的问题。(merge_requests/@199)
* **EMI_Demo(B80/B85/B87)**
	* 在gpio_shutdown之后添加了usb接口初始化的操作，以解决某些仅支持Swire(through-usb)通信方式的芯片在下载EMI程序之后无法进行Swire通信的问题。

### Features

* **(B80)**
    * ADC/AES/PWM/I2C/IR_LEARN/S7816/SPI/UART/USB/BQB/FLASH/KEYSCAN/RF/TIMER
* **ADC(B85/B87)**
	* 增加ADC GPIO采样的两点校准设置offset的接口adc_set_gpio_two_point_calib_offset。(merge_requests/@224)
	* 修改接口adc_sample_and_get_result中计算电压的公式，单点校准和两点校准均使用该接口获取电压值。(merge_requests/@224)
* **PKE(B87)**	
	* ECDH算法和ECDSA算法增加了曲线为160，192和224的情况。
* **BQB(B85/B87)**	
	*修改了BQB的配置方案以适配BDT/BQB_Tool。
* **vendor/common(B85/B87)/flash**
	* zb_cam_modify.c文件中新增接口用于修改zb flash的cam值。
* **CLOCK(B85/B87)**	
	*系统时钟配置中增加了4M、6M、8M三个选项。
* **EMI(B85/B87)**
	* 增加了对Private_1M和Private_2M模式的支持。
* **BQB_EMI_Demo(B85/B87)**
	* 增加了新的PA配置协议，用来设置PA控制引脚。
	* 增加了CLOSE_INTERNAL_CAP宏用来选择是否关闭内部电容。
* **PM**
	* (B85)增加外部32K晶振的长睡眠接口 - cpu_long_sleep_wakeup_32k_xtal。(merge_requests/@123)
	* (B85/B87)新增接口pm_set_power_cfg()设置睡眠前是否关闭audio/zb/usb的power,默认三个模块的power都关闭以节省功耗.(merge_requests/@132)
* **FLASH(B85/B87)**
	* 修改TH25D40HB的工艺为SST。(merge_requests/@114)
	* flash添加了读供应商函数flash_get_vdd_f_calib_value()，和供应商枚举类型flash_vendor_e。(merge_requests/@103)
	* 函数flash_read_page和flash_write_page的指针，增加函数flash_change_rw_func()来对其重新赋值。(merge_requests/@103)
	* 优化demo。
* **S7816(B85/B87)**
	* 添加gpio_set_irq_mask/gpio_clr_irq_mask/gpio_get_irq_status/gpio_clr_irq_status函数接口。(merge_requests/@121)	
* **RF(B85/B87)**	
	* ATE测试新增private 2M相关的测试。(merge_requests/@136)	
	* 新增rf_set_rx_settle_time函数，设置时间为参数值。(merge_requests/@129)	
	* 增加了vbat模式3db到-5db之间的能量档位用于内部测试，这些能量禁止客户开发产品时使用。
* **DUT_Demo(B80)(B85)(B87)**
	* 用户可根据自身需要开发夹具测试程序。
### Known issues

* N/A

### BREAKING CHANGES

* **ADC(B85/B87)**
	* 删除变量adc_vref_cfg和接口adc_calib_vref_enable()。
* **FLASH(B85/B87)**
	* VDDF校准、ADC校准以前是放在驱动函数cpu_wakeup_init()中的，现在拿出来放在上层的user_read_flash_value_calib()函数中，频偏校准也加入到user_read_flash_value_calib()函数中。 注意：如果使用新版本SDK，需要在cpu_wakeup_init()后调用user_read_flash_value_calib()，必须按照这样使用，否则有可能出现功能不正常或者异常现象。
	* 删除ADC校准使能adc_vref_cfg.adc_calib_en，直接使用ADC校准。
* **S7816(B87)**
	* 修改s7816_set_clk参数含义，改为传入的参数就代表分频数。
* **PKE(B87)**
	* 删除pke_get_irq_status、pke_clr_irq_status、pke_set_irq_mask、pke_clr_irq_mask、pke_set_microcode、pke_set_exe_cfg、pke_opr_start、pke_check_rt_code、pke_set_operand_width和pke_calc_pre_mont等接口。
* **RF(B85/B87)**
	* 修改rf_wire2_init函数第二个参数类型为GPIO_PinTypeDef型。
	* 将函数rf_wire2_init，rf_wire3_init 改名为rf_2wire_pta_init，rf_3wire_pta_init；并修改相关注释。
	* 函数tx_settle_adjust的设置settle时间为参数+1，现在更改为设置时间与参数一致。
	* aoa算法部分函数进行了重新命名，并修改参数，但实质没有改变。如get_input_angle_for_polygon_new修改为raw_data_to_angle_with_z并将theta角通过传参获取；get_input_angle_for_polygon改为raw_data_to_angle_no_z。

### Refactoring

* **SPI(B85/B87)**
	* 先清0再写相应值的操作，修改为一次写入，避免寄存器写0时状态可能造成异常。
* **WATCHDOG(B85/B87)**
	* 先清0再写相应值的操作，修改为一次写入，避免寄存器写0时状态可能造成异常。
* **QDEC(B85/B87)**
	* 将qdec_set_debouncing函数参数类型char改为枚举类型。
	* 将qdec.c中的total_count全局变量删除，使得在qdec_get_count_value函数中total_count的计算实现放在上层体现。
* **FLASH(B85/B87)**	
	*把flash_read_page和flash_write_page放在retention段。
* **RF(B85/B87)**	
	* 修改rf_wire3_init的实现方式。
	* 将AOA相关的操作改写成函数，并添加到rf驱动中。
* **license**
    * 将相关文件的许可证从 TELINK_BSD 更新为 TELINK_APACHE
	
### Performance Improvements

* **GPIO(B85/B87)**
	* 修改gpio_read函数的返回类型为bool类型。
* **UART(B85/B87)**
	* 调整dma_chn_en的位置，将其放在对应的uart_send_dma/uart_recbuff_init接口中。

### Note

* N/A

---

## V1.2.0

### Version

* SDK version : telink_b85m_driver_sdk v1.2.0.
* This version sdk support B85,B87.

### Bug Fixes

* **PM(B87)**
  * In random.c,fix the wrong setting of dma length.
	* fix the issue that the program will crash in the case that:
    * run one of these case first
        * deep retention with 16K sram,timer wakeup only and the time < 2.5s.
        * deep retention with 32k sram,timer wakeup only and the time < 6s.
        * deep retention pad wakeup.
    * then run one of these case
        * deep retention with 16K sram,timer wakeup only and the time > 2.5s
        * deep retention with 32K sram,timer wakeup only and the time > 6s


### Features

* N/A

### Known issues

* N/A

### BREAKING CHANGES

* N/A

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b85m_driver_sdk v1.2.0。
* 此版本SDK支持B85、B87芯片。

### Bug Fixes

* **PM(B87)**
    * 解决在如下case中会死机的问题：
    * 先运行如下case之一
        * 跑一次deep_retention with 16k sram的纯timer唤醒且时间小于2.5s
        * 跑一次deep_retention with 32k sram的纯timer唤醒且时间小于6s
        * 跑一次deep_retention的pad唤醒
    * 再运行如下case之一
        * 跑一次deep_retention with 16k sram的纯timer唤醒且时间大于2.5s
        * 跑一次deep_retention with 32k sram的纯timer唤醒且时间大于6s

### Features

* N/A

### Known issues

* N/A

### BREAKING CHANGES

* N/A

### Refactoring

* N/A

### Note

* N/A

---

## V1.1.0

### Version

* SDK version : telink_b85m_driver_sdk v1.1.0.
* This version sdk support B85,B87.

### Bug Fixes

* **ADC_RANDOM(B85)**
  * In random.c,fix the wrong setting of dma length.
* **uart(B85/B87)**
  * Fix the issue that the program will generate an tx_interrupt in dma mode after initialization.
  * In UART-DMA mode,the DMA support sending 511-bytes one time at most in the previous version.From this version,DMA support sending 2047-bytes one time at most.
* **rf(B85/B87)**
  * Fixed that the reg_rf_irq_mask function interface mistakenly cleared other interrupts when clearing one interrupt.
* **pm(B85)**
  * After wakeup from deep retention,then enter deep mode will crash while wakeup.
* **flash(B85)** 
  * Fixed the hidden danger of possible errors in reading mid when the flash model is GD25LD80C, and adopted a more reliable method.

### Features

* **IR_LEARN(B87)**
  * Add IR learning function DEMO.
* **flash(B85/B87)**
  * Add Flash voltage VDD_F calibration in cpu_wakeup_init.
  * Add API for set flash voltage and to juidge whether it is zbit flash.
* **uart(B85/B87)** 
  * Add interface uart_send_dma().
  * uart_demo add 1-wire(RTX) communication mode.
* **rf(B85/B87)** 
  * Add api set the rf tx\rx modulation index: rf_set_tx_modulation_index and rf_set_rx_modulation_index.
* **pm(B85)** 
  * add long sleep api of cpu_long_sleep_wakeup.
* **spi(B87)**
  * Added the function for slave to process the received data in spi_demo.

### Known issues

* N/A

### BREAKING CHANGES

* **uart(B85/B87)**
  * the interface uart_dma_send() change to uart_send_dma() in the demo of UART-DMA mode.
* **flash(B87)**
  * Modify some Flash API usage for compitible.
  * The API of flash_read_status、flash_write_status not provide to external use,you need use the API in the directory of flash depend on mid(eg:flash_write_status_midxxxxxx).
  * The first argument of API int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid),flash_mid need 4byte space.The second argument need 16byte.
  * The API of flash_lock,flash_unlock will be instead of flash_lock_midxxxxxx,flash_unlock_midxxxxxx.
  * delete the useless API。

### Refactoring

* **flash(B85/B87)**
  * In order to reduce the size of ram_code, the code structure of flash has been re-adjusted.

### Note

* **flash(B85/B87)**
  * Before calling the FLASH function, please check the power supply voltage of the chip. Only if the detected voltage is greater than the safe voltage value, the FLASH function can be called. Taking into account the factors such as power supply fluctuations, the safe voltage value needs to be greater than the minimum chip operating voltage. For the specific value, please make a reasonable setting according to the specific application and hardware circuit.
  * Risk description: When the chip power supply voltage is relatively low, due to the unstable power supply, there may be a risk of error in the operation of the flash (especially for the write and erase operations. If an abnormality occurs, the firmware and user data may be rewritten, resulting in the final Product failure).

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b85m_driver_sdk v1.1.0。
* 此版本SDK支持B85、B87芯片。

### Bug Fixes

* **ADC_RANDOM(B85)**
  * 在random.c中，修复设置dma长度错误问题。
* **uart(B85/B87)**
  * 修复UART-DMA模式下程序会在初始化后产生一次TX中断的问题。
  * UART-DMA模式下，之前版本的UART-DMA发送数据长度最大为511-bytes。从此版本开始，UART-DMA模式发送数据长度最大为2047-bytes。
* **rf(B85/B87)**
  * 修复了reg_rf_irq_mask函数接口在清除一个中断时误清除其他中断。
* **pm(B85)**
  * 从deep retention唤醒后再进deep模式时，deep唤醒后死机。
* **flash(B85)** 
  * 修复了flash型号为GD25LD80C时，读mid有可能出错的隐患，采用了更可靠的方式。

### Features

* **IR_LEARN(B87)**
  * 增加了红外学习功能 DEMO。
* **flash(B85/B87)**
  * 在cpu_wakeup_init接口中增加VDD_F电压校准。
  * 新增调节flash电压以及判断是否为zbit flash的接口。
* **uart(B85/B87)** 
  * 新增接口uart_send_dma()。
  * uart_demo增加单线通信模式。
* **rf(B85/B87)** 
  * 新增设置rf tx\rx mi调制接口：rf_set_tx_modulation_index 和 rf_set_rx_modulation_index。
* **pm(B85)** 
  * 新增长睡眠函数cpu_long_sleep_wakeup。
* **spi(B87)**
  * 在spi_demo中新增了slave对接收到的数据进行处理的功能。

### Known issues

* N/A

### BREAKING CHANGES

* **uart(B85/B87)**
  * UART-DMA模式demo中接口uart_dma_send()更改为uart_send_dma()。
* **flash(B87)**
  * 为兼容不同的Flash型号，Flash驱动结构做了调整，修改了部分Flash接口调用方式。
  * 为兼容不同型号的Flash，flash_read_status、flash_write_status不提供给外部使用，需要使用对应接口时，需要根据mid去选择flash目录下的接口(例如：flash_write_status_midxxxxxx)。
  * 接口int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid)的第一个参数flash_mid需要4个字节空间，第二个参数需要16byte空间。
  * 接口flash_lock、flash_unlock由flash_lock_midxxxxxx、flash_unlock_midxxxxx替代。
  * 删除不使用的接口。

### Refactoring

* **flash(B85/B87)**
  * 为了减少ram_code大小，重新调整了flash的代码结构。

### Note

* **flash(B85/B87)**
  * 在调用FLASH 函数前，请先做芯片供电电压检测，当检测电压大于安全电压值时，才可以调用FLASH 函数。考虑到电源波动等因素，安全电压值需要比最低芯片工作电压大，具体设置为多少，请根据具体应用以及硬件电路等因素进行合理的设置。
  * 风险描述：当芯片供电电压比较低时，由于电源不稳定，flash的操作可能会有出错的风险（特别是写和擦操作，如果出现异常，可能会造成固件和用户数据被改写，从而导致最终产品失效）。

---

## V1.0.0

### Version
* SDK version : telink_b85m_driver_sdk v1.0.0.
* This version sdk support B85,B87.

### Bug Fixes

* **adc(B87)**
  * Fixed the problem that the adc adc_temp_result() return value should be a signed value for b87.
* **usb(B85/B87)**
  * The header file "stdDescriptors.h" is not case sensitive when included, so it is uniformly changed to "StdDescriptors.h".
* **uart(B85/B87)**: 
  * demo : Open the mask of rx_error in dma mode, otherwise the error status may not be cleared.

### Features

* **i2c/pwm/timer(B85/B87)**
  * add the relevant interrupt interface.
* **usb(B85/B87)**
  * Support usb cdc to send wMaxPacketSize (64bytes) length data.
* **uart(B85/B87)** 
  * add function uart_ndma_read_byte() and uart_ndma_clear_rx_index()。ndma mode can use this function to get data.
  * demo : add rx_error handling (when stop bit or parity error) in ndma mode.
  * add tx_done irq feature in dma mode.

### Refactoring

* **all** 
  * Kite, Vulture demo were combined into one, so that they could be shared.

### Breaking Changes

* **s7816(B85/B87)**
  * refacotr the demo and driver of s7816.
* **Flash(B85)**
  * Modify some Flash API usage for compitible:
  * void flash_read_mid(unsigned char* mid) change to unsigned int flash_read_mid(void),the mid from 3byte change to 4byte.
  * The API of flash_read_status、flash_write_status not provide to external use,you need use the API in the directory of flash depend on mid(eg:flash_write_status_midxxxxxx).
  * The first argument of API int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid),flash_mid need 4byte space.The second argument need 16byte,has two case,8byte or 16byte,if the flash only has 8byte uid,flash_uid[8:15] will be clear to zero.
  * The API of flash_lock,flash_unlock,flash_read_otp,flash_write_otp,flash_erase_otp,flash_lock_otp will be instead of flash_lock_midxxxxxx,flash_unlock_midxxxxxx,flash_read_otp_midxxxxxx,flash_write_otp_midxxxxxx,flash_erase_otp_midxxxxxx,flash_lock_otp_midxxxxxx.
  * delete the useless API。
* **RF-PTA(B87)**
  * API CHANGE：add PTA-PIN as the input parameter of function rf_wire2_init() and rf_wire3_init().

### Performance Improvements

* **flash(B85/B87)**
  * the function flash_write_page() support cross-page writing.

### Known issues

* N/A

  

<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">

### 版本

* SDK版本: telink_b85m_driver_sdk v1.0.0。
* 此版本SDK支持B85、B87芯片。

### Bug Fixes

* **adc(B87)**
  * 修复了b87 adc的adc_temp_result()返回值应该是signed类型的问题。
* **usb(B85/B87)**
  * 头文件"stdDescriptors.h",include时没有区分大小写，统一改成"StdDescriptors.h"。
* **uart(B85/B87)**
  * demo : dma模式打开rx_error的mask,否则有可能清除不了error status。

### Features

* **i2c/pwm/timer(B85/B87)**
  * 补充相关中断接口。
* **usb(B85/B87)**
  * 支持USB CDC发送wMaxPacketSize（64bytes）长度数据。
* **uart(B85/B87)**
  * 增加接口uart_ndma_read_byte()和uart_ndma_clear_rx_index()。用途：ndma模式接收数据可直接调用该函数。
  * demo : ndma模式增加rx_error(停止位或奇偶校验出错)中断处理。
  * 在dma模式下添加tx_done irq功能。

### Refactoring

* **all** 
  * 将kite、vulture的demo合并成一个，使得可以共用。
  
### Breaking Changes

* **s7816(B85/B87)**
  * 重构s7816的demo和driver。
* **Flash(B85)**
  * 为兼容不同的Flash型号，Flash驱动结构做了调整，修改了部分Flash接口调用方式：
  * void flash_read_mid(mid) 改为 unsigned int flash_read_mid(void),mid由3byte改为4byte,最高byte用于区分mid相同但是功能存在差异的flash。
  * 为兼容不同型号的Flash,flash_read_status、flash_write_status不提供给外部使用，需要使用对应接口时，需要根据mid去选择flash目录下的接口(例如：flash_write_status_midxxxxxx)。
  * 接口int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid)的第一个参数flash_mid需要4个字节空间，第二个参数需要16byte空间，现有flash的uid有两种情况，一种16byte，一种8byte，如果是8byte，flash_uid[8:15]会被清零。
  * 接口flash_lock、flash_unlock、flash_read_otp、flash_write_otp、flash_erase_otp、flash_lock_otp由flash_lock_midxxxxxx、flash_unlock_midxxxxxx、flash_read_otp_midxxxxxx、flash_write_otp_midxxxxxx、flash_erase_otp_midxxxxxx、flash_lock_otp_midxxxxxx替代。
  * 删除不使用的接口。
* **RF-PTA(B87)**
  * 接口修改：新增PTA-PIN值作为参数传入函数rf_wire2_init()和rf_wire3_init()。

### Performance Improvements

* **flash(B85/B87)**
  * flash_write_page()函数支持跨page写。

### Known issues

* N/A


