#define BLYNK_TEMPLATE_ID "TMPL3e-Kt1AH1"
#define BLYNK_TEMPLATE_NAME "SmartNode"
#define BLYNK_AUTH_TOKEN "l2OXOjMapLMem-CDA40aBjzBdGpjsi_H"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define outputPin D2  // GPIO4 — Can be changed

char ssid[] = "Darshan";          // Your WiFi name
char pass[] = "pravina83";      // Your WiFi password

BLYNK_WRITE(V0) {
  int value = param.asInt();
  digitalWrite(outputPin, value ? HIGH : LOW);
}

void setup() {
  Serial.begin(9600);
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, LOW);  // Start LOW

  WiFi.begin(ssid, pass);
  Serial.print("🔌 Connecting to Wi-Fi");

  int attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 40) {
    delay(500);
    Serial.print(".");
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Wi-Fi connected!");
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  } else {
    Serial.println("\n❌ Wi-Fi connection failed. Check SSID/Password.");
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Blynk.run();
  }
}
