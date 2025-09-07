// Made by Divya Darshan, Harish
// Based on the IEEE paper mentioned above and developed under the course 23CS1512 – Socially Relevant Mini Project at Panimalar Engineering College.

#define BLYNK_TEMPLATE_ID "TMPL3e-Kt1AH1"
#define BLYNK_TEMPLATE_NAME "SmartNode"
#define BLYNK_AUTH_TOKEN "l2OXOjMapLMem-CDA40aBjzBdGpjsi_H"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// On ESP32, use actual GPIO numbers
#define relayPin 4   // GPIO4 → connected to relay IN pin

char ssid[] = "Darshan";      // Your Wi-Fi SSID
char pass[] = "pravina83";      // Your Wi-Fi Password

BLYNK_WRITE(V0) {
  int value = param.asInt();
  
  // Relay is usually active LOW
  digitalWrite(relayPin, value == 1 ? LOW : HIGH);
}

void setup() {
  Serial.begin(9600);
  pinMode(relayPin, OUTPUT);

  // Make sure relay is OFF initially
  digitalWrite(relayPin, HIGH);

  Serial.println("🔌 Connecting to Wi-Fi...");

  WiFi.begin(ssid, pass);
  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 40) {
    delay(500);
    Serial.print(".");
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Wi-Fi connected!");
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  } else {
    Serial.println("\n❌ Wi-Fi connection failed. Check SSID/Password.");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
  }
}
