// #define IN1 5 //D1
// #define IN2 4 //D2
// #define button2 14 //D5
// #define button1 12 //D6

#define IN1 18      
#define IN2 19      
#define button1 22  
#define button2 27  
#define button3 26  
#define hallsensorpin 25  


void setup() {
  // put your setup code here, to run once:
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(button1, INPUT_PULLDOWN);
  pinMode(button2, INPUT_PULLDOWN);
  pinMode(button3, INPUT_PULLDOWN);
  pinMode(hallsensorpin, INPUT);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int button1value = digitalRead(button1);
  Serial.print("button1value -- ");
  Serial.println(button1value);
  int button2value = digitalRead(button2);
  Serial.print("button2value -- ");
  Serial.println(button2value);
  int button3value = digitalRead(button3);
  Serial.print("button3value -- ");
  Serial.println(button3value);
  int hallsensorpinvalue = digitalRead(hallsensorpin);
  Serial.print("Hall sensor value -- ");
  Serial.println(hallsensorpinvalue);



  if (button1value == 1 && button3value == 0 && (button2value == 1 || button2value ==0)) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  } else if (button2value == 1 && button1value == 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  } else if(hallsensorpinvalue == 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
  }

  

  // if(temp == LOW) {
  //   digitalWrite(IN1, HIGH);
  //   digitalWrite(IN2, LOW);
  // }
  // else if(temp == HIGH) {
  //   digitalWrite(IN1, LOW);
  //   digitalWrite(IN2, LOW);
  //   delay(3000);
  //   digitalWrite(IN1, HIGH);
  //   digitalWrite(IN2, LOW);
  // }
  // else {
  //   digitalWrite(IN1, LOW);
  //   digitalWrite(IN2, HIGH);
  // }
}
