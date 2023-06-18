#include <Servo.h>
#include <LiquidCrystal.h>

// Define the pins for the ultrasonic sensors and LCD display
const int ultrasonic1_trigPin = 7;
const int ultrasonic1_echoPin = 8;
const int ultrasonic2_trigPin = 9;
const int ultrasonic2_echoPin = 10;
const int lcd_rs = 12;
const int lcd_en = 11;
const int lcd_d4 = 5;
const int lcd_d5 = 4;
const int lcd_d6 = 3;
const int lcd_d7 = 2;

// Define the variables for the servo motors and LCD display
Servo servo1;
Servo servo2;

int distance1 = 0;
int distance2 = 0;
int buzzerOn = 1;
int foundD1=0;

// Initialize the LCD display
LiquidCrystal lcd(lcd_rs, lcd_en, lcd_d4, lcd_d5, lcd_d6, lcd_d7);

void setup()
{
    // Set up the pins for the ultrasonic sensors
    pinMode(ultrasonic1_trigPin, OUTPUT);
    pinMode(ultrasonic1_echoPin, INPUT);
    pinMode(ultrasonic2_trigPin, OUTPUT);
    pinMode(ultrasonic2_echoPin, INPUT);

    // Set up the servo motors
    servo1.attach(6);
    servo2.attach(13);

    // Set up the LCD display
    lcd.begin(16,2);
    lcd.print("Safe Now");
}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Safe Now");
    foundD1=0;
    // Measure the distance for ultrasonic sensor1
    digitalWrite(ultrasonic1_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(ultrasonic1_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(ultrasonic1_trigPin, LOW);
    distance1 = pulseIn(ultrasonic1_echoPin, HIGH) / 58;

    // If ultrasonic sensor1 detects the train, turn on the alarm and move the servo motors to block the road
    if (distance1 < 10)
    {
        foundD1=1;
    }
    else
    {
      // If there is no train, keep the servo motors in their original position
        servo1.write(0);
        servo2.write(0);
    }
    while(foundD1){
        servo1.write(0);
        servo2.write(0);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Train is coming");
        tone(1,2000,2100);
        delay(10); // mili second
        // Measure the distance for ultrasonic sensor2
        digitalWrite(ultrasonic2_trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(ultrasonic2_trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(ultrasonic2_trigPin, LOW);
        distance2 = pulseIn(ultrasonic2_echoPin, HIGH) / 58;

        // If ultrasonic sensor2 detects the train, keep the servo motors blocking the road
        if(distance2 < 10)
        {
            while(distance2 < 10)
            {
                tone(1,2000,2100);
                delay(100);
                servo1.write(90);
                servo2.write(90);
                // tone(1,2000,2300);
                // delay(100);
              
                digitalWrite(ultrasonic2_trigPin, LOW);
                delayMicroseconds(2);
                digitalWrite(ultrasonic2_trigPin, HIGH);
                delayMicroseconds(10);
                digitalWrite(ultrasonic2_trigPin, LOW);
                distance2 = pulseIn(ultrasonic2_echoPin, HIGH) / 58;
            }


            // Move the servo motors back to their original position when the train has passed
            lcd.clear();
            lcd.setCursor(0, 0);
            
            lcd.print("Train Gone!!");
          	delay(800);
            servo1.write(0);
            servo2.write(0);
            foundD1=0;
            noTone(1);
        }
    }

    // Display the distance for ultrasonic sensor1 on the LCD display
    lcd.setCursor(0, 1);
    lcd.print("Distance1: ");
    lcd.print(distance1);
    lcd.print("cm");
    delay(200);
}
