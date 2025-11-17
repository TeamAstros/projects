#include <SPI.h>
#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>


#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>

// fire base libarary


//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "saurabh"
#define WIFI_PASSWORD "12345687"

// Insert Firebase project API Key
#define API_KEY "AIzaSyCcxY7Ju6SE8pwzehalawpJCCnwZ4S3ToQ"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://medical-f4bff-default-rtdb.firebaseio.com/" 



#define SS_PIN 4    // SDA pin
#define RST_PIN 15   // RST pin
#define BTN_PIN 5    // Button pin
#define LED_PIN 27    // LED pin (for visual feedback)
#define Button 2

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Create LCD instance (change the address if needed)


//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
String data;
int age;
bool signupOK = false;



void setup() {
  lcd.init();                 // initialize the lcd 
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);
  // pinMode(LED_PIN, OUTPUT);
  // pinMode(BUZZER_PIN, OUTPUT);
  // pinMode(Button, INPUT_PULLUP);


  lcd.backlight(); // Turn on the backlight

  SPI.begin();
  mfrc522.PCD_Init();

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO RFID");
  lcd.setCursor(0, 1);
  lcd.print("SCANNING PROJECT");
  delay(2000);
  lcd.clear();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}





void loop() {
  lcd.setCursor(0, 0);
  lcd.print(" PLACE THE RFID ");
  lcd.setCursor(0, 1);
  lcd.print("  CARD TO SCAN  ");
  delay(1000);

  // Check if a new card is present
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.print("Card UID: ");
    printCardUID(); // Print the UID to the Serial Monitor
    
    // Display the UID on the LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CARD UID:");
    lcd.setCursor(0, 1);
    String uniqueid = getCardUID();
    Serial.println(uniqueid);
    lcd.print(uniqueid);
    while(digitalRead(BTN_PIN)==1);
    delay(1000);
    get_data(uniqueid);
    
  delay(1000);
    
    delay(3000); // Pause for 3 seconds to display the UID
    lcd.clear();
  }
}

void printCardUID() {
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
}

String getCardUID() {
  String cardUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    cardUID += String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    cardUID += String(mfrc522.uid.uidByte[i], HEX);
  }
  return cardUID;
}

void get_data(String a) {
  // Construct the base path
  String basePath = "/test/" + a;

  // Check if the path exists
  if (!Firebase.RTDB.pathExisted(&fbdo, basePath)) {
    Serial.println("Path does not exist: " + basePath);
    return; // Exit the function
  }

  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Fetching the string data from Firebase
    if (Firebase.RTDB.getString(&fbdo, basePath + "/id")) {
      if (fbdo.dataType() == "string") {
        data = fbdo.stringData();
        Serial.println("Received id: " + data);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.println("UID:-");
        lcd.setCursor(0, 1);
        lcd.println(data);
        delay(1000);
      } else {
        Serial.println("Error: Data type is not a string, it is " + fbdo.dataType());
      }
    } else {
      Serial.println("Error fetching string data: " + fbdo.errorReason());
      return; // Exit the function if there's an error
    }

    while (digitalRead(BTN_PIN) == 1);
    delay(1000);

    if (Firebase.RTDB.getString(&fbdo, basePath + "/name")) {
      if (fbdo.dataType() == "string") {
        data = fbdo.stringData();
        Serial.println("Received name: " + data);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.println("Name:-");
        lcd.setCursor(0, 1);
        lcd.println(data);
        delay(1000);
      } else {
        Serial.println("Error: Data type is not a string, it is " + fbdo.dataType());
      }
    } else {
      Serial.println("Error fetching string data: " + fbdo.errorReason());
      return; // Exit the function if there's an error
    }

    while (digitalRead(BTN_PIN) == 1);
    delay(1000);

    if (Firebase.RTDB.getInt(&fbdo, basePath + "/Age")) {
      if (fbdo.dataType() == "int") {
        age = fbdo.intData();
        Serial.println("Received age : " + String(age));
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.println("Age");
        lcd.setCursor(0, 1);
        lcd.println(String(age));
        delay(1000);
      }
    } else {
      Serial.println(fbdo.errorReason());
      return; // Exit the function if there's an error
    }

    while (digitalRead(BTN_PIN) == 1);
    delay(1000);

    if (Firebase.RTDB.getString(&fbdo, basePath + "/Sex")) {
      if (fbdo.dataType() == "string") {
        data = fbdo.stringData();
        Serial.println("Sex " + data);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.println("Sex");
        lcd.setCursor(0, 1);
        lcd.println(data);
        delay(1000);
      } else {
        Serial.println("Error: Data type is not a string, it is " + fbdo.dataType());
      }
    } else {
      Serial.println("Error fetching string data: " + fbdo.errorReason());
      return; // Exit the function if there's an error
    }

    while (digitalRead(BTN_PIN) == 1);
    delay(1000);

    if (Firebase.RTDB.getString(&fbdo, basePath + "/description")) {
      if (fbdo.dataType() == "string") {
        data = fbdo.stringData();
        data.replace("\n", "");
        int dataLength = data.length();

        Serial.println("Description: " + data);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.println("Description");
        scrollText(1, data, 350, 16);
        delay(1000);
      } else {
        Serial.println("Error: Data type is not a string, it is " + fbdo.dataType());
      }
    } else {
      Serial.println("Error fetching string data: " + fbdo.errorReason());
      return; // Exit the function if there's an error
    }
  } else {
    Serial.println("Not connecting");
  }
}


void scrollText(int row, String message, int delayTime, int lcdColumns) {
  if (message.length() <= lcdColumns) {
    // If the message fits within the screen, just display it
    lcd.setCursor(0, row);
    lcd.print(message);
    return;
  }

  // Add spaces to the message for smooth scrolling
  message = message + " "; // Add a space at the end for a smooth wrap-around

  // Scroll the text
  for (int pos = 0; pos <= message.length() - lcdColumns; pos++) {
    lcd.setCursor(0, row); // Set cursor to the start of the row
    lcd.print(message.substring(pos, pos + lcdColumns)); // Print the visible portion
    delay(delayTime); // Wait before scrolling
  }
  lcd.clear();
  return;
}