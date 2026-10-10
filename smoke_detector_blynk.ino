#define BLYNK_TEMPLATE_ID "TMPL6RytCzDXI"
#define BLYNK_TEMPLATE_NAME "Fire and Smoke detecting System"
#define BLYNK_AUTH_TOKEN "BLYNK_AUTH_TOKEN"

#include <ESP32Servo.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>


char ssid[] = "Pixel 7 pro";
char pass[] = "11111111";


const int mq2Pin = 35; 
const int flameSensorPin = 13; 
const int buzzerPin = 12; 
const int ledPin = 2; 
const int servoPin = 4; 
const int relayPin = 23; 


const int mq2Threshold = 1000; 
const int servoOpenAngle = 0; 
const int servoClosedAngle = 60; 

Servo myservo; 

void setup() {
    Serial.begin(115200);


    pinMode(buzzerPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
    pinMode(relayPin, OUTPUT); 
    pinMode(flameSensorPin, INPUT);


    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    digitalWrite(relayPin, LOW);

    myservo.attach(servoPin);
    myservo.write(servoClosedAngle);
    Serial.println("System Initialized. Awaiting alarm.");

    Serial.print("Connecting to Wi-Fi...");
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWi-Fi connected!");

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
    Serial.println("Blynk connected!");
}

void loop() {

    Blynk.run();

    int mq2Value = analogRead(mq2Pin);
    int flameValue = digitalRead(flameSensorPin);

    Serial.print("MQ2 Value: ");
    Serial.print(mq2Value);
    Serial.print(" | Flame Sensor: ");
    Serial.println(flameValue);

    if (mq2Value > mq2Threshold || flameValue == HIGH) {
    
        Serial.println(">>> ALARM! Smoke or fire detected. Opening lock...");

        digitalWrite(buzzerPin, HIGH);
        digitalWrite(ledPin, HIGH);
        myservo.write(servoOpenAngle);
        digitalWrite(relayPin, HIGH);

        Blynk.logEvent("fire_alarm", "🚨 Smoke or Fire detected! Check immediately!");


        Blynk.virtualWrite(V0, "ALARM!");
        Blynk.virtualWrite(V1, mq2Value);
        Blynk.virtualWrite(V2, "Flame Detected");
        Blynk.virtualWrite(V3, "ON");
    } else {
        // Safe state

        // Deactivate Alarms
        digitalWrite(buzzerPin, LOW);
        digitalWrite(ledPin, LOW);
        myservo.write(servoClosedAngle);
        digitalWrite(relayPin, LOW);


        Blynk.virtualWrite(V0, "SAFE");
        Blynk.virtualWrite(V1, mq2Value);
        Blynk.virtualWrite(V2, "No Flame");
        Blynk.virtualWrite(V3, "OFF");
    }

    delay(500); 
}

