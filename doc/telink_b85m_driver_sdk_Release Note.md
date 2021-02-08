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
