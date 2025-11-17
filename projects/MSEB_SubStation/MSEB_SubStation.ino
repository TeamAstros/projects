#define BLYNK_TEMPLATE_ID "TMPL3TOxYtkE6"
#define BLYNK_TEMPLATE_NAME "MSCB"
#define BLYNK_AUTH_TOKEN "8EHqo8rp7uUIINxa6CA_fYn8iTClt0KW"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
const char* ssid = "HARSHAD";
const char* password = "12345678"; 
const int cur_sensorIn = 33;  // pin where the OUT pin from sensor is connected on Arduino
int mVperAmp = 185;           // this the 5A version of the ACS712 - use 100 for 20A Module and 66 for 30A Module
int Watt = 0;
double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;
#define ANALOG_IN_PIN 32  // ESP32 pin GPIO36 (ADC0) connected to voltage sensor
#define REF_VOLTAGE 3.3
#define ADC_RESOLUTION 4096.0
#define R1 30000.0  // resistor values in voltage sensor (in ohms)
#define R2 7500.0   // resistor values in voltage sensor (in ohms)
#define relay 4
#define pbutton 21
#define buzz 5 

#define current_value_label V0
#define voltage_value_label V1  
#define switch V2


void setup() {
  Serial.begin(9600);
  Serial.println("ACS712 current sensor");
  analogSetAttenuation(ADC_11db);
  pinMode(relay, OUTPUT);
  pinMode(buzz, OUTPUT);
  pinMode(pbutton, INPUT_PULLUP);


  Blynk.begin(BLYNK_AUTH_TOKEN,ssid,password);
}


void loop(){
 //current code block
   int adc_value = analogRead(ANALOG_IN_PIN);

  // determine voltage at adc input
  float voltage_adc = ((float)adc_value * REF_VOLTAGE) / ADC_RESOLUTION;

  // calculate voltage at the sensor input
  float voltage_in = voltage_adc * (R1 + R2) / R2;

  Serial.print("Measured Voltage = ");
  Serial.println(voltage_in, 2);  // gives voltage value

  Voltage = getVPP();
 VRMS = (Voltage / 2.0) * 0.707;  // root 2 is 0.707
AmpsRMS = (VRMS * 1000) / mVperAmp;
Serial.print("current = "); 
  Serial.print(AmpsRMS);  // gives current value
Serial.println(digitalRead(pbutton));

 Blynk.virtualWrite(current_value_label,1);
 Blynk.virtualWrite(voltage_value_label,voltage_in);


if (voltage_in < 9 && voltage_in > 3.5) {
    digitalWrite(relay, 1);
    Serial.println("on");
    Blynk.virtualWrite(switch,1);
    digitalWrite(buzz , 1);

     while (true);
  }
  else{
    Blynk.virtualWrite(switch,0);
    digitalWrite(relay, 0);
    digitalWrite(buzz , 0);
    }

}
float getVPP() {
  float result;
  int readValue;        // value read from the sensor
  int maxValue = 0;     // store max value here
  int minValue = 1024;  // store min value here

  uint32_t start_time = millis();
  while ((millis() - start_time) < 1000)  // sample for 1 Sec
  {
    readValue = analogRead(cur_sensorIn);
    if (readValue > maxValue) {
      maxValue = readValue;
    }
    if (readValue < minValue) {
      minValue = readValue;
    }
  }

  result = ((maxValue - minValue) * 5.0) / 1024.0;
  return result;
}
