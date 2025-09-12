/*
 * Arduino IoT Workshop - Phase 4: Security Keypad Integration
 * 
 * Description: Security system with keypad access control and sensor dashboard
 * Hardware: Arduino Uno R3, LCD1602 (16x2) with I2C backpack, Green LED (Pin 5), Red LED (Pin 6),
 *           8-pin 4x4 membrane keypad, HC-SR04 ultrasonic sensor, DHT11 temp/humidity sensor, analog water sensor
 * 
 * Pin Assignments:
 * Pin 2:  Ultrasonic Trigger
 * Pin 3:  Ultrasonic Echo  
 * Pin 4:  DHT11 Data
 * Pin 5:  Green LED (success/positive feedback)
 * Pin 6:  Red LED (error/negative feedback)
 * Pin 14: Keypad Row 1
 * Pin 15: Keypad Row 2
 * Pin 16: Keypad Row 3
 * Pin 17: Keypad Row 4
 * Pin 18: Keypad Col 1
 * Pin 19: Keypad Col 2
 * Pin 20: Keypad Col 3
 * Pin 21: Keypad Col 4
 * A0: Water Sensor
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
 * │  Pin 14 ────────┼── Keypad Row 1
 * │  Pin 15 ────────┼── Keypad Row 2
 * │  Pin 16 ────────┼── Keypad Row 3
 * │  Pin 17 ────────┼── Keypad Row 4
 * │  Pin 18 ────────┼── Keypad Col 1
 * │  Pin 19 ────────┼── Keypad Col 2
 * │  Pin 20 ────────┼── Keypad Col 3
 * │  Pin 21 ────────┼── Keypad Col 4
 * │  A0 ────────────┼── Water Sensor
 * │  A4 (SDA) ──────┼── LCD I2C SDA
 * │  A5 (SCL) ──────┼── LCD I2C SCL
 * │  VCC ───────────┼── LCD I2C VCC (5V)
 * │  GND ───────────┼── LCD I2C GND
 * │                 │
 * └─────────────────┘
 * 
 * 8-Pin Keypad Matrix (4x4 membrane):
 * ┌─────────────────┐
 * │ 1  2  3  A      │
 * │ 4  5  6  B      │
 * │ 7  8  9  C      │
 * │ *  0  #  D      │
 * └─────────────────┘
 * 
 * Expected Behavior:
 * 1. System starts in LOCKED mode - sensors hidden
 * 2. LCD shows "ENTER PASSCODE:" with cursor
 * 3. Enter passcode "1234*" (* confirms entry)
 * 4. Correct passcode: Green LED, "ACCESS GRANTED", show sensor dashboard
 * 5. Wrong passcode: Red LED blink 3 times, "ACCESS DENIED", clear input
 * 6. Auto-clear partial entry after 10 seconds of inactivity
 * 7. Return to locked mode after 60 seconds of inactivity
 * 8. Maximum 3 failed attempts before 30-second lockout
 * 
 * Security Features:
 * - Default passcode: "1234*"
 * - Asterisk display for entered digits
 * - 3 failed attempts = 30s lockout
 * - Auto-clear partial entries
 * - Return to locked mode after inactivity
 * 
 * Troubleshooting:
 * - If keypad doesn't respond: Check pin connections and power
 * - If passcode not accepted: Verify default passcode "1234*"
 * - If system locks out: Wait 30 seconds or reset Arduino
 * - If sensors don't show: Enter correct passcode first
 * 
 * Known Limitations:
 * - No motor control yet
 * - Basic security system only
 * - No configuration menu yet
 * 
 * Upgrade Path to Phase 5:
 * - Add stepper motor control
 * - Implement joystick control
 * - Add motor automation features
 * 
 * Library Requirements:
 * - LiquidCrystal_I2C (latest compatible version)
 * - DHT (latest compatible version)
 * - Keypad (latest compatible version)
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
#include <Wire.h>               // I2C communication (built-in)

// Pin Definitions
#define GREEN_LED_PIN 5
#define RED_LED_PIN 6
#define ULTRASONIC_TRIGGER_PIN 2
#define ULTRASONIC_ECHO_PIN 3
#define DHT_PIN 4
#define WATER_SENSOR_PIN A0

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

// Global Variables
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
DHT dht(DHT_PIN, DHT_TYPE);
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

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
int currentDisplayScreen = 0;  // 0=temp/humidity, 1=distance, 2=water
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

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("=== Arduino IoT Workshop - Phase 4 ===");
  Serial.println("Initializing security system...");
  
  // Initialize LED pins as outputs (from Phase 1)
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  
  // Initialize ultrasonic sensor pins
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  
  // Turn off both LEDs initially
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  
  Serial.println("LED, sensor, and keypad pins initialized successfully");
  
  // Initialize LCD display
  initializeLCD();
  
  // Initialize DHT sensor
  dht.begin();
  Serial.println("DHT sensor initialized");
  
  // Initialize timing variables
  lastBlinkTime = millis();
  lastStartupBlink = millis();
  lastDisplayUpdate = millis();
  lastSensorRead = millis();
  lastKeypadActivity = millis();
  lastSystemActivity = millis();
  
  // Initialize security system
  systemUnlocked = false;
  enteredPasscode = "";
  failedAttempts = 0;
  systemLockedOut = false;
  
  Serial.println("Security system initialized - System LOCKED");
}

void loop() {
  // Handle startup sequence first
  if (!startupComplete) {
    blinkStartup();
  } else {
    // Main security system operation
    handleSecuritySystem();
  }
  
  // Handle LCD startup sequence
  if (lcdInitialized && !lcdStartupComplete) {
    handleLCDStartup();
  }
}

void handleSecuritySystem() {
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
  
  // If system is unlocked, run sensor dashboard
  if (systemUnlocked) {
    handleSensorDashboard();
  } else {
    // System is locked - show passcode entry screen
    displayPasscodeEntry();
  }
}

void handleKeypadInput() {
  char key = keypad.getKey();
  
  if (key) {
    lastKeypadActivity = millis();
    lastSystemActivity = millis();
    
    Serial.print("Key pressed: ");
    Serial.println(key);
    
    if (systemLockedOut) {
      // Ignore keypad input during lockout
      return;
    }
    
    if (key == '#') {
      // Clear current entry
      clearEntry();
    } else if (key == '*') {
      // Confirm passcode entry
      checkPasscode();
    } else if (enteredPasscode.length() < MAX_PASSCODE_LENGTH - 1) {
      // Add digit to passcode
      enteredPasscode += key;
      Serial.print("Passcode so far: ");
      Serial.println(enteredPasscode);
    }
  }
  
  // Auto-clear partial entry after timeout
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
  failedAttempts = 0; // Reset failed attempts on successful entry
  
  // Green LED indication
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  delay(1000);
  digitalWrite(GREEN_LED_PIN, LOW);
  
  // Display access granted message
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCESS GRANTED");
    lcd.setCursor(0, 1);
    lcd.print("System Unlocked");
    delay(2000);
  }
  
  lastSystemActivity = millis();
}

void denyAccess() {
  Serial.println("ACCESS DENIED!");
  failedAttempts++;
  
  // Red LED blink 3 times
  for (int i = 0; i < 3; i++) {
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(200);
    digitalWrite(RED_LED_PIN, LOW);
    delay(200);
  }
  
  // Display access denied message
  if (lcdInitialized) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCESS DENIED");
    lcd.setCursor(0, 1);
    lcd.print("Try Again");
    delay(2000);
  }
  
  // Check for lockout condition
  if (failedAttempts >= MAX_FAILED_ATTEMPTS) {
    lockoutSystem();
  }
}

void lockoutSystem() {
  Serial.println("SYSTEM LOCKED OUT!");
  systemLockedOut = true;
  lockoutStartTime = millis();
  
  // Red LED stays on during lockout
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);
  
  // Display lockout message
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
    // Lockout period ended
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
  clearEntry();
  
  // Turn off LEDs
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
  
  // Display entered digits as asterisks
  for (int i = 0; i < enteredPasscode.length(); i++) {
    lcd.print("*");
  }
  
  // Show cursor
  if (enteredPasscode.length() < MAX_PASSCODE_LENGTH - 1) {
    lcd.print("_");
  }
}

void handleSensorDashboard() {
  unsigned long currentTime = millis();
  
  // Read sensors at regular intervals
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    readAllSensors();
    checkAlertConditions();
    updateStatusLEDs();
    lastSensorRead = currentTime;
  }
  
  // Update display rotation
  if (currentTime - lastDisplayUpdate >= DISPLAY_ROTATION_INTERVAL) {
    updateLCDDisplay();
    lastDisplayUpdate = currentTime;
  }
}

void readAllSensors() {
  Serial.println("--- Reading all sensors ---");
  
  // Read DHT11 temperature and humidity
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
  
  // Read ultrasonic distance sensor
  distance = readUltrasonicDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Read water sensor
  waterLevel = analogRead(WATER_SENSOR_PIN);
  Serial.print("Water Level: ");
  Serial.println(waterLevel);
  
  sensorDataValid = true;
  Serial.println("--- Sensor reading complete ---");
}

float readUltrasonicDistance() {
  // Clear trigger pin
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  
  // Send 10us pulse to trigger pin
  digitalWrite(ULTRASONIC_TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(ULTRASONIC_TRIGGER_PIN, LOW);
  
  // Read echo pin and calculate distance
  long duration = pulseIn(ULTRASONIC_ECHO_PIN, HIGH);
  float distance = duration * 0.034 / 2;  // Convert to cm
  
  // Handle timeout or invalid readings
  if (distance <= 0 || distance > 400) {
    distance = 999;  // Invalid reading indicator
  }
  
  return distance;
}

void checkAlertConditions() {
  // Reset alert flags
  distanceAlert = false;
  waterAlert = false;
  temperatureAlert = false;
  humidityAlert = false;
  
  // Check distance alert
  if (distance < DISTANCE_ALERT_THRESHOLD && distance > 0) {
    distanceAlert = true;
    Serial.println("ALERT: Proximity detected!");
  }
  
  // Check water alert
  if (waterLevel > WATER_ALERT_THRESHOLD) {
    waterAlert = true;
    Serial.println("ALERT: Water detected!");
  }
  
  // Check temperature alert
  if (temperature > TEMP_HIGH_THRESHOLD || temperature < TEMP_LOW_THRESHOLD) {
    temperatureAlert = true;
    Serial.println("ALERT: Temperature out of range!");
  }
  
  // Check humidity alert
  if (humidity > HUMIDITY_ALERT_THRESHOLD) {
    humidityAlert = true;
    Serial.println("ALERT: High humidity detected!");
  }
  
  // Set overall alert status
  anyAlert = distanceAlert || waterAlert || temperatureAlert || humidityAlert;
  
  if (anyAlert) {
    Serial.println("*** ALERT CONDITION ACTIVE ***");
  }
}

void updateStatusLEDs() {
  if (anyAlert) {
    // Red LED for alerts
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
  } else {
    // Green LED for normal operation
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
  }
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
  }
  
  // Move to next screen
  currentDisplayScreen = (currentDisplayScreen + 1) % 3;
  
  Serial.print("Display updated - Screen ");
  Serial.println(currentDisplayScreen);
}

// LCD control functions from Phase 2
void initializeLCD() {
  Serial.println("Initializing LCD display...");
  
  // Initialize I2C communication
  Wire.begin();
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  
  // Test LCD communication
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Security System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  delay(1000); // Brief test display
  
  // Clear and prepare for startup message
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
  
  // Display current uptime in seconds
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
    // Still in initialization phase
    if (currentTime - lcdStartupTime < 100) {
      // Show startup message immediately
      displayStartupMessage();
    }
  }
}

// LED control functions from Phase 1
void blinkStartup() {
  unsigned long currentTime = millis();
  
  // Check if it's time for next startup blink
  if (currentTime - lastStartupBlink >= STARTUP_BLINK_DURATION) {
    if (startupBlinkCount < STARTUP_BLINK_COUNT * 2) { // *2 for on/off cycles
      if (startupBlinkCount % 2 == 0) {
        // Turn on both LEDs
        digitalWrite(GREEN_LED_PIN, HIGH);
        digitalWrite(RED_LED_PIN, HIGH);
        Serial.println("Startup blink ON");
      } else {
        // Turn off both LEDs
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(RED_LED_PIN, LOW);
        Serial.println("Startup blink OFF");
      }
      startupBlinkCount++;
    } else {
      // Startup sequence complete
      startupComplete = true;
      Serial.println("LED startup sequence complete");
    }
    lastStartupBlink = currentTime;
  }
}
