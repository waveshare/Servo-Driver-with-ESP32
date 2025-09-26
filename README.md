# Servo-Driver-with-ESP32

### Description:

Application for the Waveshare Servo Driver with ESP32.

Main Differneces compared to the original version:

- MODE: SF  (Default) Serial forwarding, allows to control the servos via serial commands over the USB-C cable. 
Pressing the "Boot" button changes to the WIFI control mode where it displays the SSID you can connect to and the IP address to control the servos via an web application.

- The firmware is automatically built using github actions and can be flashed directly via the browser without the need to download any tool or the firmware.
- Platform IO support for better and easier building than with Arduino IDE


### Flashing Firmware

You can flash the latest firmware to your ESP32 directly from your web browser using the following link:

[https://fivef.github.io/Servo-Driver-with-ESP32](https://fivef.github.io/Servo-Driver-with-ESP32)

**Instructions:**
1. Open the [Flash Firmware](https://fivef.github.io/Servo-Driver-with-ESP32/) link in a compatible web browser (e.g., Chrome, Edge, Opera).
2. Connect your ESP32 board to your computer via USB.
3. Click the "Connect" button on the web page and select the serial port corresponding to your ESP32. (Just retry if it fails the first time.)
4. Once connected, click the "Install" button to begin flashing the firmware.
5. After the flashing process is complete, you can disconnect your Servo Driver Board.

### Website:

https://www.waveshare.net/shop/Servo-Driver-with-ESP32.htm

### WIKI:

https://www.waveshare.net/wiki/Servo_Driver_with_ESP32


