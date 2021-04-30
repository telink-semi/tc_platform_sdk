
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
  * void flash_read_mid(unsigned char* mid) 改为 unsigned int flash_read_mid(void),mid由3byte改为4byte,最高byte用于区分mid相同但是功能存在差异的flash。
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
