/*
 * Arduino IoT Workshop - Phase 5: Stepper Motor Automation
 * 
 * Description: Complete system with stepper motor control, joystick input, and all previous features
 * Hardware: Arduino Uno R3, LCD1602 (16x2) with I2C backpack, Green LED (Pin 5), Red LED (Pin 6),
 *           8-pin 4x4 membrane keypad, HC-SR04 ultrasonic sensor, DHT11 temp/humidity sensor, 
 *           analog water sensor, 28BYJ-48 stepper motor with ULN2003 driver, analog joystick
 * 
 * Pin Assignments:
 * Pin 2:  Ultrasonic Trigger
 * Pin 3:  Ultrasonic Echo  
 * Pin 4:  DHT11 Data
 * Pin 5:  Green LED (success/positive feedback)
 * Pin 6:  Red LED (error/negative feedback)
 * Pin 7:  Stepper Motor IN1
 * Pin 8:  Stepper Motor IN2
 * Pin 9:  Stepper Motor IN3
 * Pin 10: Stepper Motor IN4
 * Pin 14: Keypad Row 1
 * Pin 15: Keypad Row 2
 * Pin 16: Keypad Row 3
 * Pin 17: Keypad Row 4
 * Pin 18: Keypad Col 1
 * Pin 19: Keypad Col 2
 * Pin 20: Keypad Col 3
 * Pin 21: Keypad Col 4
 * A0: Water Sensor
 * A1: Joystick X-axis
 * A2: Joystick Y-axis
 * A3: Joystick Button
 * A4: SDA (I2C data line)
 * A5: SCL (I2C clock line)
 * 
 * Wiring Diagram:
 * 
 * Arduino Uno R3
 * ┌─────────────────┐
 * │                 │
 * │  Pin 2 ─────────┼── Ultrasonic Trigger
 * │  Pin 3 ─────────┼── Ultrasonic Echo
 * │  Pin 4 ─────────┼── DHT11 Data
 * │  Pin 5 ────[R]──┼── Green LED ──── GND
 * │  Pin 6 ────[R]──┼── Red LED ────── GND
 * │                 │     (220Ω)
 * │  Pin 7 ─────────┼── Stepper IN1
 * │  Pin 8 ─────────┼── Stepper IN2
 * │  Pin 9 ─────────┼── Stepper IN3
 * │  Pin 10 ────────┼── Stepper IN4
 * │  Pin 14 ────────┼── Keypad Row 1
 * │  Pin 15 ────────┼── Keypad Row 2
 * │  Pin 16 ────────┼── Keypad Row 3
 * │  Pin 17 ────────┼── Keypad Row 4
 * │  Pin 18 ────────┼── Keypad Col 1
 * │  Pin 19 ────────┼── Keypad Col 2
 * │  Pin 20 ────────┼── Keypad Col 3
 * │  Pin 21 ────────┼── Keypad Col 4
 * │  A0 ────────────┼── Water Sensor
 * │  A1 ────────────┼── Joystick X-axis
 * │  A2 ────────────┼── Joystick Y-axis
 * │  A3 ────────────┼── Joystick Button
 * │  A4 (SDA) ──────┼── LCD I2C SDA
 * │  A5 (SCL) ──────┼── LCD I2C SCL
 * │  VCC ───────────┼── LCD I2C VCC (5V)
 * │  GND ───────────┼── LCD I2C GND
 * │                 │
 * └─────────────────┘
 * 
 * 28BYJ-48 Stepper Motor with ULN2003 Driver:
 * ┌─────────────────┐
 * │ VCC  GND  IN1  IN2  IN3  IN4 │
 * │  │    │    │    │    │    │   │
 * │  │    │    │    │    │    │   │
 * │  │    │    │    │    │    │   │
 * │  │    │    │    │    │    │   │
 * └─────────────────┘
 * 
 * Analog Joystick Module:
 * ┌─────────────────┐
 * │ VCC  GND  VRX  VRY  SW  │
 * │  │    │    │    │    │   │
 * │  │    │    │    │    │   │
 * │  │    │    │    │    │   │
 * │  │    │    │    │    │   │
 * └─────────────────┘
 * 
 * Expected Behavior:
 * 1. All Phase 4 security features maintained
 * 2. After successful passcode entry, stepper motor activates
 * 3. Auto mode: Motor rotates 90° clockwise every 2 seconds
 * 4. Manual mode: Joystick controls motor rotation
 * 5. Joystick button toggles between auto/manual modes
 * 6. Motor status displayed on LCD
 * 7. Smooth stepping motion with acceleration/deceleration
 * 
 * Motor Control:
 * - 4096 steps per full revolution (90° = 1024 steps)
 * - Speed: ~500 steps per second for smooth operation
 * - Auto mode: 2-second intervals between 90° rotations
 * - Manual mode: Real-time joystick control
 * 
 * Troubleshooting:
 * - If motor doesn't move: Check ULN2003 driver connections and power
 * - If motor is jerky: Check step timing and acceleration settings
 * - If joystick doesn't respond: Check analog connections and calibration
 * - If motor status not shown: Verify LCD display and motor state variables
 * 
 * Known Limitations:
 * - No 7-segment display yet
 * - No EEPROM data logging yet
 * - Basic motor control only
 * 
 * Upgrade Path to Phase 6:
 * - Add 7-segment display for uptime
 * - Implement EEPROM data logging
 * - Add configuration menu system
 * 
 * Library Requirements:
 * - LiquidCrystal_I2C (latest compatible version)
 * - DHT (latest compatible version)
 * - Keypad (latest compatible version)
 * - Stepper (latest compatible version)
 * - Wire (built-in I2C library)
 * 
 * Version: 1.0
 * Created: 2025
 * Compatible: Arduino IDE 1.8.x / 2.x
 */

