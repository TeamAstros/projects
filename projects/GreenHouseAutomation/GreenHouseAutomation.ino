#define BLYNK_TEMPLATE_ID "TMPL3JEuka6Oc"
#define BLYNK_TEMPLATE_NAME "Green House Automation"
#define BLYNK_TEMPLATE_AUTH "2xH4CgG6pBn3nBXJtoeBGGYUUXIX_wX2"

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Pin Definitions
#define DHT11_PIN 4
#define AOUT_PIN A0

#define fanv1 12
#define fanv2 14
#define motor1 15
#define motor2 13

// DHT Sensor Initialization
DHT dht11(DHT11_PIN, DHT11);

// Blynk Credentials
char auth[] = BLYNK_TEMPLATE_AUTH;
char ssid[] = "vivo Y22";
char pass[] = "abcxyz123";

// Blynk Virtual Pins
#define V1_TEMP V1
#define V2_HUMI V2
#define V3_PUMP V3
#define V4_PUMP_STATE V4
#define V5_FAN_STATE V5

// Global Variables
bool pumpManualControl = false;

void setup() {
  Serial.begin(9600);

  // Initialize DHT Sensor
  dht11.begin();

  // Initialize Pins
  pinMode(AOUT_PIN, INPUT);
  pinMode(fanv1, OUTPUT);
  pinMode(fanv2, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);

  digitalWrite(fanv1, LOW);
  digitalWrite(fanv2, LOW);
  digitalWrite(motor1, LOW);
  digitalWrite(motor2, LOW);

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);

  Serial.println("System Initialized!");
}

// Blynk Button for Pump Control
BLYNK_WRITE(V3_PUMP) {
  int pinValue = param.asInt();
  pumpManualControl = pinValue;
}

void controlPump(bool pumpOn) {
  if (pumpOn) {
    Serial.println("Pump: ON");
    digitalWrite(motor1, HIGH);
    digitalWrite(motor2, LOW);
    Blynk.virtualWrite(V4_PUMP_STATE, 255); // Update Blynk
  } else {
    Serial.println("Pump: OFF");
    digitalWrite(motor1, LOW);
    digitalWrite(motor2, LOW);
    Blynk.virtualWrite(V4_PUMP_STATE, 0);   // Update Blynk
  }
}

void controlFan(bool fanOn) {
  if (fanOn) {
    Serial.println("Fan: ON");
    digitalWrite(fanv1, HIGH);
    digitalWrite(fanv2, LOW);
    Blynk.virtualWrite(V5_FAN_STATE, 255); // Update Blynk
  } else {
    Serial.println("Fan: OFF");
    digitalWrite(fanv1, LOW);
    digitalWrite(fanv2, LOW);
    Blynk.virtualWrite(V5_FAN_STATE, 0);   // Update Blynk
  }
}

void loop() {
  Blynk.run();

  // Read Sensor Data
  float humi = dht11.readHumidity();
  float tempC = dht11.readTemperature();

  // Retry sensor reading if invalid
  if (isnan(tempC) || isnan(humi)) {
    Serial.println("Retrying DHT11 sensor reading...");
    humi = dht11.readHumidity();
    tempC = dht11.readTemperature();
  }

  // Default to zero if still invalid
  if (isnan(tempC) || isnan(humi)) {
    Serial.println("Failed to read from DHT11 sensor!");
    humi = 0.0;
    tempC = 0.0;
  } else {
    // Log Data to Serial and Send to Blynk
    Serial.printf("Humidity: %.1f%% | Temperature: %.1f°C\n", humi, tempC);
    Blynk.virtualWrite(V1_TEMP, tempC);
    Blynk.virtualWrite(V2_HUMI, humi);
  }

  // Read Moisture Sensor
  int moisture = analogRead(AOUT_PIN);
  Serial.printf("Moisture Level: %d\n", moisture);

  // Pump Control Logic
  if (pumpManualControl || moisture > 1000) {
    controlPump(true);  // Turn Pump ON
  } else {
    controlPump(false); // Turn Pump OFF
  }

  // Fan Control Logic
  if (tempC > 29.0) {
    controlFan(true);  // Turn Fan ON
  } else {
    controlFan(false); // Turn Fan OFF
  }

  delay(500); // Short delay for stability
}
