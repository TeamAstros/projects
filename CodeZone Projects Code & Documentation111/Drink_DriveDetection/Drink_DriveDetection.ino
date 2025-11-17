#define BLYNK_TEMPLATE_NAME "Drink"
#define BLYNK_TEMPLATE_ID "TMPL3fHuUZAbq"
#define BLYNK_AUTH_TOKEN "FX4BbDNa1F8QFiN5vh_g3Ae0JiHOWbX9"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


#define MQ A0 
#define Buzzer 4 //D2

#define motor_a 12 //D6 
#define motor_b 14 //D5 

#define digitalPin 13 //D7



int sensor_values = 0 ;



// Blynk Auth Token
char auth[] = BLYNK_AUTH_TOKEN;
// Wi-Fi Credentials
char ssid[] = "vivo Y22";
char pass[] = "abcxyz123";





// Virtual Pins for Blynk LED Widgets
#define virtual_led V0

void setup() {
  // put your setup code here, to run once:
  pinMode(MQ , INPUT);
  pinMode(Buzzer , OUTPUT);
  pinMode(motor_a , OUTPUT);
  pinMode(motor_b , OUTPUT);
  pinMode(digitalPin , INPUT);

  Serial.begin(9600);
// Blynk Initialization
  Blynk.begin(auth, ssid, pass);
      digitalWrite(motor_a , 1);
    digitalWrite(motor_b , 0) ;
}

void loop() {
  Blynk.run();
  // put your main code here, to run repeatedly:
  sensor_values=analogRead(MQ);
  // Serial.println(sensor_values);
  int a = digitalRead(digitalPin);
  Serial.println(a);

  if(a==0)
  { 
   // Serial.println("sensor_value"+sensor_values);
    digitalWrite(Buzzer , 1);
    Blynk.virtualWrite(virtual_led, 1); // Turn ON LED for Slot 1
    digitalWrite(motor_a , 0);
    digitalWrite(motor_b , 0);
    // Blynk.notify("Drink & drive warning!")
    Blynk.logEvent("drunk_driver");

  }
  else {
     
    digitalWrite(Buzzer , 0);
   
    Blynk.virtualWrite(virtual_led, 0);

    digitalWrite(motor_a , 1);
    digitalWrite(motor_b , 0);

  }
  Blynk.virtualWrite(V1, sensor_values);


}
