# CST816S
 An Arduino library for the CST816S capacitive touch screen IC
 
 [![arduino-library-badge](https://www.ardu-badge.com/badge/CST816S.svg?)](https://www.arduinolibraries.info/libraries/cst816-s)

## Auto Sleep Control

Auto Sleep is referred to as Standby Mode in this [Waveshare document](https://www.waveshare.com/w/upload/5/51/CST816S_Datasheet_EN.pdf). Disabling of auto sleep or auto standby will keep the touch display in Dynamic mode. This will improve responsiveness, at the cost of about ~1.6mA.

By default, auto sleep is **enabled** with a timeout of 2 seconds. The following functions allow you to manage auto sleep behavior:

- **`void disable_auto_sleep();`**  
  Disables auto sleep, keeping the device active indefinitely.

- **`void enable_auto_sleep();`**  
  Re-enables auto sleep with the current timeout (default is 2 seconds).

- **`void set_auto_sleep_time(int seconds);`**  
  Sets the auto sleep timeout in seconds (1-255).  
  Example: `set_auto_sleep_time(10);` sets a 10-second timeout.

## User-Provided Interrupt

The CST816S library allows you to attach a custom interrupt function to handle touch events according to your application's needs. By providing a user-defined interrupt, you can trigger specific actions upon touch events, such as waking the device from a low-power state, checking gestures, or executing custom logic without constantly polling the device.

**Feature Highlights:**
- **Custom Event Handling**: Define and attach a user function that executes on each touch interrupt, allowing immediate responses to touch events.
- **Power Management**: Ideal for applications needing to manage power, such as waking from sleep modes, as the interrupt triggers only on touch.
- **Gesture-Based Logic**: Use the interrupt to wake, then analyze gestures to decide on further actions, enabling efficient and gesture-responsive behavior.



 ## Register Information

 The following information was extracted from [this document](https://www.waveshare.com/w/upload/c/c2/CST816S_register_declaration.pdf) provided by Waveshare.

 | Register Name     | Address | Bit Description                                       | Default Value | Configuration Options                                          |
|-------------------|---------|-------------------------------------------------------|---------------|----------------------------------------------------------------|
| GestureID         | 0x01    | Gesture code for detected gestures                    | N/A           | - 0x00: None  <br> - 0x01: Slide Up  <br> - 0x02: Slide Down  <br> - 0x03: Slide Left  <br> - 0x04: Slide Right  <br> - 0x05: Single Click  <br> - 0x0B: Double Click  <br> - 0x0C: Long Press |
| FingerNum         | 0x02    | Number of fingers detected                            | N/A           | - 0: No finger <br> - 1: One finger                            |
| XposH             | 0x03    | High 4 bits of X coordinate                           | N/A           | -                                                              |
| XposL             | 0x04    | Low 8 bits of X coordinate                            | N/A           | -                                                              |
| YposH             | 0x05    | High 4 bits of Y coordinate                           | N/A           | -                                                              |
| YposL             | 0x06    | Low 8 bits of Y coordinate                            | N/A           | -                                                              |
| BPC0H             | 0xB0    | High 8 bits of BPC0 value                             | N/A           | -                                                              |
| BPC0L             | 0xB1    | Low 8 bits of BPC0 value                              | N/A           | -                                                              |
| BPC1H             | 0xB2    | High 8 bits of BPC1 value                             | N/A           | -                                                              |
| BPC1L             | 0xB3    | Low 8 bits of BPC1 value                              | N/A           | -                                                              |
| ChipID            | 0xA7    | Chip model identifier                                 | N/A           | -                                                              |
| ProjID            | 0xA8    | Project number                                        | N/A           | -                                                              |
| FwVersion         | 0xA9    | Firmware version number                               | N/A           | -                                                              |
| MotionMask        | 0xEC    | Enable continuous gesture actions and double-click    | 0x00          | - Bit 0: EnDClick (enable double-click) <br> - Bit 1: EnConUD (enable continuous up/down swipe) <br> - Bit 2: EnConLR (enable continuous left/right swipe) |
| IrqPluseWidth     | 0xED    | Interrupt pulse width (0.1 ms units)                  | 10            | 1-200                                                          |
| NorScanPer        | 0xEE    | Normal scan period (10 ms units)                      | 1             | 1-30                                                           |
| MotionSlAngle     | 0xEF    | Gesture detection sliding angle control (Angle = tan(c) * 10, where c is the angle relative to the positive X-axis) | N/A           | - |
| LpScanRaw1H       | 0xF0    | High 8 bits of low-power scan channel 1 reference      | N/A           | -                                                              |
| LpScanRaw1L       | 0xF1    | Low 8 bits of low-power scan channel 1 reference       | N/A           | -                                                              |
| LpScanRaw2H       | 0xF2    | High 8 bits of low-power scan channel 2 reference      | N/A           | -                                                              |
| LpScanRaw2L       | 0xF3    | Low 8 bits of low-power scan channel 2 reference       | N/A           | -                                                              |
| LpAutoWakeTime    | 0xF4    | Low-power mode auto recalibration interval (in minutes) | 5 minutes     | 1-5 minutes                                                    |
| LpScanTH          | 0xF5    | Low-power scan wake threshold. The smaller the value, the more sensitive it is. | 48            | 1-255                                                          |
| LpScanWin         | 0xF6    | Low-power scan range. The larger the value, the more sensitive it is, but power consumption increases. | 3             | 0-3                                                            |
| LpScanFreq        | 0xF7    | Low-power scan frequency. The smaller the value, the more sensitive it is. | 7             | 1-255                                                          |
| LpScanIdac        | 0xF8    | Low-power scan current. The smaller the value, the more sensitive it is. | N/A           | 1-255                                                          |
| AutoSleepTime     | 0xF9    | Time in seconds before entering standby mode after inactivity | 2 seconds     | 1-255 seconds                                                  |
| IrqCtl            | 0xFA    | Control of interrupt behavior                         | N/A           | - Bit 7: EnTest (Enable test, periodically sends low pulses) <br> - Bit 6: EnTouch (Sends low pulse on touch detection) <br> - Bit 5: EnChange (Sends low pulse on touch state change) <br> - Bit 4: EnMotion (Sends low pulse on gesture detection) <br> - Bit 0: OnceWLP (Sends one low pulse on long press) |
| AutoReset         | 0xFB    | Auto-reset time after detecting touch with no valid gesture | 5 seconds     | - 0: Disable <br> - 1-5: Time in seconds before auto-reset      |
| LongPressTime     | 0xFC    | Time (in seconds) for a long press to trigger reset    | 10 seconds    | - 0: Disable <br> - 1-10: Long press duration in seconds         |
| IOCtl             | 0xFD    | IO control settings including soft reset and power options | N/A           | - Bit 2: SOFT_RST (0: Disable soft reset, 1: Enable soft reset) <br> - Bit 1: IIC_OD (0: Pull-up resistor, 1: Open-drain) <br> - Bit 0: En1v8 (0: VDD, 1: 1.8V) |
| DisAutoSleep      | 0xFE    | Disable automatic entry into low-power mode            | 0 (enabled)   | - 0: Enable auto sleep <br> - Non-zero: Disable auto sleep       |
