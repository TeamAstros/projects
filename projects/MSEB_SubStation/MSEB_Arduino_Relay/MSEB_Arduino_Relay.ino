#define relay_in A0
#define relay_out 7
#define buzzer 2


void setup() {
  // put your setup code here, to run once:
pinMode(relay_in,INPUT);
pinMode(relay_out,OUTPUT);
pinMode(buzzer,OUTPUT);
Serial.begin(9600);
}

void loop() {
  delay(1000);
  // put your main code here, to run repeatedly:
  int a = analogRead(relay_in );
  Serial.println(a);
if(a>500){
  digitalWrite(relay_out,0);
  Serial.println("On");
    tone(buzzer, 1000);
      Serial.println(buzzer);
}else{
  digitalWrite(relay_out,1);
  Serial.println("Of");
   noTone(buzzer);
    Serial.println(buzzer);
}
}
