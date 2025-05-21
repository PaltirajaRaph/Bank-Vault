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

    // Turn on LEDs
    digitalWrite(whiteled_pin, 255);
    digitalWrite(redled_pin, HIGH);
    digitalWrite(greenled_pin, HIGH);
    myServo.write(75);
    // Play a tone on the buzzer
    tone(buzzerpin, 1000);
    delay(1000);
    noTone(buzzerpin);
}

void loop() {
    // Print PIR sensor reading
    int pirValue = digitalRead(PIR_PIN);
    Serial.print("PIR: ");
    Serial.println(pirValue);

    // Print light sensor reading
    int lightIntensity = analogRead(lightSensorPin);
    Serial.print("Light: ");
    Serial.println(lightIntensity);

    // Print ultrasonic sensor reading
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;
    Serial.print("Distance: ");
    Serial.println(distance);

}
    // // Move servo to 90 degrees
    // myServo.write(75);
    // delay(1000); // Wait for 1 second

    // // Move servo back to 0 degrees
    // myServo.write(0);
    // delay(1000); // Wait for 1 second

    // delay(100); // Additional delay to avoid excessive serial output

