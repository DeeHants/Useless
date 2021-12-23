// Actions
const int actionCount = 3;
int actionOffset[actionCount];

// Action data itself
// Each action has multiple steps, with a type, two values, and a delay
// Servo, position (0 - 100), interval (speed), delay
// Buzzer, frequency, interval, delay
// LED, state (HIGH/LOW), unused, delay
// Ensure the counts of steps per action and total actions are accurate else Bad Things Happen(tm)
const int actionData[] = {
  // Up, slowly back, press, home
  6, // steps
  LED_ACTION, HIGH, 0, 0, // LED on
  SERVO_ACTION, 80, 100, 100, // 80% up in 100ms
  SERVO_ACTION, 40, 1000, 1000, // Back to 40% over 1s
  SERVO_ACTION, 100, 100, 100, // Quickly press
  SERVO_ACTION, 0, 0, 0, // Back home
  LED_ACTION, LOW, 0, 0,

  // slowly press, slowly home
  4, // steps
  LED_ACTION, HIGH, 0, 0, // LED on
  SERVO_ACTION, 100, 1000, 1000, // Press over 1s
  SERVO_ACTION, 0, 1000, 1000, // Back home
  LED_ACTION, LOW, 0, 0,

  // Move opposite, then quickly press
  6, // steps
  LED_ACTION, HIGH, 0, 0, // LED on
  SERVOOP_ACTION, 80, 1000, 1000, // Opposite 80% up in 1s
  SERVO_ACTION, 100, 100, 100, // Quickly press
  SERVOOP_ACTION, 0, 0, 0, // Back home
  SERVO_ACTION, 0, 0, 0, // Back home
  LED_ACTION, LOW, 0, 0,
};
