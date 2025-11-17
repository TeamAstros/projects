#define motorRelay 7
#define fanRelay 8

#define motor_input 2
#define fan_input 3

void setup() {
  Serial.begin(9600); 
  pinMode(motorRelay, OUTPUT);
  pinMode(fanRelay, OUTPUT);

  pinMode(motor_input, INPUT);
  pinMode(fan_input, INPUT);

  digitalWrite(motorRelay, HIGH);
  digitalWrite(fanRelay, HIGH);
}

void loop() {
  if(digitalRead(motor_input )== 1) {
    digitalWrite(motorRelay, LOW);
    Serial.println("Motor ON");
  } else{
    digitalWrite(motorRelay, HIGH);
    Serial.println("Motor Off");
  }
  if(digitalRead(fan_input )== 1) {
    digitalWrite(fanRelay, LOW);
    Serial.println("Fan ON");
  } else  {
    digitalWrite(fanRelay, HIGH);
    Serial.println("Fan off");
  } 
}
