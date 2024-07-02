# Explaination
- The chipId variable is initialized to 0. This will store the alternate chip ID.

- In the setup() function, the serial communication is initialized at a baud rate of 115200.

- In the loop() function:
    - A for loop iterates 17 times, incrementing by 8 each time. This is used to extract the last 3 bytes of the MAC address.
    - The ESP.getEfuseMac() function returns the MAC address of the ESP32 chip. The >> operator is used to shift the bits, and the & 0xff mask is used to extract the last byte of the MAC address. The << operator is used to shift the extracted byte to its correct position in the chipId variable.
    - The Serial.printf() function is used to print the chip model, revision, number of cores, and the alternate chip ID to the serial console.
    - The delay(3000) function is used to pause the execution for 3 seconds before the next iteration.