// Include required libraries
#include <LiquidCrystal_I2C.h>  // LCD display library (latest compatible version)
#include <DHT.h>                // Temperature/humidity sensor (latest compatible version)
#include <Keypad.h>             // 8-pin 4x4 membrane keypad (latest compatible version)
#include <Stepper.h>            // Stepper motor control (latest compatible version)
#include <Wire.h>               // I2C communication (built-in)

// Pin Definitions
#define GREEN_LED_PIN 5
#define RED_LED_PIN 6
#define ULTRASONIC_TRIGGER_PIN 2
#define ULTRASONIC_ECHO_PIN 3
#define DHT_PIN 4
#define WATER_SENSOR_PIN A0
#define JOYSTICK_X_PIN A1
#define JOYSTICK_Y_PIN A2
#define JOYSTICK_BUTTON_PIN A3

// Stepper Motor Pins
#define STEPPER_IN1_PIN 7
#define STEPPER_IN2_PIN 8
#define STEPPER_IN3_PIN 9
#define STEPPER_IN4_PIN 10

// LCD Configuration
#define LCD_ADDRESS 0x27        // I2C address for LCD (try 0x3F if not working)
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// DHT Configuration
#define DHT_TYPE DHT11

// Keypad Configuration (8-pin 4x4 membrane)
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {14, 15, 16, 17}; // Connect to the row pinouts
byte colPins[COLS] = {18, 19, 20, 21}; // Connect to the column pinouts

// Stepper Motor Configuration
#define STEPS_PER_REVOLUTION 4096  // 28BYJ-48 stepper motor
#define STEPS_PER_90_DEGREES 1024  // 90 degrees = 1024 steps
#define MOTOR_SPEED 500            // Steps per second for smooth operation

// Timing Constants
#define STARTUP_BLINK_COUNT 3
#define STARTUP_BLINK_DURATION 200
#define ALTERNATING_DURATION 500
#define LCD_INIT_DELAY 3000     // 3 seconds for "Initializing..." message
#define DISPLAY_ROTATION_INTERVAL 3000  // 3 seconds between display screens
#define SENSOR_READ_INTERVAL 1000       // 1 second between sensor reads
#define KEYPAD_TIMEOUT 10000            // 10 seconds auto-clear
#define INACTIVITY_TIMEOUT 60000        // 60 seconds return to locked
#define LOCKOUT_DURATION 30000          // 30 seconds lockout
#define AUTO_MOTOR_INTERVAL 2000        // 2 seconds between auto rotations

// Alert Thresholds
#define DISTANCE_ALERT_THRESHOLD 20     // cm
#define WATER_ALERT_THRESHOLD 500       // analog value
#define TEMP_HIGH_THRESHOLD 30.0        // °C
#define TEMP_LOW_THRESHOLD 10.0         // °C
#define HUMIDITY_ALERT_THRESHOLD 80.0   // %

// Security Configuration
#define DEFAULT_PASSCODE "1234*"
#define MAX_PASSCODE_LENGTH 6
#define MAX_FAILED_ATTEMPTS 3

