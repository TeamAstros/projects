#define BLYNK_TEMPLATE_NAME "IOT Industry Protection"
#define BLYNK_AUTH_TOKEN "cjqo_H-8rsPIhhq7uGPc7c4VCTryeeSR"
#define BLYNK_TEMPLATE_ID "TMPL3qgnOgDIU"

#include <DHT.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#define DHT11_PIN 21
#define MQ3_PIN 32
#define FlameSensor_PIN 18
#define PIRSensor_PIN 5
#define BUZZER_PIN 27
#define motor_control_pin 2
#define fan_control_pin 4

// Initialize DHT11 Sensor
DHT dht11(DHT11_PIN, DHT11);

char auth[] = "cjqo_H-8rsPIhhq7uGPc7c4VCTryeeSR";
char ssid[] = "vivo Y22";
char pass[] = "abcxyz123";

// Blynk Virtual Pins
#define V1_TEMP_LABEL V1
#define V2_HUMI_LABEL V2
#define V3_TEMP_LED V3
#define V4_FLAME_LED V4
#define V5_GAS_LED V5
#define V6_PIR_LED V6
#define V7_FAN_BUTTON V7
#define V8_MOTOR_BUTTON V8

void setup() {
  Serial.begin(9600); 
  dht11.begin();
  pinMode(MQ3_PIN, INPUT);
  pinMode(FlameSensor_PIN, INPUT);
  pinMode(PIRSensor_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(motor_control_pin, OUTPUT);
  pinMode(fan_control_pin, OUTPUT);

  digitalWrite(motor_control_pin, 0);
  digitalWrite(fan_control_pin, 0);

  Blynk.begin(auth, ssid, pass);

  Serial.println("Setup Complete. Starting Loop...");
}

void loop() {
  Blynk.run();

  // Read Data from DHT11
  float humi = dht11.readHumidity();
  float tempC = dht11.readTemperature();

  if (!isnan(tempC) && !isnan(humi)) {
    Serial.printf("DHT11 - Temperature: %.1f°C, Humidity: %.1f%%\n", tempC, humi);
    Blynk.virtualWrite(V1_TEMP_LABEL, tempC);
    Blynk.virtualWrite(V2_HUMI_LABEL, humi);

    if (tempC > 30.0) {
      Serial.println("High Temperature Detected. Turning ON Fan.");
      Blynk.virtualWrite(V3_TEMP_LED, 1); // Turn ON temperature LED
      digitalWrite(fan_control_pin, 1);
    } else {
      Serial.println("Temperature Normal. Turning OFF Fan.");
      Blynk.virtualWrite(V3_TEMP_LED, 0); // Turn OFF temperature LED
      digitalWrite(fan_control_pin, 0);
    }
  } else {
    Serial.println("Failed to read from DHT11 sensor!");
  }

  // Read MQ3 Sensor Value
  int MQ3_value = analogRead(MQ3_PIN);
  Serial.printf("MQ3 Sensor Value: %d\n", MQ3_value);
  Blynk.virtualWrite(V5_GAS_LED, MQ3_value > 400 ? 1 : 0);

  // Read Flame Sensor Value
  int FlameSensor_value = digitalRead(FlameSensor_PIN);
  Serial.printf("Flame Sensor Value: %d\n", FlameSensor_value);
  Blynk.virtualWrite(V4_FLAME_LED, FlameSensor_value == LOW ?1 : 0);

  if (FlameSensor_value == LOW) {
    Serial.println("Flame Detected! Turning ON Motor.");
    digitalWrite(motor_control_pin, 1);
  } else {
    Serial.println("No Flame Detected. Turning OFF Motor.");
    digitalWrite(motor_control_pin, 0);
  }

  // Read PIR Sensor Value
  int PIRSensor_value = digitalRead(PIRSensor_PIN);
  Serial.printf("PIR Sensor Value: %d\n", PIRSensor_value);
  Blynk.virtualWrite(V6_PIR_LED, PIRSensor_value == HIGH ? 1 : 0);

  // Buzzer Control
  if (MQ3_value > 700 || PIRSensor_value == HIGH) {
    Serial.println("Buzzer Activated! Gas Detected or Motion Detected.");
    digitalWrite(BUZZER_PIN, HIGH); // Turn buzzer ON
  } else {
    Serial.println("Buzzer Deactivated.");
    digitalWrite(BUZZER_PIN, LOW); // Turn buzzer OFF
  }

  delay(2000);
}

// Blynk Button Control for Fan
// BLYNK_WRITE(V7_FAN_BUTTON) {
//   int fan_state = param.asInt(); // Get button value (0 or 1)
//   Serial.printf("Fan Button Pressed: %d\n", fan_state);
//   digitalWrite(fan_control_pin, fan_state);
// }

// // Blynk Button Control for Motor
// BLYNK_WRITE(V8_MOTOR_BUTTON) {
//   int motor_state = param.asInt(); // Get button value (0 or 1)
//   Serial.printf("Motor Button Pressed: %d\n", motor_state);
//   digitalWrite(motor_control_pin, motor_state);
// }
