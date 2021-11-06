// Useless machine
// Designed for 1 or 2 toggles, with a servo each

// Pin data
const int toggle1Pin = 2;
const int servo1Pin = 3;
const int toggle2Pin = 4;
const int servo2Pin = 5;
const int ledPin = LED_BUILTIN;
const int buzzerPin = 8;

#include <Servo.h>

// Toggle status, array to store both sets
const int togglePin[2] = {toggle1Pin, toggle2Pin};
const int servoPin[2] = {servo1Pin, servo2Pin};
Servo servo[2];
int toggleState[2];
int toggleAction[2];
int toggleStep[2];

// Actions
const int NULL_ACTION = 0; // No action
const int SERVO_ACTION = 1; // Controls the servo for this toggle, position, speed, sleep
const int SERVOOP_ACTION = 2; // Controls the servo for the other toggle, position, speed, sleep
const int BUZZER_ACTION = 3; // Sounds the buzzer, frequency, interval, sleep
const int LED_ACTION = 4; // Sets the LED state, state, null, sleep

#include "ActionData.h"

void setup() {
  // Debug output
  Serial.begin(9600);

  // Toggles
  setupToggle(0);
  setupToggle(1);

  // Ancillaries
  pinMode(ledPin, OUTPUT);

  // Reset the servos
  servo[0].write(0);
  servo[1].write(0);

  Serial.print("Initialised");
}

// Setup the pins and state for a single toggle
void setupToggle(int toggle) {
  pinMode(togglePin[toggle], INPUT_PULLUP); // We're switching to ground so use a pullup
  servo[toggle].attach(servoPin[toggle]); // And the important bit, the servo!

  // Set the inital states
  toggleState[toggle] = HIGH; // Has pullup so is high by default
  toggleAction[toggle] = 0; // Sentinel value for no action/step
  toggleStep[toggle] = 0;
}

void loop() {
  // See if the toggles have changed and trigger the actions
  checkToggleState(0);
  checkToggleState(1);

  // Check what we need to do
  checkToggleAction(0);
  checkToggleAction(1);
}

void checkToggleState(int toggle) {
  // See if the toggles have changed and trigger the actions
  int newState = digitalRead(togglePin[toggle]);
  Serial.println("Toggle " + String(toggle) + " state: " + String(newState));

  if (newState == LOW && toggleState[toggle] != LOW) { // Has pullup so contact goes low
    toggleAction[toggle] = 1; // TODO randomise
    toggleStep[toggle] = 1;
    Serial.println("Toggle " + String(toggle) + " pressed");
  }
  toggleState[toggle] = newState;
}

void checkToggleAction(int toggle) {
  if (toggleAction[toggle]) {
    Serial.println("Toggle 1 action");
    int actionStart = getActionOffset(toggleAction[toggle] - 1);
    int actionStepStart = getActionStepOffset(toggleAction[toggle] - 1, toggleStep[toggle] - 1);
    doAction(toggle, actionStepStart);

    // See if we've got to the end
    int stepCount = actionData[actionStart];
    if (toggleStep[toggle] < stepCount) {
      // Not on the last one
      toggleStep[toggle]++;
    } else {
      Serial.println("Toggle 1 finished");
      // Reset everything
      toggleAction[toggle] = 0;
      toggleStep[toggle] = 0;

      // Reset the servo
      // TODO Remap servoPosition to correct range
      servo[toggle].write(0);
    }
  }
}

int getActionOffset(int actionIndex) {
  return actionOffset[actionIndex];
}

int getActionStepOffset(int actionIndex, int stepIndex) {
  return actionOffset[actionIndex] + 1 + (stepIndex * 4);
}

void doAction(int toggle, int actionOffset) {
  int actionType = actionData[actionOffset];
  int param1 = actionData[actionOffset + 1];
  int param2 = actionData[actionOffset + 2];
  int actionSleep = actionData[actionOffset + 3];

  Serial.println("Action type: " + String(actionType) + " at " + String(actionOffset));

  int servoPosition;
  int buzzerPitch = 0;
  int buzzerDuration = 0;
  int ledState;
  switch (actionType) {
    case SERVO_ACTION: // position, duration
    case SERVOOP_ACTION: // FIXME swap servo
      Serial.println("Servo action");
      // FIXME ignores speed // param2 // adjust sleep as we delay
      // TODO Remap servoPosition to correct range
      servo[toggle].write(param1);
      break;

    case BUZZER_ACTION: // pitch, duration
      Serial.println("Buzzer action");
      tone(buzzerPin, param1, param2);
      break;

    case LED_ACTION: // state, null
      Serial.println("LED action");
      digitalWrite(ledPin, param1);
      break;

    default:
      Serial.println("Unknown action");
      // Do Nothing
      break;
  }

  Serial.println("Action sleeping for: " + String(actionSleep));
  delay(actionSleep);
}
