/****************************************************
 * SmartNode – Wokwi Virtual Smart Home Version (Full)
 * Made by Divya Darshan & Harish
 ****************************************************/

#define BLYNK_TEMPLATE_ID "TMPL3e-Kt1AH1"
#define BLYNK_TEMPLATE_NAME "SmartNode"
#define BLYNK_AUTH_TOKEN "l2OXOjMapLMem-CDA40aBjzBdGpjsi_H"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// ---------- Pin Configuration ----------
#define RELAY_LIGHT 4     // Main Hall Light
#define RELAY_FAN 5       // Ceiling Fan
#define RELAY_TV 18       // Television
#define RELAY_LAMP 19     // Table Lamp
#define RELAY_PUMP 27     // Water Pump
#define SOIL_PIN 32       // Soil sensor (Potentiometer in Wokwi)

// ---------- WiFi Credentials ----------
char ssid[] = "Wokwi-GUEST";
char pass[] = "";

// ---------- Soil Variables ----------
int soilValue = 0;
float displayedMoisture = 0; // For smoothing
int moisturePercent = 0;

// ---------- BLYNK INPUTS ----------

// Light Control (Button on V0)
BLYNK_WRITE(V0) {
  int value = param.asInt();
  digitalWrite(RELAY_LIGHT, value);
  Serial.println(value ? "💡 Main Hall Light ON" : "💡 Main Hall Light OFF");
}

// Fan Control (Button on V4)
BLYNK_WRITE(V4) {
  int value = param.asInt();
  digitalWrite(RELAY_FAN, value);
  Serial.println(value ? "🌀 Fan ON" : "🌀 Fan OFF");
}

// TV Control (Button on V5)
BLYNK_WRITE(V5) {
  int value = param.asInt();
  digitalWrite(RELAY_TV, value);
  Serial.println(value ? "📺 TV ON" : "📺 TV OFF");
}

// Lamp Control (Button on V6)
BLYNK_WRITE(V6) {
  int value = param.asInt();
  digitalWrite(RELAY_LAMP, value);
  Serial.println(value ? "💡 Lamp ON" : "💡 Lamp OFF");
}

// Pump Control (Button on V3)
BLYNK_WRITE(V3) {
  int value = param.asInt();
  digitalWrite(RELAY_PUMP, value);
  Serial.println(value ? "🚿 Pump ON (Manual)" : "🚿 Pump OFF (Manual)");
}

// ---------- SETUP ----------
void setup() {
  Serial.begin(115200);

  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_TV, OUTPUT);
  pinMode(RELAY_LAMP, OUTPUT);
  pinMode(RELAY_PUMP, OUTPUT);

  digitalWrite(RELAY_LIGHT, LOW);
  digitalWrite(RELAY_FAN, LOW);
  digitalWrite(RELAY_TV, LOW);
  digitalWrite(RELAY_LAMP, LOW);
  digitalWrite(RELAY_PUMP, LOW);

  Serial.println("🌐 Connecting to Wi-Fi...");
  WiFi.begin(ssid, pass);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("✅ SmartNode Connected to Blynk IoT!");
}

// ---------- LOOP ----------
void loop() {
  Blynk.run();

  // --- Read actual soil sensor ---
  soilValue = analogRead(SOIL_PIN);
  int targetMoisture = map(soilValue, 4095, 0, 0, 100);
  targetMoisture = constrain(targetMoisture, 0, 100);

  // --- Smooth the displayed value ---
  float smoothing = 0.05; // 5% change per loop
  displayedMoisture = displayedMoisture + (targetMoisture - displayedMoisture) * smoothing;

  // --- Update Blynk ---
  Blynk.virtualWrite(V1, displayedMoisture);

  String soilState;
  if (displayedMoisture < 25) soilState = "Super Dry";
  else if (displayedMoisture < 40) soilState = "Dry";
  else if (displayedMoisture <= 70) soilState = "Moist";
  else if (displayedMoisture <= 85) soilState = "Wet";
  else soilState = "Soaked";

  Blynk.virtualWrite(V2, soilState);

  // --- Auto Pump Control ---
  if (displayedMoisture < 30) {
    digitalWrite(RELAY_PUMP, HIGH);
    Blynk.virtualWrite(V3, 1);
    Serial.println("🚿 Auto Pump ON (Soil too dry)");
  } else if (displayedMoisture > 70) {
    digitalWrite(RELAY_PUMP, LOW);
    Blynk.virtualWrite(V3, 0);
    Serial.println("🚿 Auto Pump OFF (Soil moist enough)");
  }

  Serial.print("🌿 Soil Moisture (smoothed): ");
  Serial.print(displayedMoisture);
  Serial.print("% | ");
  Serial.println(soilState);

  delay(200);  // Faster loop for smooth decrease
}
