/***************************************
 *  Line Following + Obstacle Avoidance
 *  Arduino Nano (ATmega328P)
 *  
 *  - 3 × IR sensors (left, center, right)
 *  - L298N motor driver (dual DC motors)
 *  - HC-SR04 ultrasonic sensor for obstacle scanning
 *  - When an obstacle < 15 cm is detected:
 *      • Stop
 *      • Rotate 5° every 100 ms up to ±90°
 *      • Return to original heading
 *      • Resume line following
 ***************************************/

// ----- PIN DEFINITIONS -----
// IR line sensors:
#define IR_LEFT 6    // Left sensor
#define IR_CENTER 7  // Center sensor
#define IR_RIGHT 8   // Right sensor

// Motor driver (L298N):
#define ENA 9  // PWM speed control for left motor
#define IN1 2  // Direction control for left motor
#define IN2 3
#define ENB 10  // PWM speed control for right motor
#define IN3 4   // Direction control for right motor
#define IN4 5

// Ultrasonic sensor (HC-SR04):
#define TRIG_PIN 11  // Trigger pulse output
#define ECHO_PIN 12  // Echo pulse input

// ----- CONSTANTS -----
// Speed settings:
const int LINE_SPEED = 150;  // Base forward speed (0–255)
const int TURN_SPEED = 90;  // Speed when turning

// Obstacle threshold (in cm):
const int OBST_THRESH = 15;  // Distance at which to start scanning

// ===== SETUP =====
void setup() {
  // Configure IR sensor pins as inputs:
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_CENTER, INPUT);
  pinMode(IR_RIGHT, INPUT);

  // Configure motor driver pins as outputs:
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Configure ultrasonic sensor pins:
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Ensure motors are stopped before starting:
  stopMotors();
}

// ===== MAIN LOOP =====
void loop() {
  // 1. Read distance to obstacle (in cm)
  //int distance = readUltrasonic();

  /* // 2. If obstacle detected within threshold, run scan routine
  if (distance > 0 && distance < OBST_THRESH) {
    obstacleAvoidance();
  }
  // 3. Otherwise, keep following the line
  else {
    lineFollowing();
  }*/
  lineFollowing();
  // moveForward(LINE_SPEED);
}

// ===== SENSOR FUNCTIONS =====

// Read HC-SR04 and convert echo time to centimeters:
int readUltrasonic() {
  // Send a 10 µs HIGH pulse on TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Measure the pulse width on ECHO_PIN
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Speed of sound: 343 m/s → 29.1 µs per cm (round-trip)
  return (int)((duration / 2) / 29.1);
}

// ===== MOTION PRIMITIVES =====

void moveForward(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

// Move straight backward:
void moveBackward(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
 
}

// Rotate robot clockwise (right turn in place):
void turnClockwise(int speed) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

// Rotate robot counter-clockwise (left turn in place):
void turnCounterClockwise(int speed) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
}

// Stop both motors immediately:
void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// ===== LINE FOLLOWING =====

// Uses 3 IR sensors to keep robot on a black line:
void lineFollowing() {
  // Read each sensor (LOW = over black)
  bool left = (digitalRead(IR_LEFT) == LOW);
  bool center = (digitalRead(IR_CENTER) == LOW);
  bool right = (digitalRead(IR_RIGHT) == LOW);

  // If centered on line → go straight
  if (digitalRead(IR_CENTER) == HIGH && digitalRead(IR_RIGHT) == LOW && digitalRead(IR_LEFT) == LOW) {
    moveForward(LINE_SPEED);
  }
  //If veering left → turn left
  else if (digitalRead(IR_LEFT) == HIGH) {
    turnCounterClockwise(TURN_SPEED);
  }
  // If veering right → turn right
  else if (digitalRead(IR_RIGHT) == HIGH) {
    turnClockwise(TURN_SPEED);
  }
  // If completely off line → stop
  else {
    // stopMotors();
    turnClockwise(TURN_SPEED);
  }
}

// ===== OBSTACLE AVOIDANCE =====

// Scan ±90° in 5° increments, pausing 100 ms per step:
/*void obstacleAvoidance() {
  stopMotors();                 // Halt before scanning
  const int steps = 90 / 5;     // Number of 5° increments

  // ---- Clockwise scan ----
  for (int i = 0; i < steps; i++) {
    turnClockwise(TURN_SPEED);
    delay(100); stopMotors();   // Step + brief pause
    delay(50);
  }

  // ---- Return to original heading ----
  for (int i = 0; i < steps; i++) {
    turnCounterClockwise(TURN_SPEED);
    delay(100); stopMotors();
    delay(50);
  }

  // ---- Counter-clockwise scan ----
  for (int i = 0; i < steps; i++) {
    turnCounterClockwise(TURN_SPEED);
    delay(100); stopMotors();
    delay(50);
  }

  // ---- Return to original heading ----
  for (int i = 0; i < steps; i++) {
    turnClockwise(TURN_SPEED);
    delay(100); stopMotors();
    delay(50);
  }
}

void loop() {
  // Test Motor A forward
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  analogWrite(ENA, 200);
  delay(2000);
  // Test Motor A reverse
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  delay(2000);
  // Test Motor B forward
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(ENB, 200);
  delay(2000);
  // Test Motor B reverse
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
  delay(2000);
}

*/