// Joystick Configuration
#define JOYSTICK_CENTER 512             // Center position (0-1023)
#define JOYSTICK_DEADZONE 50            // Dead zone around center
#define JOYSTICK_THRESHOLD 200          // Threshold for movement detection

// Global Variables
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
DHT dht(DHT_PIN, DHT_TYPE);
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
Stepper stepper(STEPS_PER_REVOLUTION, STEPPER_IN1_PIN, STEPPER_IN2_PIN, STEPPER_IN3_PIN, STEPPER_IN4_PIN);

// LED control variables (from Phase 1)
unsigned long lastBlinkTime = 0;
bool currentLED = false; // false = green, true = red
bool startupComplete = false;
int startupBlinkCount = 0;
unsigned long lastStartupBlink = 0;

// LCD control variables
bool lcdInitialized = false;
bool lcdStartupComplete = false;
unsigned long lcdStartupTime = 0;

// Display rotation variables
int currentDisplayScreen = 0;  // 0=temp/humidity, 1=distance, 2=water, 3=motor status
unsigned long lastDisplayUpdate = 0;

// Sensor data variables
float temperature = 0.0;
float humidity = 0.0;
float distance = 0.0;
int waterLevel = 0;
bool sensorDataValid = false;

// Alert status variables
bool distanceAlert = false;
bool waterAlert = false;
bool temperatureAlert = false;
bool humidityAlert = false;
bool anyAlert = false;

// Sensor read timing
unsigned long lastSensorRead = 0;

// Security system variables
bool systemUnlocked = false;
String enteredPasscode = "";
unsigned long lastKeypadActivity = 0;
unsigned long lastSystemActivity = 0;
int failedAttempts = 0;
bool systemLockedOut = false;
unsigned long lockoutStartTime = 0;

// Motor control variables
bool motorActive = false;
bool autoMode = true;  // true = auto mode, false = manual mode
unsigned long lastAutoMotorMove = 0;
int currentMotorPosition = 0;  // Current position in steps
bool motorMoving = false;

// Joystick variables
int joystickX = 0;
int joystickY = 0;
bool joystickButtonPressed = false;
bool lastJoystickButtonState = false;
unsigned long lastJoystickRead = 0;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("=== Arduino IoT Workshop - Phase 5 ===");
  Serial.println("Initializing complete system with motor control...");
  
  // Initialize LED pins as outputs (from Phase 1)
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  
  // Initialize ultrasonic sensor pins
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  
  // Initialize joystick button pin
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
  
  // Turn off both LEDs initially
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  
  Serial.println("LED, sensor, keypad, and joystick pins initialized successfully");
  
  // Initialize LCD display
  initializeLCD();
  
  // Initialize DHT sensor
  dht.begin();
  Serial.println("DHT sensor initialized");
  
  // Initialize stepper motor
  stepper.setSpeed(MOTOR_SPEED);
  Serial.println("Stepper motor initialized");
  
  // Initialize timing variables
  lastBlinkTime = millis();
  lastStartupBlink = millis();
  lastDisplayUpdate = millis();
  lastSensorRead = millis();
  lastKeypadActivity = millis();
  lastSystemActivity = millis();
  lastAutoMotorMove = millis();
  lastJoystickRead = millis();
  
  // Initialize security system
  systemUnlocked = false;
  enteredPasscode = "";
  failedAttempts = 0;
  systemLockedOut = false;
  
  // Initialize motor system
  motorActive = false;
  autoMode = true;
  currentMotorPosition = 0;
  motorMoving = false;
  
  Serial.println("Complete system initialized - System LOCKED");
}

void loop() {
  // Handle startup sequence first
  if (!startupComplete) {
    blinkStartup();
  } else {
    // Main complete system operation
    handleCompleteSystem();
  }
  
  // Handle LCD startup sequence
  if (lcdInitialized && !lcdStartupComplete) {
    handleLCDStartup();
  }
}

void handleCompleteSystem() {
  unsigned long currentTime = millis();
  
  // Handle keypad input
  handleKeypadInput();
  
  // Check for system lockout
  if (systemLockedOut) {
    handleLockout();
    return;
  }
  
  // Check for inactivity timeout
  if (systemUnlocked && (currentTime - lastSystemActivity > INACTIVITY_TIMEOUT)) {
    lockSystem();
    return;
  }
  
  // If system is unlocked, run complete dashboard
  if (systemUnlocked) {
    handleCompleteDashboard();
  } else {
    // System is locked - show passcode entry screen
    displayPasscodeEntry();
  }
}

