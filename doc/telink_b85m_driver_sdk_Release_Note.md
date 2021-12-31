### Version

* SDK version: telink_b85m_driver_sdk v1.3.0.
* This version of SDK supports B80, B85, and B87 chips.

### Bug Fixes

* **PM**
	* (B85) Fixed the problem that the actual sleep time is inaccurate when the 32k rc long sleep function is used, when the set sleep time is less than 2ms. ([d0ecde8d](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/d0ecde8dab1f6e3debe99e3f8a7971cbfbf68d0a))
	* (B85/B87) Solve the problem that the wake-up source cannot be obtained in deep mode. ([c0a4b51e](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/c0a4b51ea7a32bc7ce6c35acc3b42aacc78f2da3))
* **TIMER(B85/B87)**
	* Modify the timer0_gpio_init and timer1_gpio_init functions to fix the problem that the gpio interrupt will be accidentally touched when the gpio trigger and gpio width modes are used. ([83f9e6d4](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/83f9e6d49ddcaa47dece25b155a3500c68e87d37))
* **UART(B85/B87)**
	* Modify the function implementation of uart_init/uart_init_baudrate/uart_set_rts to avoid possible exceptions caused by register writing 0 status. ([14de20b1](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/14de20b1ce383ca1c7b2d3762edf0c9905ca37a8))
* **S7816(B85/B87)**
	* Modify the function implementation of s7816_set_clk to avoid possible exceptions caused by register writing 0 status. ([14de20b1](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/14de20b1ce383ca1c7b2d3762edf0c9905ca37a8))
* **SWIRE(B85/B87)**
	* Modify the function implementation of swire_set_slave_id to avoid possible exceptions caused by register writing 0 status. ([14de20b1](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/14de20b1ce383ca1c7b2d3762edf0c9905ca37a8))
* **IR_LEARN(B85/B87)**
	* Modify the function implementation of ir_learn_set_pin/ir_learn_set_mode/ir_learn_set_timeout to avoid possible abnormalities caused by register writing 0 status. ([14de20b1](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/14de20b1ce383ca1c7b2d3762edf0c9905ca37a8))
* **QDEC(B85/B87)**
	* The qdec_clear_conuter interface cannot realize the function of clearing the count. Delete it, add the qdec_reset interface, and use the qdec_reset interface to reset qdec and clear the qdec count value. ([5f5ed691](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/5f5ed69104a9e64dd3bd16233c958375a3bba4b0))
* **RF**
	* (B85/B87) Fix the problem that IO may be in an unstable state during the setting of rf_rffe_set_pin in B85 and B87. ([1ff99a4f](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/1ff99a4fb72836352f6b5e314032d787ad200577))
	* (B87) Fixed the problem of getting the timestamp error from the received package through RF_ZIGBEE_PACKET_TIMESTAMP_GET/RF_NRF_ESB_PACKET_TIMESTAMP_GET/RF_NRF_SB_PACKET_TIMESTAMP_GET and rf_aoa_aod_get_pkt_timestamp. ([3a281ed6](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/3a281ed695326e686b55bcb2d653fa214b3d0ae4))
	* (B85/B87) Fixed the problem of abnormal packet receiving caused by calling rf_access_code_comm or rf_longrange_access_code_comm within 5us after calling rf_drv_init(RF_MODE_LR_S2_500K) or rf_drv_init(RF_MODE_LR_S8_125K). ([3a281ed6](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/3a281ed695326e686b55bcb2d653fa214b3d0ae4))
* **GPIO(B85/B87)**
	* To realize the shutdown function, IO must be set to GPIO function, the original configuration is wrong. ([1b797a29](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/1b797a292c8b242f799cbda46cc7dcb70acb7400))
	* Fixed the problem caused by error bit operation in the gpio_shutdown function, which will mistakenly shut down the input function of other pins in the same group of the selected pin. ([5597d497](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/5597d49790afb81a818281cff3dedd026c7e63fb))
* **PRINTF(B85/B87)**
	* Solved the problem that the printf function cannot print negative numbers. ([bc795e5a](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/bc795e5a9da97f8f2c13f5de60659f901b0760eb))

### Features

* **(B80)**
    * ADC/AES/PWM/I2C/IR_LEARN/S7816/SPI/UART/USB/BQB/FLASH/KEYSCAN/RF/TIMER
