# Arduino IoT Workshop - Code Requirements Document

## Project Overview
Multi-phase Arduino workshop building an Environmental Monitoring Station with Security Access. Each phase has standalone code that participants will download and upload to their Arduino Uno.

## Hardware Configuration
- **Microcontroller**: Arduino Uno R3
- **Shield**: Prototype expansion board with breadboard
- **Power**: 9V battery via power supply module
- **Display**: LCD1602 (16x2 character display with I2C backpack)
- **Input Devices**: 
  - 8-pin 4x4 membrane keypad matrix
  - Analog joystick module
- **Sensors**:
  - HC-SR04 ultrasonic distance sensor
  - DHT11 temperature and humidity sensor
  - Water/moisture sensor (analog)
- **Output Devices**:
  - Green LED (success/positive feedback)
  - Red LED (error/negative feedback)
  - 4-digit 7-segment display with 74HC595 shift register
  - 28BYJ-48 stepper motor with ULN2003 driver board
- **Connectivity**: Standard jumper wires and breadboard connections

## Pin Assignments
```
// Digital Pins
Pin 2:  Ultrasonic Trigger
Pin 3:  Ultrasonic Echo  
Pin 4:  DHT11 Data
Pin 5:  Green LED
Pin 6:  Red LED
Pin 7:  Stepper Motor IN1
Pin 8:  Stepper Motor IN2
Pin 9:  Stepper Motor IN3
Pin 10: Stepper Motor IN4
Pin 11: 7-Segment Data (74HC595)
Pin 12: 7-Segment Clock (74HC595)
Pin 13: 7-Segment Latch (74HC595)

// Analog Pins
A0: Water Sensor
A1: Joystick X-axis
A2: Joystick Y-axis
A3: Joystick Button

// I2C (for LCD)
A4: SDA
A5: SCL

// 8-Pin Keypad Matrix (4x4 membrane)
Pin 14: Keypad Row 1
Pin 15: Keypad Row 2
Pin 16: Keypad Row 3
Pin 17: Keypad Row 4
Pin 18: Keypad Col 1
Pin 19: Keypad Col 2
Pin 20: Keypad Col 3
Pin 21: Keypad Col 4
```

## Phase 1: Basic Setup and LED Control
**File**: `phase1_basic_leds.ino`

### Requirements:
- Initialize green and red LEDs on specified pins
- Create startup sequence: both LEDs blink 3 times simultaneously
- Implement alternating blink pattern: green 500ms, red 500ms, repeat
- Include clear serial output for debugging
- Use non-blocking delay approach for future expansion

### Functions Required:
```cpp
void setup()
void loop()
void blinkStartup()
void alternatingBlink()
```

## Phase 2: LCD Display Integration
**File**: `phase2_lcd_display.ino`

### Requirements:
- Include LCD I2C library support (LiquidCrystal_I2C)
- Initialize 16x2 LCD on I2C address 0x27
- Display workshop title "ENV MONITOR v1.0" on line 1
- Display "Initializing..." on line 2 for 3 seconds
- Clear screen and show "System Ready" with current timestamp
- Maintain LED functionality from Phase 1
- Handle LCD initialization errors gracefully

### Functions Required:
```cpp
void setup()
void loop()
void initializeLCD()
void displayStartupMessage()
void displaySystemReady()
```

## Phase 3: Multi-Sensor Data Collection
**File**: `phase3_sensor_dashboard.ino`

### Requirements:
- Integrate all sensors: ultrasonic, DHT11, water sensor
- Display sensor readings on LCD in rotating fashion (3-second intervals)
- Screen 1: "Temp: XX.X°C" / "Humidity: XX.X%"
- Screen 2: "Distance: XXX cm" / "Status: CLEAR/DETECTED"
- Screen 3: "Water Level: XXX" / "Status: DRY/WET"
- LED indicators: Green when all sensors normal, Red when any alert condition
- Serial output of all sensor values for debugging
- Handle sensor read failures and display error messages

### Alert Conditions:
- Distance < 20cm (proximity detection)
- Water sensor > 500 (water detected)
- Temperature > 30°C or < 10°C
- Humidity > 80%

### Functions Required:
```cpp
void setup()
void loop()
void readAllSensors()
void updateLCDDisplay()
void checkAlertConditions()
void updateStatusLEDs()
```

## Phase 4: Security Keypad Integration
**File**: `phase4_security_system.ino`