void handleCompleteDashboard() {
  unsigned long currentTime = millis();
  
  // Read sensors at regular intervals
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readAllSensors();
    checkAlertConditions();
    updateStatusLEDs();
    lastSensorRead = currentTime;
  }
  
  // Read joystick at regular intervals
  if (currentTime - lastJoystickRead >= 50) {  // 20Hz joystick reading
    readJoystick();
    lastJoystickRead = currentTime;
  }
  
  // Control stepper motor
  controlStepperMotor();
  
  // Update display rotation (now includes motor status)
  if (currentTime - lastDisplayUpdate >= DISPLAY_ROTATION_INTERVAL) {
    updateLCDDisplay();
    lastDisplayUpdate = currentTime;
  }
}

void controlStepperMotor() {
  if (!systemUnlocked) {
    motorActive = false;
    return;
  }
  
  // Activate motor after system unlock
  if (!motorActive) {
    motorActive = true;
    Serial.println("Motor activated - entering auto mode");
  }
  
  if (autoMode) {
    autoMotorMovement();
  } else {
    manualMotorControl();
  }
}

void autoMotorMovement() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastAutoMotorMove >= AUTO_MOTOR_INTERVAL && !motorMoving) {
    Serial.println("Auto motor movement - rotating 90 degrees");
    stepMotor(STEPS_PER_90_DEGREES, 1);  // 1 = clockwise
    lastAutoMotorMove = currentTime;
  }
}

void manualMotorControl() {
  // Manual control based on joystick input
  if (abs(joystickX - JOYSTICK_CENTER) > JOYSTICK_THRESHOLD && !motorMoving) {
    int direction = (joystickX > JOYSTICK_CENTER) ? 1 : -1;  // 1 = clockwise, -1 = counterclockwise
    int steps = map(abs(joystickX - JOYSTICK_CENTER), JOYSTICK_THRESHOLD, 512, 10, 50);
    
    Serial.print("Manual motor control - Direction: ");
    Serial.print(direction);
    Serial.print(", Steps: ");
    Serial.println(steps);
    
    stepMotor(steps, direction);
  }
}

void stepMotor(int steps, int direction) {
  if (motorMoving) return;  // Prevent overlapping movements
  
  motorMoving = true;
  
  // Smooth acceleration/deceleration
  int accelerationSteps = min(steps / 4, 50);  // First 25% for acceleration
  int decelerationSteps = min(steps / 4, 50);  // Last 25% for deceleration
  int constantSteps = steps - accelerationSteps - decelerationSteps;
  
  // Acceleration phase
  for (int i = 0; i < accelerationSteps; i++) {
    if (direction > 0) {
      stepper.step(1);
      currentMotorPosition++;
    } else {
      stepper.step(-1);
      currentMotorPosition--;
    }
    delayMicroseconds(2000);  // Slower for acceleration
  }
  
  // Constant speed phase
  for (int i = 0; i < constantSteps; i++) {
    if (direction > 0) {
      stepper.step(1);
      currentMotorPosition++;
    } else {
      stepper.step(-1);
      currentMotorPosition--;
    }
    delayMicroseconds(1000);  // Normal speed
  }
  
  // Deceleration phase
  for (int i = 0; i < decelerationSteps; i++) {
    if (direction > 0) {
      stepper.step(1);
      currentMotorPosition++;
    } else {
      stepper.step(-1);
      currentMotorPosition--;
    }
    delayMicroseconds(2000);  // Slower for deceleration
  }
  
  motorMoving = false;
  
  Serial.print("Motor movement complete - Position: ");
  Serial.println(currentMotorPosition);
}

void readJoystick() {
  joystickX = analogRead(JOYSTICK_X_PIN);
  joystickY = analogRead(JOYSTICK_Y_PIN);
  
  // Read joystick button with debouncing
  bool currentButtonState = !digitalRead(JOYSTICK_BUTTON_PIN);  // Inverted due to pullup
  
  if (currentButtonState && !lastJoystickButtonState) {
    // Button pressed - toggle auto/manual mode
    autoMode = !autoMode;
    Serial.print("Mode switched to: ");
    Serial.println(autoMode ? "AUTO" : "MANUAL");
    
    // Visual feedback
    if (lcdInitialized) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Mode: ");
      lcd.print(autoMode ? "AUTO" : "MANUAL");
      lcd.setCursor(0, 1);
      lcd.print("Motor Control");
      delay(1000);
    }
  }
  
  lastJoystickButtonState = currentButtonState;
}

