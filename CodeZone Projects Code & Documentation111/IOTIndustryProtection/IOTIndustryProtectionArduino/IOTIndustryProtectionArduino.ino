#define motorRelay 7
#define fanRelay 8

#define motor_input A0
#define fan_input A1

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
  Serial.print("motor input = ");
  Serial.println(analogRead(motor_input));
  Serial.print("fan input = ");
  Serial.println(analogRead(fan_input));
  if(analogRead(motor_input ) > 500) {
    digitalWrite(motorRelay, LOW);
    Serial.println("Motor ON");
  } else{
    digitalWrite(motorRelay, HIGH);
    Serial.println("Motor Off");
  }
  if(analogRead(fan_input ) > 500) {
    digitalWrite(fanRelay, LOW);
    Serial.println("Fan ON");
  } else  {
    digitalWrite(fanRelay, HIGH);
    Serial.println("Fan off");
  } 
}
