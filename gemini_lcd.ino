#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>

// WiFi Credentials
const char* ssid = "Hotspot kavalentra";
const char* password = "Botarun@123";

// API Credentials
const char* Gemini_Token = "AIzaSyDP6tiA-3TRm3zYFxy3fe3iZVcT6Mpu638";
const char* Gemini_Max_Tokens = "100";

String res = "";

// LCD Pins
LiquidCrystal lcd(19, 33, 18, 22, 25, 23); // RS, E, D4, D5, D6, D7

// Motor Pins
#define IN1 14  // Motor A IN1
#define IN2 12 // Motor A IN2
#define IN3 13 // Motor B IN3
#define IN4 4 // Motor B IN4
#define ENA 5 // PWM for Motor A
#define ENB 22 // PWM for Motor B

void setup() {
  Serial.begin(115200);
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(2000);

  // Setup Motor Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopMotors();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  lcd.clear();
  lcd.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print(".");
  }
  lcd.clear();
  lcd.print("WiFi Connected!");
  Serial.println("WiFi connected successfully.");
  delay(1000);
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.print("Ask Question: ");
  Serial.println("");
  Serial.println("Ask your Question : ");

  // Get Question from Serial Monitor
  while (!Serial.available());
  while (Serial.available()) {
    char add = Serial.read();
    res += add;
    delay(1);
  }

  res.trim(); // Trim whitespace
  res = "\"" + res + "\""; // Add quotes

  Serial.println("Sending Question: " + res);
  lcd.clear();
  lcd.print("Processing...");

  HTTPClient https;

  if (https.begin("https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=" + (String)Gemini_Token)) {
    https.addHeader("Content-Type", "application/json");
    https.setTimeout(15000); 

    // Prepare Payload
    String payload = String("{\"contents\": [{\"parts\":[{\"text\":" + res + "}]}],\"generationConfig\": {\"maxOutputTokens\": " + (String)Gemini_Max_Tokens + "}}");

    int httpCode = https.POST(payload);
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String response = https.getString();

        DynamicJsonDocument doc(4096); // JSON buffer size
        deserializeJson(doc, response);

        String Answer = doc["candidates"][0]["content"]["parts"][0]["text"];
        Answer.trim();

        Serial.println("Answer: " + Answer);

        // Display Answer on LCD
        lcd.clear();
        lcd.print("Answering...");
        delay(1000);

        // Parse Answer to Control Motors
        if (Answer.indexOf("forward") != -1) {
          moveForward();
        } else if (Answer.indexOf("backward") != -1) {
          moveBackward();
        } else if (Answer.indexOf("left") != -1) {
          turnLeft();
        } else if (Answer.indexOf("right") != -1) {
          turnRight();
        } else if (Answer.indexOf("stop") != -1) {
          stopMotors();
        } else {
          lcd.clear();
          lcd.print("Unknown Command");
        }
        delay(3000);
        stopMotors(); // Stop motors after the command is executed
      } else {
        Serial.printf("[HTTP] Error Code: %d\n", httpCode);
        lcd.clear();
        lcd.print("HTTP Error: ");
        lcd.setCursor(0, 1);
        lcd.print(httpCode);
      }
    } else {
      Serial.printf("[HTTP] Error: %s\n", https.errorToString(httpCode).c_str());
      lcd.clear();
      lcd.print("HTTP Err: -1");
    }
    https.end();
  } else {
    Serial.println("[HTTP] Unable to connect to server");
    lcd.clear();
    lcd.print("Connect Error");
  }

  res = "";
}

// Motor Control Functions
void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255); 
 // analogWrite(ENB, 255);
  lcd.clear();
  lcd.print("Moving Forward");
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
  //analogWrite(ENB, 255);
  lcd.clear();
  lcd.print("Moving Backward");
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 200);
  //analogWrite(ENB, 200);
  lcd.clear();
  lcd.print("Turning Left");
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 200);
  //analogWrite(ENB, 200);
  lcd.clear();
  lcd.print("Turning Right");
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  //analogWrite(ENB, 0);
  lcd.clear();
  lcd.print("Stopped");
}