void updateLCDDisplay() {
  if (!lcdInitialized || !sensorDataValid) return;
  
  lcd.clear();
  
  switch (currentDisplayScreen) {
    case 0:  // Temperature and Humidity
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperature, 1);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Humidity: ");
      lcd.print(humidity, 1);
      lcd.print("%");
      break;
      
    case 1:  // Distance and Status
      lcd.setCursor(0, 0);
      lcd.print("Distance: ");
      if (distance == 999) {
        lcd.print("ERR");
      } else {
        lcd.print(distance, 0);
        lcd.print("cm");
      }
      lcd.setCursor(0, 1);
      lcd.print("Status: ");
      if (distanceAlert) {
        lcd.print("DETECTED");
      } else {
        lcd.print("CLEAR");
      }
      break;
      
    case 2:  // Water Level and Status
      lcd.setCursor(0, 0);
      lcd.print("Water Level: ");
      lcd.print(waterLevel);
      lcd.setCursor(0, 1);
      lcd.print("Status: ");
      if (waterAlert) {
        lcd.print("WET");
      } else {
        lcd.print("DRY");
      }
      break;
      
    case 3:  // Motor Status
      lcd.setCursor(0, 0);
      lcd.print("Motor: ");
      lcd.print(motorActive ? "Active" : "Stopped");
      lcd.setCursor(0, 1);
      lcd.print("Mode: ");
      lcd.print(autoMode ? "AUTO" : "MANUAL");
      break;
  }
  
  // Move to next screen (now 4 screens)
  currentDisplayScreen = (currentDisplayScreen + 1) % 4;
  
  Serial.print("Display updated - Screen ");
  Serial.println(currentDisplayScreen);
}

// All previous functions from Phase 4 remain the same
void handleKeypadInput() {
  char key = keypad.getKey();
  
  if (key) {
    lastKeypadActivity = millis();
    lastSystemActivity = millis();
    
    Serial.print("Key pressed: ");
    Serial.println(key);
    
    if (systemLockedOut) {
      return;
    }
    
    if (key == '#') {
      clearEntry();
    } else if (key == '*') {
      checkPasscode();
    } else if (enteredPasscode.length() < MAX_PASSCODE_LENGTH - 1) {
      enteredPasscode += key;
      Serial.print("Passcode so far: ");
      Serial.println(enteredPasscode);
    }
  }
  
  if (!enteredPasscode.isEmpty() && (millis() - lastKeypadActivity > KEYPAD_TIMEOUT)) {
    clearEntry();
  }
}

void checkPasscode() {
  Serial.print("Checking passcode: ");
  Serial.println(enteredPasscode);
  
  if (enteredPasscode == DEFAULT_PASSCODE) {
    grantAccess();
  } else {
    denyAccess();
  }
  
  clearEntry();
}

void grantAccess() {
  Serial.println("ACCESS GRANTED!");
  systemUnlocked = true;
  failedAttempts = 0;
  
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(GREEN_LED_PIN, LOW);
  
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCESS GRANTED");
    lcd.setCursor(0, 1);
    lcd.print("Motor Activated");
    delay(2000);
  }
  
  lastSystemActivity = millis();
}

void denyAccess() {
  Serial.println("ACCESS DENIED!");
  failedAttempts++;
  
  for (int i = 0; i < 3; i++) {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(200);
    digitalWrite(RED_LED_PIN, LOW);
    delay(200);
  }
  
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCESS DENIED");
    lcd.setCursor(0, 1);
    lcd.print("Try Again");
    delay(2000);
  }
  
  if (failedAttempts >= MAX_FAILED_ATTEMPTS) {
    lockoutSystem();
  }
}

void lockoutSystem() {
  Serial.println("SYSTEM LOCKED OUT!");
  systemLockedOut = true;
  lockoutStartTime = millis();
  
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("SYSTEM LOCKED");
    lcd.setCursor(0, 1);
    lcd.print("Wait 30 seconds");
  }
}

void handleLockout() {
  unsigned long currentTime = millis();
  
  if (currentTime - lockoutStartTime >= LOCKOUT_DURATION) {
    systemLockedOut = false;
    failedAttempts = 0;
    digitalWrite(RED_LED_PIN, LOW);
    
    Serial.println("Lockout period ended");
    
    if (lcdInitialized) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Lockout Ended");
      lcd.setCursor(0, 1);
      lcd.print("Enter Passcode");
      delay(2000);
    }
  }
}

void clearEntry() {
  enteredPasscode = "";
  Serial.println("Entry cleared");
}

