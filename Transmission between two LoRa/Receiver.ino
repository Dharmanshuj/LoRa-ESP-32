#include <heltec.h> // Include the Heltec library

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the serial port to connect (for native USB devices)
  }

  Serial.println("LoRa Receiver");

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
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Received a packet
    String receivedText = "";
    while (LoRa.available()) {
      receivedText += (char)LoRa.read();
    }
    Serial.print("Received packet: ");
    Serial.println(receivedText);

    if (receivedText.startsWith("hello")) {
      sendReply(receivedText);
    } else if (receivedText.startsWith("Ack:")) {
      Serial.println("Acknowledgment received");
      // Update display with the acknowledgment
      Heltec.display->clear();
      Heltec.display->drawString(0, 0, "Ack received:");
      Heltec.display->drawString(0, 10, receivedText);
      Heltec.display->display();
    }

    // Update display with the received message
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "Received packet:");
    Heltec.display->drawString(0, 10, receivedText);
    Heltec.display->drawString(0, 20, "RSSI: " + String(LoRa.packetRssi()));
    Heltec.display->display();
  }

  delay(10); // Small delay to prevent flooding the loop
}

void sendReply(String receivedText) {
  String replyMessage = "Reply: " + receivedText;
  Serial.print("Sending reply: ");
  Serial.println(replyMessage);

  LoRa.beginPacket();
  LoRa.print(replyMessage);
  LoRa.endPacket();

  // Update display with the reply message
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "Sent reply:");
  Heltec.display->drawString(0, 10, replyMessage);
  Heltec.display->display();
}
