/*
 * Arduino IoT Workshop - Phase 3: Multi-Sensor Data Collection
 * 
 * Description: Multi-sensor dashboard with rotating display and alert system
 * Hardware: Arduino Uno R3, LCD1602 (16x2) with I2C backpack, Green LED (Pin 5), Red LED (Pin 6),
 *           HC-SR04 ultrasonic sensor, DHT11 temp/humidity sensor, analog water sensor
 * 
 * Pin Assignments:
 * Pin 2:  Ultrasonic Trigger
 * Pin 3:  Ultrasonic Echo  
 * Pin 4:  DHT11 Data
 * Pin 5:  Green LED (success/positive feedback)
 * Pin 6:  Red LED (error/negative feedback)
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
 * │  A0 ────────────┼── Water Sensor
 * │  A4 (SDA) ──────┼── LCD I2C SDA
 * │  A5 (SCL) ──────┼── LCD I2C SCL
 * │  VCC ───────────┼── LCD I2C VCC (5V)
 * │  GND ───────────┼── LCD I2C GND
 * │                 │
 * └─────────────────┘
 * 
 * Sensor Connections:
 * 
 * HC-SR04 Ultrasonic Sensor:
 * ┌─────────────────┐
 * │ VCC  GND  Trig  Echo │
 * │  │    │    │     │   │
 * │  │    │    │     │   │
 * │  │    │    │     │   │
 * │  │    │    │     │   │
 * └─────────────────┘
 * 
 * DHT11 Temperature/Humidity:
 * ┌─────────────────┐
 * │ VCC  GND  Data  NC  │
 * │  │    │    │    │   │
 * │  │    │    │    │   │
 * │  │    │    │    │   │
 * │  │    │    │    │   │
 * └─────────────────┘
 * 
 * Expected Behavior:
 * 1. Startup: Both LEDs blink 3 times simultaneously
 * 2. LCD initialization with sensor readings
 * 3. Rotating display screens (3-second intervals):
 *    - Screen 1: Temperature/Humidity
 *    - Screen 2: Distance/Status (CLEAR/DETECTED)
 *    - Screen 3: Water Level/Status (DRY/WET)
 * 4. LED indicators: Green when normal, Red when alerts
 * 5. Serial output of all sensor values
 * 
 * Alert Conditions:
 * - Distance < 20cm (proximity detection)
 * - Water sensor > 500 (water detected)
 * - Temperature > 30°C or < 10°C
 * - Humidity > 80%
 * 
 * Troubleshooting:
 * - If sensors don't read: Check wiring and power connections
 * - If DHT11 fails: Check data pin and power supply
 * - If ultrasonic fails: Check trigger/echo pins and power
 * - If water sensor erratic: Check analog connection and calibration
 * - If LCD doesn't display: Check I2C address and connections
 * 
 * Known Limitations:
 * - No keypad integration yet
 * - No motor control yet
 * - Basic sensor reading only
 * 
 * Upgrade Path to Phase 4:
 * - Add security keypad system
 * - Implement access control
 * - Hide sensor data behind authentication
 * 
 * Library Requirements:
 * - LiquidCrystal_I2C (latest compatible version)
 * - DHT (latest compatible version)
 * - Wire (built-in I2C library)
 * 
 * Version: 1.0
 * Created: 2025
 * Compatible: Arduino IDE 1.8.x / 2.x
 */

// Include required libraries
#include <LiquidCrystal_I2C.h>  // LCD display library (latest compatible version)
#include <DHT.h>                // Temperature/humidity sensor (latest compatible version)
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

// Timing Constants
#define STARTUP_BLINK_COUNT 3
#define STARTUP_BLINK_DURATION 200
#define ALTERNATING_DURATION 500
#define LCD_INIT_DELAY 3000     // 3 seconds for "Initializing..." message
#define DISPLAY_ROTATION_INTERVAL 3000  // 3 seconds between display screens
#define SENSOR_READ_INTERVAL 1000       // 1 second between sensor reads

// Alert Thresholds
#define DISTANCE_ALERT_THRESHOLD 20     // cm
#define WATER_ALERT_THRESHOLD 500       // analog value
#define TEMP_HIGH_THRESHOLD 30.0        // °C
#define TEMP_LOW_THRESHOLD 10.0         // °C
#define HUMIDITY_ALERT_THRESHOLD 80.0   // %

// Global Variables
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);
DHT dht(DHT_PIN, DHT_TYPE);

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

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(9600);
  Serial.println("=== Arduino IoT Workshop - Phase 3 ===");
  Serial.println("Initializing multi-sensor dashboard...");
  
  // Initialize LED pins as outputs (from Phase 1)
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  
  // Initialize ultrasonic sensor pins
  pinMode(ULTRASONIC_TRIGGER_PIN, OUTPUT);
  pinMode(ULTRASONIC_ECHO_PIN, INPUT);
  
  // Turn off both LEDs initially
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, LOW);
  
  Serial.println("LED and sensor pins initialized successfully");
  
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
}

void loop() {
  // Handle startup sequence first
  if (!startupComplete) {
    blinkStartup();
  } else {
    // Main sensor dashboard operation
    handleSensorDashboard();
  }
  
  // Handle LCD startup sequence
  if (lcdInitialized && !lcdStartupComplete) {
    handleLCDStartup();
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
  lcd.print("Sensor Dashboard");
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
