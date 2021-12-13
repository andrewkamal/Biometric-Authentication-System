# Biometric-Authentication-System
Github repo: https://github.com/andrewkamal/Biometric-Authentication-System
## Group Members
- Andrew Fahmy
- Nada Badawy
- Rinal Mohamed
## Project Idea

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/door%20lock.jpg)

This project aims to implement a biometric-based authentication system that uses a fingerprint sensor to authenticate users into any physical space. The system is controlled by an admin who can communicate with the system all the time through a wifi chip that is integrated into the system and is controlled by the admin through Telegram. The admin of the system is alerted through Telegram if a fraud activity gets detected at the site. Fraud activities in these scenarios are indicated by having an unsuccessful fingerprint three times in a row.

## Hardware Requirements
- ESP32
- Nucleo STM32 Board
- Potentiometer
- JM101-B FingerPrint Sensor
- LCD
- Push Buttons
- LEDs
- PN532

## Software Requirements

- Stm32CubeMX
- Keil uVision
- TeraTerm
- Arduino IDE


## System Design

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/final%20ACdiagram.png)

For our system Design, we made use of input modules such as the JM101-B FingerPrint sensor, the push buttons to check the validity of a the scanned finger, or to add a new finger to the system. We are using two STM32 Nucleo boards to configure, manage and implement our system. The first is purely used for both the LCD as well as the fingerPrint sensor. The second board is used for the implementation of the SPI configuration of the PN532 sensor. Our outputs are mainly the led's either green for success or red for failure. Also, the pushed notifications to the admin's telegram to notify in case of three time unsuccessful attempts.

## Overall System Activity Diagram 
The activity diagram of the main system represents the very high level flow of the program. At the very beginning, if the FP system is working properly, the FP sensor will be the main system used for authentication. If there are any problems with the FP sensor, the NFC system will be used as the fail safe system to authenticate the users and replace the functionality of the FP sensor temporarily not to cause any inconveniences for users.

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/All%20system.png)

## Main system Activity Design

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/system1.png)


## Fail Safe Activity Diagrm 
![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/nfc.png)

## JM101B Sensor
The fingerprint module used in this project is of type JM101-B (datasheet attached below),  it uses UART protocol to communicate with the microcontroller (STM32). The communication is done by having the microcontroller transmit a particular command ( the available commands are described below in details) to the Sensor and the sensor then generates an acknowledgment packet that acknowledges the receipt of the command packet sent from the microcontroller and indicates whether the command has been processed successfully and did the intended function or not through the confirmation byte (byte 9) that is included in the acknowledgment frame.

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/FP_.jpg)

## Main System connections and Setup:
![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/JM101B.PNG)


## Supported commands:
- Registration (Add Fingerprint)
- Fingerprint verification
- Delete Fingerprint by ID
- Delete all Fingerprints

## Implemented and Tested commands:
We implement and test the Registration process to add a new fingerprint to the sensor. In order to do so we send a registration command to the sensor as follows

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/Command.PNG)

Then we checked if the Sensor responded with success or failure by checking the byte 9 from the Response packet.
Success → 0x00
Failure to generate feature → 0x02

## SOLVED ISSUES: 
- We have been facing an issue reading the acknowledgment packet because we used to receive one byte at a time while the acknowledgment frame gets generated as a whole packet and should be received all at once. 
- The LCD was defaulted with low contrast, so the displayed characters were not able to be seen. To solve this problem, we used a potentiometer to increase its contrast to be able to see the characters. 

## CubMX project: 

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/CubMX.PNG)

After interfacing the sensor with the STM32 we interface our system with LCD to act as a user interface. 

## Main System connections and Setup:

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/mainsys.jpeg)
## the component used: 
- LCD: act like a user interface to notify the users how to deal with the sensor
- JM101B Sensor:  biometric fingerprint sensor which add and verify users according to their fingerprints
- Push Buttons: there are 2 push buttons which help the user to choose from one of the modes ( ADD or Verify)
- Leds: two leds; the red indicates Access Denied and the red indicates access Granted 
- Potentiometer: to udjust the contrast of the LCD 

## PN532 Sensor
Our Near Field Communication (NFC) sensor is the PN532 using V3 Module. The PN532 was not the only module we came across in the market, we also stumbled upon the RC522 when looking for one to buy. When comparing between both sensors, we figured that the PN532 module supports SPI, I2C, and UART communication, while the RC522 only supports SPI. In addition to the NFC reading, the SPI supports both RFID read and write. Hence, we moved forward with the former and chose the SPI configuration instead of the I2C.

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/NFC_Module.jpg)

After thoroughly investigating the PN532 datasheet, we discovered that according to which communication the sensor will be used for, we need to set two switches builtin as 1-ch1 & 0-ch2 when using the I2C or 0-ch1 & 1-ch2 for the SPI.

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/NFC_STM32.png)

