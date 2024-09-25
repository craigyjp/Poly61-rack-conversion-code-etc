/*
  ShiftRegister74HC595 - Library for simplified control of 74HC595 shift registers.
  Developed and maintained by Timo Denk and contributers, since Nov 2014.
  Additional information is available at https://timodenk.com/blog/shift-register-arduino-library/
  Released into the public domain.
*/

#include <ShiftRegister74HC595.h>

// create a global shift register object
int numberOfShiftRegisters = 1;  // number of shift registers attached in series
int serialDataPin = 2;           // DS
int clockPin = 3;                // SHCP
int latchPin = 4;                // STCP
ShiftRegister74HC595<9> sr(serialDataPin, clockPin, latchPin);

#define HOLD_RELAY 0
#define RANGE_1_RELAY 1
#define RANGE_2_RELAY 2
#define RANGE_FULL_RELAY 3
#define MODE_DOWN_RELAY 4
#define MODE_UP_DOWN_RELAY 5
#define MODE_UP_RELAY 6
#define HOLD_LED 7

#define RANGE_LED_RED 9
#define RANGE_LED_GREEN 8
#define MODE_LED_RED 11
#define MODE_LED_GREEN 10

#define HOLD_SW 5
#define RANGE_SW 6
#define MODE_SW 7

// Variables to store button states and previous states for debouncing
bool holdState = false;
bool previousHoldState = false;

int rangeState = 0;
bool previousRangeState = false;

int modeState = 0;
bool previousModeState = false;

void setup() {
  // Initialize the button pins
  pinMode(HOLD_SW, INPUT_PULLUP);
  pinMode(RANGE_SW, INPUT_PULLUP);
  pinMode(MODE_SW, INPUT_PULLUP);

  // Initialize the LED pins
  pinMode(RANGE_LED_RED, OUTPUT);
  pinMode(RANGE_LED_GREEN, OUTPUT);
  pinMode(MODE_LED_RED, OUTPUT);
  pinMode(MODE_LED_GREEN, OUTPUT);

  // Turn off LEDs initially
  digitalWrite(RANGE_LED_RED, LOW);
  digitalWrite(RANGE_LED_GREEN, LOW);
  digitalWrite(MODE_LED_RED, LOW);
  digitalWrite(MODE_LED_GREEN, LOW);

  digitalWrite(RANGE_LED_RED, HIGH);
  digitalWrite(RANGE_LED_GREEN, LOW);
  sr.set(RANGE_2_RELAY, LOW);
  sr.set(RANGE_FULL_RELAY, LOW);
  sr.set(RANGE_1_RELAY, HIGH);

  digitalWrite(MODE_LED_RED, HIGH);
  digitalWrite(MODE_LED_GREEN, LOW);
  sr.set(MODE_UP_RELAY, LOW);
  sr.set(MODE_UP_DOWN_RELAY, LOW);
  sr.set(MODE_DOWN_RELAY, HIGH);
}

void loop() {
  // Latch behavior for HOLD_SW (toggle on/off each press)
  bool holdPressed = digitalRead(HOLD_SW) == LOW;
  if (holdPressed && !previousHoldState) {  // Button is pressed and was not pressed previously
    holdState = !holdState;                 // Toggle state
    if (holdState) {
      // Turn on HOLD LED
      sr.set(HOLD_LED, HIGH);
      sr.set(HOLD_RELAY, HIGH);
    } else {
      // Turn off HOLD LED
      sr.set(HOLD_LED, LOW);
      sr.set(HOLD_RELAY, LOW);
    }
  }
  previousHoldState = holdPressed;  // Update previous state for debounce

  // Latch behavior for RANGE_SW (3 positions)
  bool rangePressed = digitalRead(RANGE_SW) == LOW;
  if (rangePressed && !previousRangeState) {  // Button is pressed and was not pressed previously
    // Change rangeState in a cycle from 0 -> 1 -> 2
    rangeState = (rangeState + 1) % 3;

    if (rangeState == 0) {
      // Position 1: turn on RANGE_LED_RED
      digitalWrite(RANGE_LED_RED, HIGH);
      digitalWrite(RANGE_LED_GREEN, LOW);
      sr.set(RANGE_2_RELAY, LOW);
      sr.set(RANGE_FULL_RELAY, LOW);
      sr.set(RANGE_1_RELAY, HIGH);
    } else if (rangeState == 1) {
      // Position 2: turn on both RANGE LEDs
      digitalWrite(RANGE_LED_RED, LOW);
      digitalWrite(RANGE_LED_GREEN, HIGH);
      sr.set(RANGE_FULL_RELAY, LOW);
      sr.set(RANGE_1_RELAY, LOW);
      sr.set(RANGE_2_RELAY, HIGH);
    } else {
      // Position 3: turn on RANGE_LED_GREEN
      digitalWrite(RANGE_LED_RED, HIGH);
      digitalWrite(RANGE_LED_GREEN, HIGH);
      sr.set(RANGE_1_RELAY, LOW);
      sr.set(RANGE_2_RELAY, LOW);
      sr.set(RANGE_FULL_RELAY, HIGH);
    }
  }
  previousRangeState = rangePressed;  // Update previous state for debounce

  // Latch behavior for MODE_SW (3 positions)
  bool modePressed = digitalRead(MODE_SW) == LOW;
  if (modePressed && !previousModeState) {  // Button is pressed and was not pressed previously
    // Change modeState in a cycle from 0 -> 1 -> 2
    modeState = (modeState + 1) % 3;

    if (modeState == 0) {
      // Position 1: turn on MODE_LED_RED
      digitalWrite(MODE_LED_RED, HIGH);
      digitalWrite(MODE_LED_GREEN, LOW);
      sr.set(MODE_UP_RELAY, LOW);
      sr.set(MODE_UP_DOWN_RELAY, LOW);
      sr.set(MODE_DOWN_RELAY, HIGH);
    } else if (modeState == 1) {
      // Position 2: turn on both MODE LEDs
      digitalWrite(MODE_LED_RED, HIGH);
      digitalWrite(MODE_LED_GREEN, HIGH);
      sr.set(MODE_DOWN_RELAY, LOW);
      sr.set(MODE_UP_RELAY, LOW);
      sr.set(MODE_UP_DOWN_RELAY, HIGH);
    } else {
      // Position 3: turn on MODE_LED_GREEN
      digitalWrite(MODE_LED_RED, LOW);
      digitalWrite(MODE_LED_GREEN, HIGH);
      sr.set(MODE_DOWN_RELAY, LOW);
      sr.set(MODE_UP_DOWN_RELAY, LOW);
      sr.set(MODE_UP_RELAY, HIGH);
    }
  }
  previousModeState = modePressed;  // Update previous state for debounce

  delay(50);  // Small delay to debounce button presses
}