void lockSystem() {
  Serial.println("System locked due to inactivity");
  systemUnlocked = false;
  motorActive = false;
  clearEntry();
  
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System Locked");
    lcd.setCursor(0, 1);
    lcd.print("Enter Passcode");
    delay(2000);
  }
}

void displayPasscodeEntry() {
  if (!lcdInitialized) return;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENTER PASSCODE:");
  lcd.setCursor(0, 1);
  
  for (int i = 0; i < enteredPasscode.length(); i++) {
    lcd.print("*");
  }
  
  if (enteredPasscode.length() < MAX_PASSCODE_LENGTH - 1) {
    lcd.print("_");
  }
}

void readAllSensors() {
  Serial.println("--- Reading all sensors ---");
  
  float newTemp = dht.readTemperature();
  float newHumidity = dht.readHumidity();
  
  if (!isnan(newTemp) && !isnan(newHumidity)) {
    temperature = newTemp;
    humidity = newHumidity;
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  } else {
    Serial.println("DHT sensor read failed");
  }
  
  distance = readUltrasonicDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  waterLevel = analogRead(WATER_SENSOR_PIN);
  Serial.print("Water Level: ");
  Serial.println(waterLevel);
  
  sensorDataValid = true;
  Serial.println("--- Sensor reading complete ---");
}

float readUltrasonicDistance() {
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  
  digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  
  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;
  
  if (distance <= 0 || distance > 400) {
    distance = 999;
  }
  
  return distance;
}

void checkAlertConditions() {
  distanceAlert = false;
  waterAlert = false;
  temperatureAlert = false;
  humidityAlert = false;
  
  if (distance < DISTANCE_ALERT_THRESHOLD && distance > 0) {
    distanceAlert = true;
    Serial.println("ALERT: Proximity detected!");
  }
  
  if (waterLevel > WATER_ALERT_THRESHOLD) {
    waterAlert = true;
    Serial.println("ALERT: Water detected!");
  }
  
  if (temperature > TEMP_HIGH_THRESHOLD || temperature < TEMP_LOW_THRESHOLD) {
    temperatureAlert = true;
    Serial.println("ALERT: Temperature out of range!");
  }
  
  if (humidity > HUMIDITY_ALERT_THRESHOLD) {
    humidityAlert = true;
    Serial.println("ALERT: High humidity detected!");
  }
  
  anyAlert = distanceAlert || waterAlert || temperatureAlert || humidityAlert;
  
  if (anyAlert) {
    Serial.println("*** ALERT CONDITION ACTIVE ***");
  }
}

void updateStatusLEDs() {
  if (anyAlert) {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
  } else {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
  }
}

void initializeLCD() {
  Serial.println("Initializing LCD display...");
  
  Wire.begin();
  
  lcd.init();
  lcd.backlight();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Complete System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  delay(1000);
  
  lcd.clear();
  
  lcdInitialized = true;
  lcdStartupTime = millis();
  
  Serial.println("LCD initialized successfully");
  Serial.println("Displaying startup message...");
}

void displayStartupMessage() {
  if (!lcdInitialized) return;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ENV MONITOR v1.0");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  Serial.println("Startup message displayed");
}

void displaySystemReady() {
  if (!lcdInitialized) return;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("System Ready");
  lcd.setCursor(0, 1);
  
  unsigned long uptime = millis() / 1000;
  lcd.print("Uptime: ");
  lcd.print(uptime);
  lcd.print("s");
  
  Serial.println("System ready message displayed");
}

void handleLCDStartup() {
  unsigned long currentTime = millis();
  
  if (currentTime - lcdStartupTime >= LCD_INIT_DELAY) {
    if (!lcdStartupComplete) {
      displaySystemReady();
      lcdStartupComplete = true;
      Serial.println("LCD startup sequence complete");
    }
  } else {
    if (currentTime - lcdStartupTime < 100) {
      displayStartupMessage();
    }
  }
}

void blinkStartup() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastStartupBlink >= STARTUP_BLINK_DURATION) {
    if (startupBlinkCount < STARTUP_BLINK_COUNT * 2) {
      if (startupBlinkCount % 2 == 0) {
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, HIGH);
        Serial.println("Startup blink ON");
      } else {
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        Serial.println("Startup blink OFF");
      }
      startupBlinkCount++;
    } else {
      startupComplete = true;
      Serial.println("LED startup sequence complete");
    }
    lastStartupBlink = currentTime;
  }
}
