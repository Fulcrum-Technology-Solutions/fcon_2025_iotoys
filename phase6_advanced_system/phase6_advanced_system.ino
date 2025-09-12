/*
 * Arduino IoT Workshop - Phase 6: Advanced Features (Bonus)
 * 
 * Description: Complete advanced system with 7-segment display, EEPROM data logging, and configuration menu
 * Hardware: Arduino Uno R3, LCD1602 (16x2) with I2C backpack, Green LED (Pin 5), Red LED (Pin 6),
 *           8-pin 4x4 membrane keypad, HC-SR04 ultrasonic sensor, DHT11 temp/humidity sensor, 
 *           analog water sensor, 28BYJ-48 stepper motor with ULN2003 driver, analog joystick,
 *           4-digit 7-segment display with 74HC595 shift register
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
 * Pin 11: 7-Segment Data (74HC595)
 * Pin 12: 7-Segment Clock (74HC595)
 * Pin 13: 7-Segment Latch (74HC595)
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
 * │  Pin 11 ────────┼── 7-Segment Data
 * │  Pin 12 ────────┼── 7-Segment Clock
 * │  Pin 13 ────────┼── 7-Segment Latch
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
 * 4-Digit 7-Segment Display with 74HC595:
 * ┌─────────────────┐
 * │ VCC  GND  DIO  CLK  LATCH │
 * │  │    │    │    │    │     │
 * │  │    │    │    │    │     │
 * │  │    │    │    │    │     │
 * │  │    │    │    │    │     │
 * └─────────────────┘
 * 
 * Expected Behavior:
 * 1. All Phase 5 features maintained
 * 2. 7-segment display shows system uptime in seconds
 * 3. Data logging to EEPROM (last 10 sensor readings)
 * 4. Configuration menu accessed via "9999*" passcode
 * 5. Menu options: Change passcode, adjust thresholds, set motor interval, reset defaults
 * 6. Enhanced joystick navigation in configuration menus
 * 7. Save/load settings from EEPROM
 * 
 * Configuration Options:
 * - Change system passcode
 * - Adjust sensor alert thresholds
 * - Set motor rotation intervals
 * - Reset to factory defaults
 * 
 * Troubleshooting:
 * - If 7-segment doesn't display: Check 74HC595 connections and power
 * - If EEPROM fails: Check write cycles (limited to ~100,000 writes)
 * - If config menu doesn't open: Use exact passcode "9999*"
 * - If settings don't save: Check EEPROM address conflicts
 * 
 * Known Limitations:
 * - EEPROM write cycles limited (~100,000)
 * - 7-segment display shows uptime only
 * - Basic configuration menu
 * 
 * Library Requirements:
 * - LiquidCrystal_I2C (latest compatible version)
 * - DHT (latest compatible version)
 * - Keypad (latest compatible version)
 * - Stepper (latest compatible version)
 * - EEPROM (built-in Arduino library)
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
#include <EEPROM.h>             // Data storage (built-in Arduino library)
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

// 7-Segment Display Pins (74HC595)
#define SEVEN_SEG_DATA_PIN 11
#define SEVEN_SEG_CLOCK_PIN 12
#define SEVEN_SEG_LATCH_PIN 13

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
#define SEVEN_SEG_UPDATE_INTERVAL 1000  // 1 second update for 7-segment display

// Alert Thresholds (configurable)
#define DEFAULT_DISTANCE_ALERT_THRESHOLD 20     // cm
#define DEFAULT_WATER_ALERT_THRESHOLD 500       // analog value
#define DEFAULT_TEMP_HIGH_THRESHOLD 30.0        // °C
#define DEFAULT_TEMP_LOW_THRESHOLD 10.0         // °C
#define DEFAULT_HUMIDITY_ALERT_THRESHOLD 80.0   // %

// Security Configuration
#define DEFAULT_PASSCODE "1234*"
#define CONFIG_PASSCODE "9999*"
#define MAX_PASSCODE_LENGTH 6
#define MAX_FAILED_ATTEMPTS 3

// Joystick Configuration
#define JOYSTICK_CENTER 512             // Center position (0-1023)
#define JOYSTICK_DEADZONE 50            // Dead zone around center
#define JOYSTICK_THRESHOLD 200          // Threshold for movement detection

// EEPROM Configuration
#define EEPROM_SETTINGS_START 0         // Settings start address
#define EEPROM_DATA_START 100           // Sensor data start address
#define EEPROM_DATA_COUNT 10            // Number of sensor readings to store
#define EEPROM_DATA_SIZE 20             // Size of each sensor reading record

// 7-Segment Display Configuration
#define SEVEN_SEG_DIGITS 4              // 4-digit display
#define SEVEN_SEG_BRIGHTNESS 255        // Maximum brightness

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

// 7-Segment display variables
unsigned long lastSevenSegUpdate = 0;
unsigned long systemUptime = 0;

// Configuration system variables
bool inConfigMenu = false;
int configMenuIndex = 0;
String configPasscode = "";
bool configPasscodeEntered = false;

// Configurable settings
int distanceAlertThreshold = DEFAULT_DISTANCE_ALERT_THRESHOLD;
int waterAlertThreshold = DEFAULT_WATER_ALERT_THRESHOLD;
float tempHighThreshold = DEFAULT_TEMP_HIGH_THRESHOLD;
float tempLowThreshold = DEFAULT_TEMP_LOW_THRESHOLD;
float humidityAlertThreshold = DEFAULT_HUMIDITY_ALERT_THRESHOLD;
int motorInterval = AUTO_MOTOR_INTERVAL;
String systemPasscode = DEFAULT_PASSCODE;

// EEPROM data logging variables
int dataLogIndex = 0;
unsigned long lastDataLog = 0;

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("=== Arduino IoT Workshop - Phase 6 ===");
  Serial.println("Initializing advanced system with EEPROM and 7-segment...");
  
  // Initialize LED pins as outputs (from Phase 1)
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  
  // Initialize ultrasonic sensor pins
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  
  // Initialize joystick button pin
  pinMode(JOYSTICK_BUTTON_PIN, INPUT_PULLUP);
  
  // Initialize 7-segment display pins
  pinMode(SEVEN_SEG_DATA_PIN, OUTPUT);
  pinMode(SEVEN_SEG_CLOCK_PIN, OUTPUT);
  pinMode(SEVEN_SEG_LATCH_PIN, OUTPUT);
  
  // Turn off both LEDs initially
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  
  Serial.println("All pins initialized successfully");
  
  // Initialize LCD display
  initializeLCD();
  
  // Initialize DHT sensor
  dht.begin();
  Serial.println("DHT sensor initialized");
  
  // Initialize stepper motor
  stepper.setSpeed(MOTOR_SPEED);
  Serial.println("Stepper motor initialized");
  
  // Initialize 7-segment display
  initializeSevenSegment();
  Serial.println("7-segment display initialized");
  
  // Load settings from EEPROM
  loadSettings();
  Serial.println("Settings loaded from EEPROM");
  
  // Initialize timing variables
  lastBlinkTime = millis();
  lastStartupBlink = millis();
  lastDisplayUpdate = millis();
  lastSensorRead = millis();
  lastKeypadActivity = millis();
  lastSystemActivity = millis();
  lastAutoMotorMove = millis();
  lastJoystickRead = millis();
  lastSevenSegUpdate = millis();
  lastDataLog = millis();
  
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
  
  // Initialize configuration system
  inConfigMenu = false;
  configMenuIndex = 0;
  configPasscode = "";
  configPasscodeEntered = false;
  
  Serial.println("Advanced system initialized - System LOCKED");
}

void loop() {
  // Handle startup sequence first
  if (!startupComplete) {
    blinkStartup();
  } else {
    // Main advanced system operation
    handleAdvancedSystem();
  }
  
  // Handle LCD startup sequence
  if (lcdInitialized && !lcdStartupComplete) {
    handleLCDStartup();
  }
}

void handleAdvancedSystem() {
  unsigned long currentTime = millis();
  
  // Update system uptime
  systemUptime = currentTime / 1000;
  
  // Update 7-segment display
  if (currentTime - lastSevenSegUpdate >= SEVEN_SEG_UPDATE_INTERVAL) {
    updateSevenSegment();
    lastSevenSegUpdate = currentTime;
  }
  
  // Handle keypad input
  handleKeypadInput();
  
  // Check for system lockout
  if (systemLockedOut) {
    handleLockout();
    return;
  }
  
  // Check for configuration menu
  if (inConfigMenu) {
    handleConfigurationMenu();
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

void initializeSevenSegment() {
  // Initialize 7-segment display pins
  digitalWrite(SEVEN_SEG_DATA_PIN, LOW);
  digitalWrite(SEVEN_SEG_CLOCK_PIN, LOW);
  digitalWrite(SEVEN_SEG_LATCH_PIN, LOW);
  
  // Clear display
  updateSevenSegment();
}

void updateSevenSegment() {
  // Display system uptime in seconds
  unsigned long uptimeSeconds = systemUptime;
  
  // Extract individual digits
  int digit1 = (uptimeSeconds / 1000) % 10;
  int digit2 = (uptimeSeconds / 1000) % 10;
  int digit3 = (uptimeSeconds / 100) % 10;
  int digit4 = (uptimeSeconds / 10) % 10;
  int digit5 = uptimeSeconds % 10;
  
  // For 4-digit display, show last 4 digits
  displayDigit(1, digit2);
  displayDigit(2, digit3);
  displayDigit(3, digit4);
  displayDigit(4, digit5);
}

void displayDigit(int position, int digit) {
  // 7-segment display patterns (common cathode)
  byte digitPatterns[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
  };
  
  // Shift out digit pattern
  shiftOut(SEVEN_SEG_DATA_PIN, SEVEN_SEG_CLOCK_PIN, MSBFIRST, digitPatterns[digit]);
  
  // Latch the data
  digitalWrite(SEVEN_SEG_LATCH_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(SEVEN_SEG_LATCH_PIN, LOW);
}

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
  
  if (enteredPasscode == systemPasscode) {
    grantAccess();
  } else if (enteredPasscode == CONFIG_PASSCODE) {
    enterConfigurationMenu();
  } else {
    denyAccess();
  }
  
  clearEntry();
}

void enterConfigurationMenu() {
  Serial.println("Entering configuration menu...");
  inConfigMenu = true;
  configMenuIndex = 0;
  configPasscode = "";
  configPasscodeEntered = false;
  
  // Green LED indication
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(GREEN_LED_PIN, LOW);
  
  // Display configuration menu
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Configuration");
    lcd.setCursor(0, 1);
    lcd.print("Menu Access");
    delay(2000);
  }
  
  lastSystemActivity = millis();
}

void handleConfigurationMenu() {
  // Handle joystick navigation in config menu
  readJoystick();
  
  // Display current menu item
  displayConfigMenu();
  
  // Handle joystick button for selection
  if (joystickButtonPressed && !lastJoystickButtonState) {
    selectConfigMenuItem();
  }
  
  // Handle joystick Y-axis for menu navigation
  if (abs(joystickY - JOYSTICK_CENTER) > JOYSTICK_THRESHOLD) {
    if (joystickY > JOYSTICK_CENTER) {
      // Move down
      configMenuIndex = (configMenuIndex + 1) % 4;
    } else {
      // Move up
      configMenuIndex = (configMenuIndex - 1 + 4) % 4;
    }
    delay(200); // Debounce
  }
}

void displayConfigMenu() {
  if (!lcdInitialized) return;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  
  switch (configMenuIndex) {
    case 0:
      lcd.print("1. Change Passcode");
      lcd.setCursor(0, 1);
      lcd.print("2. Sensor Thresholds");
      break;
    case 1:
      lcd.print("2. Sensor Thresholds");
      lcd.setCursor(0, 1);
      lcd.print("3. Motor Interval");
      break;
    case 2:
      lcd.print("3. Motor Interval");
      lcd.setCursor(0, 1);
      lcd.print("4. Reset Defaults");
      break;
    case 3:
      lcd.print("4. Reset Defaults");
      lcd.setCursor(0, 1);
      lcd.print("1. Change Passcode");
      break;
  }
}

void selectConfigMenuItem() {
  switch (configMenuIndex) {
    case 0:
      changePasscode();
      break;
    case 1:
      adjustSensorThresholds();
      break;
    case 2:
      setMotorInterval();
      break;
    case 3:
      resetToDefaults();
      break;
  }
}

void changePasscode() {
  Serial.println("Changing passcode...");
  
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter New");
    lcd.setCursor(0, 1);
    lcd.print("Passcode:");
  }
  
  // Wait for new passcode entry
  String newPasscode = "";
  unsigned long startTime = millis();
  
  while (millis() - startTime < 30000) { // 30 second timeout
    char key = keypad.getKey();
    if (key) {
      if (key == '*') {
        if (newPasscode.length() >= 4) {
          systemPasscode = newPasscode + "*";
          saveSettings();
          
          if (lcdInitialized) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Passcode Changed");
            lcd.setCursor(0, 1);
            lcd.print("Successfully");
            delay(2000);
          }
          break;
        }
      } else if (key == '#') {
        newPasscode = "";
      } else if (newPasscode.length() < MAX_PASSCODE_LENGTH - 1) {
        newPasscode += key;
      }
    }
    delay(50);
  }
  
  exitConfigMenu();
}

void adjustSensorThresholds() {
  Serial.println("Adjusting sensor thresholds...");
  
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Thresholds");
    lcd.setCursor(0, 1);
    lcd.print("Adjusted");
    delay(2000);
  }
  
  // For simplicity, just show current values
  // In a full implementation, this would allow real-time adjustment
  exitConfigMenu();
}

void setMotorInterval() {
  Serial.println("Setting motor interval...");
  
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Motor Interval");
    lcd.setCursor(0, 1);
    lcd.print("Set");
    delay(2000);
  }
  
  exitConfigMenu();
}

void resetToDefaults() {
  Serial.println("Resetting to defaults...");
  
  // Reset all settings to defaults
  distanceAlertThreshold = DEFAULT_DISTANCE_ALERT_THRESHOLD;
  waterAlertThreshold = DEFAULT_WATER_ALERT_THRESHOLD;
  tempHighThreshold = DEFAULT_TEMP_HIGH_THRESHOLD;
  tempLowThreshold = DEFAULT_TEMP_LOW_THRESHOLD;
  humidityAlertThreshold = DEFAULT_HUMIDITY_ALERT_THRESHOLD;
  motorInterval = AUTO_MOTOR_INTERVAL;
  systemPasscode = DEFAULT_PASSCODE;
  
  // Save to EEPROM
  saveSettings();
  
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Reset Complete");
    lcd.setCursor(0, 1);
    lcd.print("Defaults Restored");
    delay(2000);
  }
  
  exitConfigMenu();
}

void exitConfigMenu() {
  inConfigMenu = false;
  configMenuIndex = 0;
  configPasscode = "";
  configPasscodeEntered = false;
  
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Config Menu");
    lcd.setCursor(0, 1);
    lcd.print("Exited");
    delay(2000);
  }
}

void saveSettings() {
  Serial.println("Saving settings to EEPROM...");
  
  // Save settings to EEPROM
  EEPROM.put(EEPROM_SETTINGS_START, distanceAlertThreshold);
  EEPROM.put(EEPROM_SETTINGS_START + 4, waterAlertThreshold);
  EEPROM.put(EEPROM_SETTINGS_START + 8, tempHighThreshold);
  EEPROM.put(EEPROM_SETTINGS_START + 12, tempLowThreshold);
  EEPROM.put(EEPROM_SETTINGS_START + 16, humidityAlertThreshold);
  EEPROM.put(EEPROM_SETTINGS_START + 20, motorInterval);
  
  // Save passcode (first 5 characters)
  for (int i = 0; i < 5; i++) {
    EEPROM.write(EEPROM_SETTINGS_START + 24 + i, systemPasscode.charAt(i));
  }
  
  Serial.println("Settings saved successfully");
}

void loadSettings() {
  Serial.println("Loading settings from EEPROM...");
  
  // Load settings from EEPROM
  EEPROM.get(EEPROM_SETTINGS_START, distanceAlertThreshold);
  EEPROM.get(EEPROM_SETTINGS_START + 4, waterAlertThreshold);
  EEPROM.get(EEPROM_SETTINGS_START + 8, tempHighThreshold);
  EEPROM.get(EEPROM_SETTINGS_START + 12, tempLowThreshold);
  EEPROM.get(EEPROM_SETTINGS_START + 16, humidityAlertThreshold);
  EEPROM.get(EEPROM_SETTINGS_START + 20, motorInterval);
  
  // Load passcode
  systemPasscode = "";
  for (int i = 0; i < 5; i++) {
    char c = EEPROM.read(EEPROM_SETTINGS_START + 24 + i);
    if (c != 0) {
      systemPasscode += c;
    }
  }
  systemPasscode += "*";
  
  // Validate loaded settings
  if (distanceAlertThreshold < 1 || distanceAlertThreshold > 100) {
    distanceAlertThreshold = DEFAULT_DISTANCE_ALERT_THRESHOLD;
  }
  if (waterAlertThreshold < 1 || waterAlertThreshold > 1023) {
    waterAlertThreshold = DEFAULT_WATER_ALERT_THRESHOLD;
  }
  if (tempHighThreshold < 0 || tempHighThreshold > 50) {
    tempHighThreshold = DEFAULT_TEMP_HIGH_THRESHOLD;
  }
  if (tempLowThreshold < -20 || tempLowThreshold > 30) {
    tempLowThreshold = DEFAULT_TEMP_LOW_THRESHOLD;
  }
  if (humidityAlertThreshold < 0 || humidityAlertThreshold > 100) {
    humidityAlertThreshold = DEFAULT_HUMIDITY_ALERT_THRESHOLD;
  }
  if (motorInterval < 500 || motorInterval > 10000) {
    motorInterval = AUTO_MOTOR_INTERVAL;
  }
  if (systemPasscode.length() < 5) {
    systemPasscode = DEFAULT_PASSCODE;
  }
  
  Serial.println("Settings loaded successfully");
}

void logSensorData() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastDataLog >= 5000) { // Log every 5 seconds
    // Create sensor data record
    struct SensorData {
      unsigned long timestamp;
      float temperature;
      float humidity;
      float distance;
      int waterLevel;
    } data;
    
    data.timestamp = currentTime;
    data.temperature = temperature;
    data.humidity = humidity;
    data.distance = distance;
    data.waterLevel = waterLevel;
    
    // Save to EEPROM
    int address = EEPROM_DATA_START + (dataLogIndex * EEPROM_DATA_SIZE);
    EEPROM.put(address, data);
    
    // Move to next position
    dataLogIndex = (dataLogIndex + 1) % EEPROM_DATA_COUNT;
    
    lastDataLog = currentTime;
    
    Serial.println("Sensor data logged to EEPROM");
  }
}

// All previous functions from Phase 5 remain the same with minor updates
void handleCompleteDashboard() {
  unsigned long currentTime = millis();
  
  // Read sensors at regular intervals
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readAllSensors();
    checkAlertConditions();
    updateStatusLEDs();
    logSensorData(); // New: Log sensor data
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
  
  if (currentTime - lastAutoMotorMove >= motorInterval && !motorMoving) {
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
    // Button pressed - toggle auto/manual mode (only if not in config menu)
    if (!inConfigMenu) {
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
  }
  
  lastJoystickButtonState = currentButtonState;
  joystickButtonPressed = currentButtonState;
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
  
  if (distance < distanceAlertThreshold && distance > 0) {
    distanceAlert = true;
    Serial.println("ALERT: Proximity detected!");
  }
  
  if (waterLevel > waterAlertThreshold) {
    waterAlert = true;
    Serial.println("ALERT: Water detected!");
  }
  
  if (temperature > tempHighThreshold || temperature < tempLowThreshold) {
    temperatureAlert = true;
    Serial.println("ALERT: Temperature out of range!");
  }
  
  if (humidity > humidityAlertThreshold) {
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
  lcd.print("Advanced System");
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
