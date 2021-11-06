// Actions
const int actionCount = 2;
int actionOffset[actionCount];

// Action data itself
const int actionData[] = {
  // Up, slowly back, press, home
  6, // 4 steps
  LED_ACTION, HIGH, 0, 0, // LED on
  SERVO_ACTION, 80, 100, 100, // 80% up in 100ms
  SERVO_ACTION, 40, 1000, 1000, // Back to 40% over 1s
  SERVO_ACTION, 100, 100, 100, // Quickly press
  SERVO_ACTION, 0, 0, 0, // Back home
  LED_ACTION, LOW, 0, 0,

  // slowly press, slowly home
  4, // 2 steps
  LED_ACTION, HIGH, 0, 0, // LED on
  SERVO_ACTION, 100, 1000, 1000, // Press over 1s
  SERVO_ACTION, 0, 1000, 1000, // Back home
  LED_ACTION, LOW, 0, 0,
};
