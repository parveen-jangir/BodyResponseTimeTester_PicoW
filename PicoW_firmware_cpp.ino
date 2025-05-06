#include <Arduino.h>

#define noOfLED 10
#define recoveryTime 500

uint8_t ledPin[10] = {20, 21, 2, 3, 4, 5, 6, 7, 8, 9};
uint8_t butPin[10] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

int getRandomNumber() {
  randomSeed(analogRead(A0)); // Seed the random generator using noise from an analog pin
  Serial.print("Random no. ");
  Serial.println(random(0, noOfLED + 1));
  return random(0, noOfLED); // Generates a number between 0 and n (inclusive)
}

void initialTest() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPin[i], HIGH);
    delay(500);
    // while (!digitalRead(butPin[i]));
    // delay(1000); // Wait for 1 second to observe the LED
    digitalWrite(ledPin[i], LOW);
  }
}

void test(unsigned int time) {
  unsigned long testEndTime = millis() + time; // Record the start time
  Serial.println(testEndTime);
  int achived = 0; // Variable to track the number of LEDs turned off
  Serial.println("Test started. Press the corresponding button to turn off the LED.");

  while (millis() < testEndTime) { // Loop until the time is up
    int ledIndex = getRandomNumber(); // Get a random LED index
    unsigned long ledEndTime = millis() + recoveryTime; // Record the time when the LED should turn off
    Serial.print(ledEndTime);
    Serial.print("     ");
    Serial.println(millis());
    digitalWrite(ledPin[ledIndex], HIGH);
    while (millis() < ledEndTime) {
      if (digitalRead(butPin[ledIndex])) { // Check if the corresponding button is pressed
        digitalWrite(ledPin[ledIndex], LOW); // Turn off the LED
        achived++; // Increment the count of LEDs turned off
        Serial.print("Score: ");
        Serial.println(achived);
        break; // Exit the loop if the button is pressed
        delay(5);
      }
    // Serial.println(millis());
    }
    digitalWrite(ledPin[ledIndex], LOW); // Wait for the corresponding button to be pressed
    delay(3000);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Hello, Raspberry Pi Pico!");

  for (int i = 0; i < 10; i++) {
    pinMode(butPin[i], INPUT);
    pinMode(ledPin[i], OUTPUT);
  }

  initialTest();
}

void loop() {

  Serial.println("Enter time in seconds: ");
  while (!Serial.available()); // Wait for user input
  unsigned int time = Serial.parseInt(); // Read the time from the Serial input
  Serial.println(time * 1000);

  test(time * 1000); // Call the test function with the time parameter
  Serial.println("Test completed. Press any button to continue.");
}