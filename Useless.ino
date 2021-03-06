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

// Set to 1 to enable the serial debug output
#define DEBUG 1

void setup() {
  // Debug output
  #if DEBUG
  Serial.begin(9600);
  #endif

  // Setup the action data and make sure it can be accessed
  setupActionData();

  // Toggles
  setupToggle(0);
  setupToggle(1);

  // Ancillaries
  pinMode(ledPin, OUTPUT);

  // Reset the servos
  servo[0].write(0);
  servo[1].write(0);

  // Ensure we have random numbers
  randomSeed(analogRead(0));

  #if DEBUG
  Serial.println("Initialised");
  #endif
}

// Setup the action data array so wwe can use it efficiently
void setupActionData() {
  int nextOffset = 0;
  for(int index = 0; index < actionCount; index++) {
    // Save the offset for this action
    actionOffset[index] = nextOffset;
    int stepCount = actionData[nextOffset];

    // Calculate the offset for the next action
    nextOffset += 1 + (stepCount * 4);
  }
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
  // Attempt to debounce by waiting 100ms and seeing if it's still pressed.
  if (newState == LOW) {
    delay(100);
    newState = digitalRead(togglePin[toggle]);
  }
  #if DEBUG
  Serial.println("Toggle " + String(toggle) + " state: " + String(newState));
  #endif

  if (newState == LOW && toggleState[toggle] != LOW) { // Has pullup so contact goes low
    int nextAction = random(0, actionCount) + 1;
    toggleAction[toggle] = nextAction;
    toggleStep[toggle] = 1;
    #if DEBUG
    Serial.println("Toggle " + String(toggle) + " pressed, starting action " + String(nextAction));
    #endif
  }
  toggleState[toggle] = newState;
}

void checkToggleAction(int toggle) {
  if (toggleAction[toggle]) {
    #if DEBUG
    Serial.println("Toggle 1 action");
    #endif
    // Calculate the offset into the action data array
    int actionStart = actionOffset[toggleAction[toggle] - 1];
    int actionStepStart = actionStart + 1 + ((toggleStep[toggle] - 1) * 4); // The step count, and 4 for each step

    // Do the action!
    doAction(toggle, actionStepStart);

    // See if we've got to the end
    int stepCount = actionData[actionStart];
    if (toggleStep[toggle] < stepCount) {
      // Not on the last one
      toggleStep[toggle]++;
    } else {
      #if DEBUG
      Serial.println("Toggle 1 finished");
      #endif
      // Reset everything
      toggleAction[toggle] = 0;
      toggleStep[toggle] = 0;

      // Reset the servo
      // TODO Remap servoPosition to correct range
      servo[toggle].write(0);
    }
  }
}

void doAction(int toggle, int actionOffset) {
  int actionType = actionData[actionOffset];
  int param1 = actionData[actionOffset + 1];
  int param2 = actionData[actionOffset + 2];
  int actionSleep = actionData[actionOffset + 3];

  #if DEBUG
  Serial.println("Action type: " + String(actionType) + " at " + String(actionOffset));
  #endif

  int servoPosition;
  int buzzerPitch = 0;
  int buzzerDuration = 0;
  int ledState;
  switch (actionType) {
    case SERVO_ACTION:
    case SERVOOP_ACTION:
      #if DEBUG
      Serial.println("Servo action");
      #endif
      // param1 = position
      // param2 = duration

      // Swap toggle ID
      if (actionType == SERVOOP_ACTION) {
        toggle = (toggle == 1 ? 0 : 1);
      }

      // FIXME ignores speed // param2 // adjust sleep as we delay
      // TODO Remap servoPosition to correct range
      servo[toggle].write(param1);
      break;

    case BUZZER_ACTION:
      #if DEBUG
      Serial.println("Buzzer action");
      #endif
      // param1 = pitch
      // param2 = duration

      tone(buzzerPin, param1, param2);
      break;

    case LED_ACTION:
      #if DEBUG
      Serial.println("LED action");
      #endif
      // param1 = state

      digitalWrite(ledPin, param1);
      break;

    default:
      #if DEBUG
      Serial.println("Unknown action");
      #endif
      // Do Nothing
      break;
  }

  #if DEBUG
  Serial.println("Action sleeping for: " + String(actionSleep));
  #endif
  delay(actionSleep);
}
