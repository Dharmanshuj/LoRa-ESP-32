#include <heltec.h> // Include the Heltec library

int counter = 0;
bool awaitingReply = false;
unsigned long lastSendTime = 0;
unsigned long replyTimeout = 5000; // 5 seconds

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the serial port to connect (for native USB devices)
  }

  Serial.println("LoRa Sender");

  // Initialize the Heltec board with Display and LoRa
  Heltec.begin(true /* Display */, true /* LoRa */, true /* Serial */, true /* PABOOST */, 433E6 /* LoRa Band */);

  // Initialize LoRa with frequency and PABOOST
  if (!LoRa.begin(433E6, true)) {
    Serial.println("Starting LoRa failed!");
    while (1) {
      delay(1000); // Add a delay to prevent flooding the serial monitor
    }
  } else {
    Serial.println("LoRa initialization successful.");
  }
}

void loop() {
  if (!awaitingReply && millis() - lastSendTime > 5000) {
    sendMessage();
    lastSendTime = millis();
    awaitingReply = true;
  }

  if (awaitingReply) {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String receivedText = "";
      while (LoRa.available()) {
        receivedText += (char)LoRa.read();
      }
      Serial.print("Received reply: ");
      Serial.println(receivedText);

      if (receivedText.startsWith("Reply:")) {
        sendAcknowledgment();
        awaitingReply = false;
      }
    }

    // Check for timeout
    if (millis() - lastSendTime > replyTimeout) {
      Serial.println("Reply timeout. Resending...");
      awaitingReply = false; // Allow resending
    }
  }
}

void sendMessage() {
  String message = "hello " + String(counter);
  Serial.print("Sending packet: ");
  Serial.println(message);

  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  // Update display with the sent message
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Sent packet:");
  Heltec.display->drawString(0, 10, message);
  Heltec.display->display();

  counter++;
}

void sendAcknowledgment() {
  String ackMessage = "Ack: " + String(counter - 1);
  Serial.print("Sending acknowledgment: ");
  Serial.println(ackMessage);

  LoRa.beginPacket();
  LoRa.print(ackMessage);
  LoRa.endPacket();

  // Update display with the acknowledgment message
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Sent acknowledgment:");
  Heltec.display->drawString(0, 10, ackMessage);
  Heltec.display->display();
}