### Requirements:
- Add 4x4 membrane keypad support
- Default passcode: "1234*" (* confirms entry)
- System starts in LOCKED mode - sensors hidden
- LCD shows "ENTER PASSCODE:" and cursor
- Display entered digits as asterisks for security
- Correct passcode: Green LED, "ACCESS GRANTED", show sensor dashboard
- Wrong passcode: Red LED blink 3 times, "ACCESS DENIED", clear input
- Auto-clear partial entry after 10 seconds of inactivity
- '#' key clears current entry
- Return to locked mode after 60 seconds of inactivity

### Security Features:
- Maximum 3 failed attempts before 30-second lockout
- During lockout: Red LED stays on, LCD shows "SYSTEM LOCKED"
- Successful entry resets failed attempt counter

### Functions Required:
```cpp
void setup()
void loop()
void handleKeypadInput()
void checkPasscode()
void grantAccess()
void denyAccess()
void lockoutSystem()
void clearEntry()
```

## Phase 5: Stepper Motor Automation
**File**: `phase5_complete_system.ino`

### Requirements:
- Include all previous functionality
- Add stepper motor control (28BYJ-48 with ULN2003)
- After successful passcode entry, stepper motor activates
- Motor rotates 90 degrees clockwise every 2 seconds while in monitoring mode
- Use smooth stepping motion (not jerky)
- Add motor status to display rotation: "Motor: Active/Stopped"
- Joystick can manually control motor when system unlocked:
  - Left/Right: Manual rotation control
  - Button press: Toggle auto/manual mode

### Motor Control:
- Smooth acceleration/deceleration
- 4096 steps per full revolution (90° = 1024 steps)
- Speed: ~500 steps per second for smooth operation
- Auto mode: 2-second intervals between 90° rotations
- Manual mode: Real-time joystick control

### Functions Required:
```cpp
void setup()
void loop()
void controlStepperMotor()
void autoMotorMovement()
void manualMotorControl()
void readJoystick()
void stepMotor(int steps, int direction)
```

## Phase 6: Advanced Features (Bonus)
**File**: `phase6_advanced_system.ino`

### Requirements:
- Add 4-digit 7-segment display showing system uptime
- Implement data logging to EEPROM (last 10 sensor readings)
- Add system configuration menu accessed via special code "9999*"
- Configuration options:
  - Change passcode
  - Adjust sensor thresholds
  - Set motor rotation interval
  - Reset system to defaults
- Enhanced joystick navigation in menus
- Save/load settings from EEPROM

### Functions Required:
```cpp
void setup()
void loop()
void updateSevenSegment()
void configurationMenu()
void saveSettings()
void loadSettings()
void resetToDefaults()
```

## Code Structure Requirements

### All Phases Must Include:
1. **Header comments** with phase description and pin assignments
2. **Library includes** with version compatibility notes
3. **Global variable definitions** with clear naming conventions
4. **Setup function** with proper initialization order
5. **Main loop** with non-blocking timing
6. **Error handling** for sensor failures and hardware issues
7. **Serial debugging** output for troubleshooting
8. **Code comments** explaining each major section

### Coding Standards:
- Use consistent indentation (2 spaces)
- Meaningful variable names (camelCase)
- Function names that describe their purpose
- Constants defined with #define or const
- Avoid blocking delays in main loop
- Include timeout handling for all sensor reads
- Graceful degradation when hardware fails

### Libraries Required:
```cpp
#include <LiquidCrystal_I2C.h>  // LCD display (latest compatible version)
#include <DHT.h>                // Temperature/humidity sensor (latest compatible version)
#include <Keypad.h>             // 8-pin 4x4 membrane keypad (latest compatible version)
#include <Stepper.h>            // Stepper motor control (latest compatible version)
#include <EEPROM.h>             // Data storage (Phase 6)
```

### Testing Requirements:
Each phase must be independently testable and include:
- Hardware connection verification
- Sensor calibration routines
- Component failure simulation
- Reset/restart functionality
- Clear error messages for common issues

## Documentation Requirements:
Each code file should include:
- Header comments with phase description and pin assignments
- Wiring diagram ASCII art in comments
- Expected behavior description
- Troubleshooting section
- Known limitations
- Upgrade path to next phase
- Library includes with version compatibility notes

This requirements document provides complete specifications for generating all six phases of the Arduino workshop code, ensuring consistent functionality and educational progression.