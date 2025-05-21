#include <Servo.h>

#define PIR_PIN 4
#define redled_pin 8
#define greenled_pin 9
#define whiteled_pin 10
#define trigPin 7
#define echoPin 6
#define lightSensorPin A0
#define servoPin 5
#define buzzerpin 3

Servo myServo;
bool isVaultOpen = false;

void setup() {
    Serial.begin(9600);
    pinMode(redled_pin, OUTPUT);
    pinMode(greenled_pin, OUTPUT);
    pinMode(whiteled_pin, OUTPUT);
    pinMode(buzzerpin, OUTPUT);
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(PIR_PIN, INPUT);
    pinMode(lightSensorPin, INPUT);
    myServo.attach(servoPin);
}

int measureDistance() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    int duration = pulseIn(echoPin, HIGH);
    int distance = (duration / 2) * 0.0348;
    Serial.print("Ultrasonic:");
    Serial.println(distance);
    return distance;
}

int readPIR() {
    int pirValue = digitalRead(PIR_PIN);
    Serial.print("PIR:");
    Serial.println(pirValue);
    return pirValue;
}

int readLightIntensity() {
    int lightIntensity = analogRead(lightSensorPin);
    Serial.print("Light Intensity:");
    Serial.println(lightIntensity);
    return lightIntensity;
}

String readCommand() {
    String command;
    if (Serial.available() > 0) {
        command = Serial.readStringUntil('\n');
        command.trim(); // Remove any trailing whitespace
        Serial.print("Received command: ");
        Serial.println(command); // Debugging output
    }
    return command;
}

int light(int lightIntensity) {
    const int highThreshold = 200;   
    const int midThreshold = 500;
    if (lightIntensity < highThreshold) {
        analogWrite(whiteled_pin, 10); 
    } else if (lightIntensity < midThreshold) {
        analogWrite(whiteled_pin, 100); 
    } else {
        digitalWrite(whiteled_pin, HIGH);
    }
}

void activate(String command) {
    if (command == "OPEN!") {
        int currentPosition = myServo.read(); 
        int targetPosition = 75; 
        int step = 1; 
        int delayTime = 15; 
        while (currentPosition != targetPosition) {
            if (currentPosition < targetPosition) {
                currentPosition += step;
            } else {
                currentPosition -= step;
            }
            myServo.write(currentPosition);
            delay(delayTime);
        }
        int lightIntensity = readLightIntensity(); 
        light(lightIntensity); 
        digitalWrite(redled_pin, LOW);
        digitalWrite(greenled_pin, HIGH);
        isVaultOpen = true;  
    }
    if (command == "DANGER!") {
        myServo.write(0);
        digitalWrite(whiteled_pin, LOW);
        digitalWrite(redled_pin, HIGH);
        digitalWrite(greenled_pin, LOW);
        tone(buzzerpin, 1000);
        delay(1000);
        noTone(buzzerpin);
        delay(1000);
    }
    if (command == "CLOSE!") {
        int currentPosition = myServo.read(); 
        int targetPosition = 75; 
        int step = 1; 
        int delayTime = 15; 
        while (currentPosition != targetPosition) {
            if (currentPosition < targetPosition) {
                currentPosition += step;
            } else {
                currentPosition -= step;
            }
            myServo.write(currentPosition);
            delay(delayTime);
        }
        digitalWrite(whiteled_pin, LOW);
        digitalWrite(redled_pin, LOW);
        digitalWrite(greenled_pin, LOW);
        isVaultOpen = false;  // Reset the flag to indicate the vault is closed
        noTone(buzzerpin);
    }
}

void deactivate() {
    int currentPosition = myServo.read(); 
        int targetPosition = 0; 
        int step = 1; 
        int delayTime = 15; 
        while (currentPosition != targetPosition) {
            if (currentPosition < targetPosition) {
                currentPosition += step;
            } else {
                currentPosition -= step;
            }
            myServo.write(currentPosition);
            delay(delayTime);
        }
    digitalWrite(whiteled_pin, LOW);
    digitalWrite(redled_pin, LOW);
    digitalWrite(greenled_pin, LOW);
    noTone(buzzerpin);
}

void loop() {
    int distance = measureDistance();
    int pirValue = readPIR();
    int lightIntensity = readLightIntensity();
    String command = readCommand();
    
    if (distance <= 20 && pirValue == HIGH && !isVaultOpen) {
        Serial.println("Activate!");
    } else if (!isVaultOpen) {
        deactivate();
        Serial.println("Deactivate!");
    }

    if (command != "") {
        activate(command);
    }

    delay(1000);
}
