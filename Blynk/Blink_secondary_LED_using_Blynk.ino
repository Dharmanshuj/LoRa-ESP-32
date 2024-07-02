// Board :- Heltec ESP32 Series Dev-boards version 0.0.9
// Library :- Heltec ESP32 Dev-Boards version 1.1.2
//            Blynk by Volodymyr Shymanskyy version 1.3.2
//            BlynkNcpDriver version 0.6.3
#define BLYNK_TEMPLATE_ID "TMPL3siGPOXMV"
#define BLYNK_TEMPLATE_NAME "LED"
#define BLYNK_AUTH_TOKEN "_Gd04LcGXFH259YKEhl2TArvFozMhbL5" 
#define BLYNK_PRINT Serial

#include<heltec.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
# define LED_PIN 25
// Your WiFi credentials.
// Set password to "" for open networks.

//#define BLYNK_AUTH_TOKEN "-iHs4asjc7AuAvzxZSm9nYos8wz9_tAz"
char ssid[] = "your-ssid";
char pass[] = "your-password";

// Your Blynk auth token.

void setup()
{
  // Debug console
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("LoRa Sender");

  // Initialize the Heltec board with Display and LoRa
  Heltec.begin(true /* Display */, true /* LoRa */, true /* Serial */, true /* PABOOST */, 433E6 /* LoRa Band */);

  // Connect to Wi-Fi
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  if (!LoRa.begin(433E6, true)) {
    Serial.println("Starting LoRa failed!");
    while (1) {
      delay(1000); // Add a delay to prevent flooding the serial monitor
    }
  } else {
    Serial.println("LoRa initialization successful.");
  }

  // Connect to Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Synchronize the virtual pin state with the Blynk server
  // Blynk.syncVirtual(V1);
}

BLYNK_WRITE(V1) {
  int pinValue = param.asInt(); // Get value as integer
  Serial.print("V1 value: ");
  Serial.println(pinValue);

  if (pinValue) {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED turned ON");
    Blynk.logEvent("led_status", "LED turned ON");

    LoRa.beginPacket();
    LoRa.print("LED turned ON");
    LoRa.endPacket();

    // Update display with the sent message
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "LED turned ON");
    Heltec.display->display();

  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED turned OFF");
    Blynk.logEvent("led_status", "LED turned OFF");

    LoRa.beginPacket();
    LoRa.print("LED turned OFF");
    LoRa.endPacket();

    // Update display with the sent message
    Heltec.display->clear();
    Heltec.display->drawString(0, 0, "LED turned OFF");
    Heltec.display->display();
  }
}

// This function is called periodically by Blynk
// It updates the value of a virtual pin
// BLYNK_READ(V2) {
//   int sensorValue = analogRead(34); // Read sensor value from pin 34
//   Blynk.virtualWrite(V2, sensorValue);
// }

void loop()
{
  Blynk.run();
}
