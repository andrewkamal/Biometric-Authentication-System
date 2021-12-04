# Biometric-Authentication-System
## - Group Members
- Andrew Fahmy
- Nada Badawy
- Rinal Mohamed
## - Project Idea
![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/door%20lock.jpg)
A biometric sytem that takes optical fingerprint as input to authenticate the user and in return open an electric lock, as well as register the time the user came in.
In addtion, we alert the admin using an alarm and a wifi chip if there is a user trying to access the facility with three times unrecognized fingerprints scans in a row.
## - Hardware Requirements
- ESP32
- Nucleo STM32 Board
- Passive Buzzer
- JM101-B FingerPrint Sensor
- LCD
- Push Buttons
- LEDs

## - Software Requirements

- Stm32CubeMX
- Keil uVision
- TeraTerm

## - System Design
![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/Updated%20Diagram.png)

## - Activity Design
![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/Activity_diagram.png)

## - JM101B Sensor
The fingerprint module used in this project is of type JM101-B (datasheet attached below),  it uses UART protocol to communicate with the microcontroller (STM32). The communication is done by having the microcontroller sensor transmit a particular command ( the available commands are described below in details) to the Sensor and the sensor then generates an acknowledgment packet that acknowledges the receipt of the command packet sent from the sensor and indicates whether the command has been processed successfully and did the intended function or not through the confirmation byte (byte 9) that is included in the acknowledgment frame.
![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/FP_.jpg)
## The communication between the STM32 and the sensor:

## - PN532 Sensor
Our Near Field Communication (NFC) sensor is the PN532 using V3 Module. The PN532 was not the only module we came across in the market, we also stumbled upon the RC522 when looking for one to buy. When comparing between both sensors, we figured that the PN532 module supports SPI, I2C, and UART communication, while the RC522 only supports SPI. In addition to the NFC reading, the SPI supports both RFID read and write. Hence, we moved forward with the former and chose the SPI configuration instead of the I2C.

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/NFC_Module.jpg)

After thoroughly investigating the PN532 datasheet, we discovered that according to which communication the sensor will be used for, we need to set two switches builtin as 1-ch1 & 0-ch2 when using the I2C or 0-ch1 & 1-ch2 for the SPI.

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/NFC_STM32.png)

We also need to mention that in the STM32 CubeMx configuration, the Prescaler in TIM2 was set to 720, and the AutoReloadRegister (ARR) to 1999. The SPI1 was set to Full-Duplex Master Mode, with a data size of 8 Bits instead of the default 4 and follows an LSB First not MSB. The clock parameters of the SPI1 was set for 128 in Prescaler to support 562.5 KBits/s. It is recommended to allow a high level of Prescaler here since when we tried to configure the system when it was low, the sensor did not function properly as it should. To display the uid that is read with the sensor, we enabled USART2 to display what is read in TeraTerm. Nevertheless, in the GPIO settings, we added one GPIO input to the RST state and then we added another that was not used in case we needed to configure a button for a certain task. Also, we included four GPIO outputs for the IRQ, SS, the Green and the Red LEDS, where one indicates success while the other indicates failure. The Clock configuration was set to 72 Mhz instead of the max 80 Mhz to settle the APB to a certain value that is supported by the sensor.

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/NFC_ProtoType.jpg)

## - ESP32 and Telegram 
ESP32 module is used in the project to support WIFI connectivity and enable the communication with the admin of the system at all times. The admin of the system will be notified through the ESP32 module of any fraud activity detected at the site being monitored. Fraud activity is indicated by having three unsuccessful fingerprint scans in a row. In this case, the STM32 μC will set a GPIO pin (flag pin) to one once the count of unsuccessful scans reaches 3. This flag pin will be the input of a GPIO pin in the ESP32 module and will be read continuously in the code of the ESP module to push a notification to the admin through Telegram to alert him of what’s happening at the site as shown in the diagram below.
 ![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/esp.png)

The admin also has the ability to send different commands to the system to control a number of physical elements that exist at the facility. For example, the admin can send a command to turn on all the emergency LEDs at the facility to alert the security staff at any point in time. 
The admin can also send a command to declare a normal state at the site. Additionally, he can inquire about the state of the system at any point in time to see whether it’s in an emergency state or a normal state.

Supported commands:

Alert_security_staff

Declare_normal_state

Get_state

