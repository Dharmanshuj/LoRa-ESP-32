# Sender Code

### Global variables

- counter: an integer that increments each time a message is sent
- awaitingReply: a boolean that indicates whether the device is waiting for a reply
- lastSendTime: an unsigned long that stores the timestamp of the last sent message
- replyTimeout: an unsigned long that sets the timeout for waiting for a reply (5 seconds)

### Setup function

- Initializes the serial port at 115200 baud
- Initializes the Heltec board with display, LoRa, serial, and PABOOST enabled
- Sets the LoRa frequency to 433E6 (433 MHz)
- Checks if LoRa initialization was successful, and if not, enters an infinite loop with a          - delay to prevent flooding the serial monitor

### Loop function

- Checks if it's time to send a new message (every 5 seconds) and sends one if necessary
- If awaiting a reply, checks for incoming packets and processes them
- If a packet is received, checks if it starts with "Reply:" and sends an acknowledgment if so
- If no reply is received within the timeout period, resends the original message

### sendMessage function

- Creates a message with the current counter value
- Sends the message over LoRa
- Updates the display with the sent message

### sendAcknowledgment function

- Creates an acknowledgment message with the previous counter value
- Sends the acknowledgment over LoRa
- Updates the display with the acknowledgment message

# Reveiver Code

### Setup function

- Initializes the serial port at 115200 baud
- Initializes the Heltec board with display, LoRa, serial, and PABOOST enabled
- Sets the LoRa frequency to 433E6 (433 MHz)
- Checks if LoRa initialization was successful, and if not, enters an infinite loop with a delay to 
prevent flooding the serial monitor

### Loop function

- Checks for incoming LoRa packets using LoRa.parsePacket()
- If a packet is received, processes it:
- Extracts the received text from the packet
- Checks if the received text starts with "hello" and sends a reply if so
- Checks if the received text starts with "Ack:" and updates the display with the acknowledgment if so
- Updates the display with the received message, including the RSSI (Received Signal Strength Indication) value

### sendReply function

- Creates a reply message by prepending "Reply: " to the received text
- Sends the reply message over LoRa
- Updates the display with the reply message
