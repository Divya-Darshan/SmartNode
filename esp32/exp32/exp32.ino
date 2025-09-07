// Made by Divya Darshan, Harish
// Based on the IEEE paper mentioned above and developed under the course 23CS1512 – Socially Relevant Mini Project at Panimalar Engineering College.

#define BLYNK_TEMPLATE_ID "TMPL3e-Kt1AH1"
#define BLYNK_TEMPLATE_NAME "SmartNode"
#define BLYNK_AUTH_TOKEN "l2OXOjMapLMem-CDA40aBjzBdGpjsi_H"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Relay control
#define relayPin 4   // GPIO4 → connected to relay IN pin

// Soil moisture sensor
int soilPin = 32;      // Analog pin
int soilValue = 0;
int dryValue = 3500;   // sensor value in dry air (calibrate!)
int wetValue = 1200;   // sensor value fully in water (calibrate!)

// Wi-Fi credentials
char ssid[] = "Darshan";
char pass[] = "pravina83";

// Relay control via Blynk app (Switch widget on V0)
BLYNK_WRITE(V0) {
  int value = param.asInt();
  digitalWrite(relayPin, value == 1 ? LOW : HIGH);  // Relay usually active LOW
}

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  // Relay OFF initially

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

    // Read soil sensor
    soilValue = analogRead(soilPin);
    int moisturePercent = map(soilValue, dryValue, wetValue, 0, 100);
    moisturePercent = constrain(moisturePercent, 0, 100);

    // Short status labels
    String state;
    if (moisturePercent < 25) {
      state = "Super Dry";
    } else if (moisturePercent < 40) {
      state = "Dry";
    } else if (moisturePercent <= 70) {
      state = "Moist";
    } else if (moisturePercent <= 85) {
      state = "Wet";
    } else {
      state = "Soaked";
    }

    // Final message → "Moisture: 55% | Moist"
    String statusMsg = String(moisturePercent) + "% | " + state;

    // Print to Serial
    Serial.print("Soil Value: ");
    Serial.print(soilValue);
    Serial.print(" | ");
    Serial.println(statusMsg);

    // Send to Blynk
    Blynk.virtualWrite(V1, moisturePercent);   // V1 = Gauge/Display widget
    Blynk.virtualWrite(V2, statusMsg);         // V2 = Text Label widget

    delay(2000); // Update every 2 seconds
  }
}
