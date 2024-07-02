#include <heltec.h> // Include the Heltec library

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // Wait for the serial port to connect (for native USB devices)
  }

  pinMode(25, OUTPUT);
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
  // Try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // Received a packet
    Serial.print("Received packet: '");

    String receivedText = "";
    while (LoRa.available()) {
      char receivedChar = (char)LoRa.read();
      Serial.print(receivedChar);
      receivedText += receivedChar;
    }
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    if(receivedText.endsWith("ON"))
    {
      digitalWrite(25, HIGH);
    }
    else
    {
      digitalWrite(25, LOW);
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