We also need to mention that in the STM32 CubeMx configuration, the Prescaler in TIM2 was set to 720, and the AutoReloadRegister (ARR) to 1999. The SPI1 was set to Full-Duplex Master Mode, with a data size of 8 Bits instead of the default 4 and follows an LSB First not MSB. The clock parameters of the SPI1 was set for 128 in Prescaler to support 562.5 KBits/s. It is recommended to allow a high level of Prescaler here since when we tried to configure the system when it was low, the sensor did not function properly as it should. To display the uid that is read with the sensor, we enabled USART2 to display what is read in TeraTerm. Nevertheless, in the GPIO settings, we added one GPIO input to the RST state and then we added another that was not used in case we needed to configure a button for a certain task. Also, we included four GPIO outputs for the IRQ, SS, the Green and the Red LEDS, where one indicates success while the other indicates failure. The Clock configuration was set to 72 Mhz instead of the max 80 Mhz to settle the APB to a certain value that is supported by the sensor.

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/NFC_Connection_Updated.jpeg)

The following is an example of the output whenever any reading is taken by the NFC sensor. As shown below, it can either grant access or deny it based on the uid reading while taking in consideration if it is registered or not.

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/NFC%20TeraTerm.png)

## ESP32 and Telegram 
ESP32 module is used in the project to support WIFI connectivity and enable the communication with the admin of the system at all times. The admin of the system will be notified through the ESP32 module of any fraud activity detected at the site being monitored. Fraud activity is indicated by having three unsuccessful fingerprint scans in a row. In this case, the STM32 μC will set a GPIO pin (flag pin) to one once the count of unsuccessful scans reaches 3. This flag pin will be the input of a GPIO pin in the ESP32 module and will be read continuously in the code of the ESP module to push a notification to the admin through Telegram to alert him of what’s happening at the site as shown in the diagram below.

![alt text](https://github.com/andrewkamal/Biometric-Authentication-System/blob/main/Images/esp.png)

The admin also has the ability to send different commands to the system to control a number of physical elements that exist at the facility. For example, the admin can send a command to turn on all the emergency LEDs at the facility to alert the security staff at any point in time. 
The admin can also send a command to declare a normal state at the site. Additionally, he can inquire about the state of the system at any point in time to see whether it’s in an emergency state or a normal state.

Supported commands:

Alert_security_staff

Declare_normal_state

Get_state


## Demos
Milestone 3 (Demos): 
Interfacing the fingerprint sensor with the LCD: [Link](https://drive.google.com/file/d/1GA7guuEns3KkpbxFE4PJ1iBcuDGD7kZa/view?usp=sharing)

Fraud Detection through Telegram: [Link](https://drive.google.com/file/d/1GCFsPM2aAPnpM03C57JtzW5-_HOmkuVY/view?usp=sharing)

NFC Module: [Link](https://drive.google.com/file/d/1vV_LXz9GBeB0cCzsBTUuMgCg0iYOknQe/view?usp=sharing)

## Final Submission:
please refer to the Demo: [Link](https://drive.google.com/drive/my-drive)

## Slides
Milestone 1 (Intial Presentation): [Link](https://docs.google.com/presentation/d/1zvmKQrSaM8J1B9W9fq0iGHN_LQyvAFlY/edit#slide=id.p1)

Milestone 2 (Progress Presentation 1): [Link](https://docs.google.com/presentation/d/18PbApuCLR6rVmSS4hLHxZogK3drjYBY5/edit#slide=id.p1)

Milestone 3 (Progress Presentation 2): [Link](https://docs.google.com/presentation/d/1OdqsuPTCykq9MJpy14oOrDOri2u-x1Vt/edit#slide=id.p1)

Milestone 4 (Final Presentation): [Link](https://docs.google.com/presentation/d/1NLhvfGiLwZ7oxhbd5B8Zf40z2_y3UmaMpPtBPBASaK4/edit#slide=id.g105a6a69d26_1_49)

## References
[1] JM-101 optical fingerprint module user manual - 3V3. (n.d.). Retrieved December 13, 2021, from https://3v3.com.ua/data/files/JM-101-OPTICAL-FINGERPRINT-MODULE-USER-MANUAL-V1.8A.PDF. 

[2] Elechouse. (n.d.). Retrieved December 13, 2021, from http://www.elechouse.com/elechouse/images/product/PN532_module_V3/PN532_%20Manual_V3.pdf. 

[3] R307 finger print module user manual - OpenHacks. (n.d.). Retrieved December 13, 2021, from https://www.openhacks.com/uploadsproductos/r307_fingerprint_module_user_manual.pdf. 

[4] 1602 a instructions - funduino. de. (n.d.). Retrieved December 13, 2021, from https://funduino.de/DL/1602LCD.pdf. 


