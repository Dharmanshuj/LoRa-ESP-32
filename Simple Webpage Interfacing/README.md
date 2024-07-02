## Includes and definitions

- Includes the necessary libraries: WiFi, ESPmDNS, and ESPAsyncWebServer. Go to (**Sketch -> Include Library -> Manage Libraries**)
- Defines the LED pins: LED1 on pin 15 and LED2 on pin 12
- Defines a web page as a string in PROGMEM, which includes HTML, CSS, and JavaScript code for the web interface

## Setup function

- Initializes the serial port at 115200 baud
- Sets the LED pins as outputs
- Creates a soft AP (access point) with the name "DJ" and no password
- Starts the MDNS responder with the name "ESP" (accessible at esp.local/)
- Configures the web server:
  - Serves the web page at the root URL ("/")
  - Handles GET requests for LED control:
    - /led1/on and /led1/off toggle LED1
    - /led2/on and /led2/off toggle LED2
- Sets up a 404 error handler for unknown URLs
- Starts the web server

## Loop function

- Currently empty, as the web server is handled by the ESPAsyncWebServer library

## Web page

- The web page is a Bootstrap-based interface with two sliding switches for controlling LED1 and LED2
- Each switch has an onclick event that sends a GET request to the web server to toggle the corresponding LED
- The web page uses JavaScript and XMLHttpRequest to send the GET requests

## Video

https://github.com/Dharmanshuj/LoRa-ESP-32/assets/124305189/a4848b6c-538d-4073-b56a-dd5ecacadc41

## Thank You!!
