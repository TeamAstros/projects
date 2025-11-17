#include <SoftwareSerial.h>

// Define Bluetooth RX and TX Pins
SoftwareSerial BT(2, 3);  // Use available digital pins instead of 0 and 1.

const int light_relay = 5 ;
const int fan_relay = 6 ;
const int smallLight_relay = 7 ;


void setup() {
  // Initialize Bluetooth Module
  pinMode(light_relay , OUTPUT);
  pinMode(fan_relay , OUTPUT);
  pinMode(smallLight_relay , OUTPUT);
  Serial.begin(9600);
  BT.begin(9600);

}

void loop() {
  if (BT.available()) {
    char Command = BT.read();
    Serial.println(Command);


    if(Command == 'a')
    digitalWrite(light_relay , 1);

    else if(Command == 'b') 
    digitalWrite(light_relay , 0);

    else if(Command == 'c')
    digitalWrite(fan_relay , 1);

    else if(Command == 'd')
    digitalWrite(fan_relay , 0);

    else if(Command == 'e')
    digitalWrite( smallLight_relay , 1);

    else if(Command == 'f')
    digitalWrite( smallLight_relay , 0);

 
  }
}