* **ADC(B85/B87)**
	* Add two-point calibration setting offset interface adc_set_gpio_two_point_calib_offset for ADC GPIO sampling. ([1621354c](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/1621354c7316ccc9d0862c7cfba82e3720da1920))
	* Modify the formula for calculating voltage in the interface adc_sample_and_get_result. Both single-point calibration and two-point calibration use this interface to obtain voltage values. ([1621354c](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/1621354c7316ccc9d0862c7cfba82e3720da1920))
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
	* (B85) Add the long sleep interface for external 32K crystal oscillator-cpu_long_sleep_wakeup_32k_xtal. ([d0ecde8d](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/d0ecde8dab1f6e3debe99e3f8a7971cbfbf68d0a))
	* (B85/B87) Added interface pm_set_power_cfg() to set whether to turn off the power of audio/zb/usb before sleep. By default, the power of the three modules is turned off to save power. ([164805ab](http://192.168.48.36 /src/driver/telink_b85m_driver_src/commit/164805ab655c4ba94aae67f1bc54cec9d27936c7))
* **FLASH(B85/B87)**
	* Modify the process of TH25D40HB to SST. ([8b722091](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/8b7220913ba0061c1d5db80ff4a7c53b8b8f1015))
	* Flash has added the function flash_get_vdd_f_calib_value() to read the vendor, and the vendor enumeration type flash_vendor_e. ([09687afd](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/09687afd1141e5ef2bf26ff14c5b2cdb84854af2))
	* For pointers to functions flash_read_page and flash_write_page, add function flash_change_rw_func() to re-assign them. ([09687afd](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/09687afd1141e5ef2bf26ff14c5b2cdb84854af2))
	* Optimize the demo.
* **S7816(B85/B87)**
	* Add gpio_set_irq_mask/gpio_clr_irq_mask/gpio_get_irq_status/gpio_clr_irq_status function interface. ([7b87c367](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/7b87c367b027f2be8a8bf3db367f68d8197186ff))
* **RF(B85/B87)**
	* ATE test adds private 2M related test. ([a62929a7](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/a62929a7a0e6b69669c60231a0a414dd54fcb2c2))
	* Added rf_set_rx_settle_time function to set the time as a parameter value. ([1ff99a4f](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/1ff99a4fb72836352f6b5e314032d787ad200577))
	* The energy range between 3db and -5db in vbat mode is added for internal testing. These energies are forbidden to be used by customers when developing products.

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

### Performance Improvements

* **GPIO(B85/B87)**
	* Modify the return type of the gpio_read function to bool type.
* **UART(B85/B87)**
	* Adjust the position of dma_chn_en and place it in the corresponding uart_send_dma/uart_recbuff_init interface.


<hr style="border-bottom:2.5px solid rgb(146, 240, 161)">


### 版本

* SDK版本: telink_b85m_driver_sdk v1.3.0。
* 此版本SDK支持B80、B85、B87芯片。

### Bug Fixes

* **PM**
	* (B85)修复了当使用32k rc长睡眠函数时，当设置的睡眠时间小于2ms时，实际睡眠时间不准的问题。([d0ecde8d](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/d0ecde8dab1f6e3debe99e3f8a7971cbfbf68d0a))
	* (B85/B87)解决deep模式下不能获取唤醒源的问题。([c0a4b51e](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/c0a4b51ea7a32bc7ce6c35acc3b42aacc78f2da3))
* **TIMER(B85/B87)**
	* 修改timer0_gpio_init和timer1_gpio_init函数，修复了使用gpio trigger和gpio width模式会误触gpio中断的问题。([83f9e6d4](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/83f9e6d49ddcaa47dece25b155a3500c68e87d37))
* **UART(B85/B87)**
	* 修改uart_init/uart_init_baudrate/uart_set_rts的函数实现，避免存在寄存器写0状态可能造成的异常。([14de20b1](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/14de20b1ce383ca1c7b2d3762edf0c9905ca37a8))
* **S7816(B85/B87)**
	* 修改s7816_set_clk的函数实现，避免存在寄存器写0状态可能造成的异常。([14de20b1](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/14de20b1ce383ca1c7b2d3762edf0c9905ca37a8))
* **SWIRE(B85/B87)**
	* 修改swire_set_slave_id的函数实现，避免存在寄存器写0状态可能造成的异常。([14de20b1](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/14de20b1ce383ca1c7b2d3762edf0c9905ca37a8))
* **IR_LEARN(B85/B87)**
	* 修改ir_learn_set_pin/ir_learn_set_mode/ir_learn_set_timeout的函数实现，避免存在寄存器写0状态可能造成的异常。([14de20b1](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/14de20b1ce383ca1c7b2d3762edf0c9905ca37a8))
* **QDEC(B85/B87)**
	* qdec_clear_conuter接口不能实现清除计数的功能，将其删除，添加qdec_reset接口，使用qdec_reset接口来复位qdec,清除qdec计数值。([5f5ed691](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/5f5ed69104a9e64dd3bd16233c958375a3bba4b0))
* **RF**
	* (B85/B87)修复B85，B87中rf_rffe_set_pin设置过程中可能会造成IO处于不定态的问题。([1ff99a4f](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/1ff99a4fb72836352f6b5e314032d787ad200577))
	* (B87)修复了通过RF_ZIGBEE_PACKET_TIMESTAMP_GET/RF_NRF_ESB_PACKET_TIMESTAMP_GET/RF_NRF_SB_PACKET_TIMESTAMP_GET以及rf_aoa_aod_get_pkt_timestamp从收到的包中获取timestamp错误的问题。([3a281ed6](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/3a281ed695326e686b55bcb2d653fa214b3d0ae4))
	* (B85/B87)修复了调用rf_drv_init(RF_MODE_LR_S2_500K)或者rf_drv_init(RF_MODE_LR_S8_125K)之后，5us内调用rf_access_code_comm或者rf_longrange_access_code_comm造成收包异常的问题。([3a281ed6](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/3a281ed695326e686b55bcb2d653fa214b3d0ae4))
* **GPIO(B85/B87)**
	* 实现shutdown功能必须要将IO设为GPIO功能，原来的配置有误。([1b797a29](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/1b797a292c8b242f799cbda46cc7dcb70acb7400))
	* 修正了gpio_shutdown函数中由于错误位运算而引起的问题,会错误关闭被选中pin的同组其他pin的输入功能。([5597d497](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/5597d49790afb81a818281cff3dedd026c7e63fb))
* **PRINTF(B85/B87)**
	* 解决了printf函数不能打印负数的问题。([bc795e5a](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/bc795e5a9da97f8f2c13f5de60659f901b0760eb))

### Features

* **(B80)**
    * ADC/AES/PWM/I2C/IR_LEARN/S7816/SPI/UART/USB/BQB/FLASH/KEYSCAN/RF/TIMER
* **ADC(B85/B87)**
	* 增加ADC GPIO采样的两点校准设置offset的接口adc_set_gpio_two_point_calib_offset。([1621354c](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/1621354c7316ccc9d0862c7cfba82e3720da1920))
	* 修改接口adc_sample_and_get_result中计算电压的公式，单点校准和两点校准均使用该接口获取电压值。([1621354c](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/1621354c7316ccc9d0862c7cfba82e3720da1920))
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
	* (B85)增加外部32K晶振的长睡眠接口 - cpu_long_sleep_wakeup_32k_xtal。([d0ecde8d](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/d0ecde8dab1f6e3debe99e3f8a7971cbfbf68d0a))
	* (B85/B87)新增接口pm_set_power_cfg()设置睡眠前是否关闭audio/zb/usb的power,默认三个模块的power都关闭以节省功耗.([164805ab](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/164805ab655c4ba94aae67f1bc54cec9d27936c7))
* **FLASH(B85/B87)**
	* 修改TH25D40HB的工艺为SST。([8b722091](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/8b7220913ba0061c1d5db80ff4a7c53b8b8f1015))
	* flash添加了读供应商函数flash_get_vdd_f_calib_value()，和供应商枚举类型flash_vendor_e。([09687afd](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/09687afd1141e5ef2bf26ff14c5b2cdb84854af2))
	* 函数flash_read_page和flash_write_page的指针，增加函数flash_change_rw_func()来对其重新赋值。([09687afd](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/09687afd1141e5ef2bf26ff14c5b2cdb84854af2))
	* 优化demo。
* **S7816(B85/B87)**
	* 添加gpio_set_irq_mask/gpio_clr_irq_mask/gpio_get_irq_status/gpio_clr_irq_status函数接口。([7b87c367](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/7b87c367b027f2be8a8bf3db367f68d8197186ff))	
* **RF(B85/B87)**	
	* ATE测试新增private 2M相关的测试。([a62929a7](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/a62929a7a0e6b69669c60231a0a414dd54fcb2c2))	
	* 新增rf_set_rx_settle_time函数，设置时间为参数值。([1ff99a4f](http://192.168.48.36/src/driver/telink_b85m_driver_src/commit/1ff99a4fb72836352f6b5e314032d787ad200577))	
	* 增加了vbat模式3db到-5db之间的能量档位用于内部测试，这些能量禁止客户开发产品时使用。

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


