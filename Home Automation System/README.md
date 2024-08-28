# Project Overview

This IoT project involves creating a smart home automation system and a pet/family member tracking system using a combination of sensors and a responsive website. The main features include:

**1. Controlling Lights:** Remotely turn lights on or off via a website.

**2. Automatic Light Control:** Use an IR sensor to detect motion and automatically turn on lights when someone enters a room.

**3. Environmental Monitoring:** Use a BME688 sensor to read temperature, pressure, and humidity.

**4. GPS Tracking:** Track the location of a pet or family member using a GPS sensor.

## Software Requirement

### Arduino Version 2.3.2

### Board : Heltec ESP32 Series Dev-boards version 0.0.9

**For Proper Installation Guide** [**Click Here**](https://github.com/Dharmanshuj/LoRa-ESP-32/tree/main/Installation)
### Libraries to be Installed : 
1. Adafruit BME680 Library by Adafruit version 2.0.4
2. Adafruit Unified Sensor by Adafruit version 1.1.14
3. ArduinoJson by Benoit Blanchon version 6.18.5
4. AsyncTCP by dvarrel version 1.1.4
5. DHT sensor library by adafruit version 1.4.6
6. ESPAsyncTCP by dvarrel version 1.2.4
7. ESPAsyncWebServer by lacamera 3.1.0
8. EspSoftwareSerial by Dirk Kaar, Peter Lerup version 8.1.0
9. Heltec ESP32 Dev-Boards by Heltec version 1.1.2
10. TinyGPSPlus by Mikal Hart version 1.0.3
11. WebSockets by Markus Sattler version 2.4.1

## Material Required

**1. Wifi LoRa 32 board**

![heltec_wifi_lora_32_v2](https://github.com/Dharmanshuj/LoRa-ESP-32/assets/124305189/07e2d8ad-cacd-4426-a329-d08a657b295a) 

**2. IR Sensor** 
 ![IR Sensor](https://github.com/user-attachments/assets/6f11dd53-097a-49f3-acff-22581eccb3f6)

**3. NEO 6MV2 GPS Sensor**
 
 ![NEO-6MV2-GPS-Module](https://github.com/user-attachments/assets/7b088c90-36b1-4d38-a145-f785a56906a4)

**4. EVE BME688 Sensor**

 ![BME sensor](https://github.com/user-attachments/assets/37c2702e-4997-4a4f-922d-aa510e6c52e5)

**5. LEDs**

![WhatsApp_Image_2024-07-01_at_16 31 09_b4c19aab-removebg-preview](https://github.com/user-attachments/assets/3fb34714-8ec0-43dd-9956-f631cdbe97b2)

**6. Connecting Wires**
 
 ![image](https://github.com/user-attachments/assets/6b786446-477d-4703-89eb-eb2433116d0b)

**7. Bread Board**

![400_point_breadboard__pololu_351__00](https://github.com/user-attachments/assets/97a09944-9785-4b75-87d9-7d26b4efb727)

## Demonstration

![WhatsApp_Image_2024-07-12_at_09 28 55_bb47f453-removebg-preview (1)](https://github.com/user-attachments/assets/1afba4f6-1b17-4809-b06e-99441d2ff32f)

![WhatsApp Image 2024-08-29 at 00 39 57_46ce4482](https://github.com/user-attachments/assets/83cc56d8-ac9d-4adb-8b50-725ef3ebfab5)

![WhatsApp Image 2024-08-29 at 00 39 58_1b99bebd](https://github.com/user-attachments/assets/5da5d479-9c5d-4719-8aee-d9e0c0f16a5d)

## Video Demonstration

https://github.com/user-attachments/assets/e6ac23a8-30b5-454b-a113-0fe4235e3cd3

## How It Works

### User Interface (UI) and Control:

The responsive website serves as the user interface for interacting with the home automation system.
Users can log in to the website and view the current status of lights (on/off) and environmental data. They can manually control the lights using on-screen buttons.

### Automatic Light Control:

The IR sensor continuously monitors the room for any movement. When motion is detected (e.g., someone enters the room), the sensor sends a signal to the microcontroller.
The microcontroller processes this input and triggers the lights to turn on automatically. After a certain period of inactivity, the lights can be programmed to turn off automatically.

### Environmental Monitoring:

The BME688 sensor continuously measures temperature, pressure, and humidity data.
This data is periodically sent to the microcontroller, which then transmits it to the website for real-time display.

### GPS Tracking:

The GPS sensor is attached to a pet's collar or a family member's wearable device.
The sensor constantly sends location data (latitude and longitude) to the microcontroller.
The microcontroller forwards this data to the website, where it is plotted on a map. This feature allows users to track the real-time location of their pet or family member.

## Technologies Involved

**HTML/CSS/JavaScript:** For building the responsive website and user interface.

**Backend Development (Node.js, Python, etc.):** To handle communication between the website and the microcontroller.

**Microcontroller Programming (Arduino, C/C++, Python):** To interface with sensors and actuators, process data, and communicate with the server.

**IoT Communication Protocols (HTTP, MQTT, WebSockets, etc.):** To facilitate communication between the microcontroller and the website.

## Potential Enhancements

**Voice Control:** Integrate voice control using platforms like Google Assistant or Alexa to control lights and other devices.

**Machine Learning:** Use machine learning to predict user behavior and adjust settings automatically (e.g., turning on lights based on usual schedules).

**Alerts and Notifications:** Set up notifications to alert the user if the temperature exceeds a certain threshold or if someone enters the house unexpectedly.

## Conclusion

This IoT project provides a comprehensive solution for smart home automation and pet/family member tracking. By integrating multiple sensors and a responsive web interface, it offers convenience, security, and real-time monitoring, enhancing the overall smart home experience.

## Thank You !!
