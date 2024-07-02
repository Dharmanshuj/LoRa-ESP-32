# WiFi Scanning

### Setup function

- Initializes the serial port at 115200 baud
- Sets the WiFi mode to station mode (WIFI_STA) and disconnects from any previously connected access point (AP)
- Waits for 100ms to ensure the disconnection is complete
- Prints "Setup done" to the serial monitor

### Loop function

- Prints "scan start" to the serial monitor
- Calls WiFi.scanNetworks() to scan for available WiFi networks
- Prints "scan done" to the serial monitor
- Checks if any networks were found:
  - If no networks were found, prints "no networks found"
  - If networks were found, prints the number of networks found and iterates through each network:
    - Prints the network number, SSID, RSSI, and encryption type (open or secured with *) for each network
    - Waits for 10ms between each network printout
- Waits for 5 seconds before scanning again

# Connecting with WiFi

### Setup function

- Initializes the serial port at 115200 baud
- Waits for 10ms to ensure the serial port is ready
- Prints a blank line to the serial monitor
- Prints the WiFi network SSID to the serial monitor
- Calls WiFi.begin() to start connecting to the WiFi network using the provided SSID and password
- Waits for the WiFi connection to be established, printing a dot (.) every 500ms to indicate progress
- Once connected, prints "WiFi connected" and the local IP address to the serial monitor

### Loop function

- Currently empty, but this is where you would add code to send data via HTTP GET requests to the data.sparkfun.com service

**To complete this sketch, you would need to:**

- Replace "your-ssid" and "your-password" with your actual WiFi network credentials.
- Obtain a streamId and privateKey from data.sparkfun.com and add them to the sketch.
- Implement the logic to send data via HTTP GET requests to the data.sparkfun.com service in the loop() function.
