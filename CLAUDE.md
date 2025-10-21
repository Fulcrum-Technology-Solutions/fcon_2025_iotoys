# Arduino IoT Workshop - Code Requirements Document

## Project Overview
3-phase Arduino workshop building an Interactive Gaming and Sensor System. Each phase has standalone code that participants will download and upload to their Arduino Uno.

## File/Folder Hierarchy
```
fcon_2025_iotoys/
├── README.md
├── CLAUDE.md
├── docs/
│   ├── wiring-diagrams/
│   │   ├── phase0-power-wiring.png
│   │   ├── phase0-power-wiring.svg
│   │   ├── phase1-led-wiring.png
│   │   ├── phase1-led-wiring.svg
│   │   ├── phase2-button-buzzer-wiring.png
│   │   ├── phase2-button-buzzer-wiring.svg
│   │   ├── phase3-proximity-alarm-wiring.png
│   │   └── phase3-proximity-alarm-wiring.svg
│   ├── troubleshooting.md
│   └── component-guide.md
├── phase0_power_test/
│   ├── README.md
│   ├── phase0_power_test.ino
│   ├── diagram_phase0_power_test.png
│   └── diagram_phase0_power_test.svg
├── phase1_led_blink/
│   ├── README.md
│   ├── phase1_led_blink.ino
│   ├── diagram_phase1_led_blink.png
│   └── diagram_phase1_led_blink.svg
├── phase2-_utton_buzzer/
│   ├── README.md
│   ├── phase2_button_buzzer.ino
│   ├── diagram_phase2_button_buzzer.png
│   └── diagram_phase2_button_buzzer.svg
├── phase3_proximity_alarm/
│   ├── README.md
│   ├── phase3_proximity_alarm.ino
│   ├── diagram_phase3_proximity_alarm.png
│   └── diagram_phase3_proximity_alarm.svg
└── resources/
    └── workshop-presentation/
        └── placeholder.txt
```

### File Descriptions:
- **Main README.md**: Workshop overview, prerequisites, and getting started guide
- **CLAUDE.md**: This requirements document
- **Phase folders**: Each contains standalone Arduino code (.ino file), README with instructions, and wiring diagrams (PNG/SVG)
  - **phase0-power-test/**: Power validation and initial setup verification
  - **phase1-led-blink/**: LED blink system
  - **phase2-button-buzzer/**: Button and buzzer control with LED toggle
  - **phase3-proximity-alarm/**: Proximity alarm system with ultrasonic sensor and buzzer
  - Each phase README includes: component list, pin assignments, wiring instructions, expected behavior, troubleshooting
- **docs/**: Comprehensive documentation including:
  - **wiring-diagrams/**: Consolidated wiring diagrams for all phases (PNG and SVG formats)
  - **troubleshooting.md**: Common issues and solutions
  - **component-guide.md**: Component reference and specifications
- **resources/**: Reference materials for workshop presentation files

## Hardware Configuration
- **Microcontroller**: Arduino Uno R3
- **Breadboard**: 0-60 columns, A-J rows with power rails
- **Power Module**: 5V/3.3V dual output with jumpers (7V-10V input)
- **Power Rails**: Top rail (5V), Bottom rail (ground)
- **Display**: Serial Monitor (via USB connection)
- **Input Devices**: 
  - Push button (momentary switch)
- **Sensors**:
  - HC-SR04 ultrasonic distance sensor
- **Output Devices**:
  - Red LED (Phase 1-3) - direct connection
  - 4-digit 7-segment display (common cathode) - Phase 3 only
  - Active buzzer
- **Connectivity**: Direct Arduino connections, breadboard for power distribution

## Pin Assignments
```
// Power Connections
Arduino 5V  ----->  Breadboard Top Rail (Positive/Red Side)
Arduino GND ----->  Breadboard Bottom Rail (Negative/Blue Side)

// Phase 1-3: LED Control
Pin 13: Red LED (direct connection to ground rail)

// Phase 3: 7-Segment Display (Common Cathode with Shift Register)
Pin 4: 7-Segment Shift Register Data
Pin 5: 7-Segment Shift Register Latch
Pin 6: 7-Segment Shift Register Clock
Pin 9-12: 7-Segment Common Cathodes (digits 1-4)

// Phase 2-3: Button and Buzzer
Pin 7: Active Buzzer
Pin 8: Push Button (with internal pull-up resistor)

// Phase 3: Ultrasonic Sensor
Pin 2: Ultrasonic Trigger (HC-SR04)
Pin 3: Ultrasonic Echo (HC-SR04)

// Available Pins
A0-A5: Available for future expansion
```

## Connection Strategy
- **Direct Arduino**: All components connect directly to Arduino pins
- **Breadboard**: Power distribution and ground connections only
- **Serial Output**: All information display via USB Serial Monitor
- **Power Distribution**: All components powered from breadboard power rails
- **Ground**: All components grounded via breadboard ground rail

## Phase 1: LED Blink System
**File**: `phase1_led_blink.ino`

### Requirements:
- Initialize Arduino Uno R3 with Serial communication at 9600 baud
- Initialize red LED on Pin 13
- Connect LED with direct connection to breadboard ground rail
- Create simple blink pattern: LED on for 500ms, off for 500ms, continuous
- Include serial output describing LED state changes
- Use non-blocking millis() timing
- Display workshop header and system information in Serial Monitor

### LED Pattern:
- **Continuous**: LED blinks every 500ms (on/off cycle)
- **Serial Output**: Display LED state changes with timestamps

### Functions Required:
```cpp
void setup()
void loop()
void blinkLED()
void updateSerial()
```

## Phase 2: Button and Buzzer Control
**File**: `phase2_button_buzzer.ino`

### Requirements:
- Build on Phase 1 foundation (LED on Pin 13)
- Connect active buzzer to pin 7
- Connect push button to pin 8 (use internal pull-up resistor)
- Power all components from breadboard rails (5V and ground)
- Implement button toggle functionality:
  - LED starts in OFF state
  - Button press toggles LED on/off
  - Each button press triggers buzzer beep (200ms duration)
  - LED state persists until next button press
- Enhanced serial output showing button presses and LED state changes

### Functionality:
- **Initial State**: LED off, buzzer silent
- **Button Press**: Toggle LED state + single buzzer beep
- **LED State**: Remains in current state until next button press
- **Serial Output**: Display button events and LED state

### Functions Required:
```cpp
void setup()
void loop()
void readButton()
void toggleLED()
void triggerBuzzer()
void handleBuzzer()
```

## Phase 3: Proximity Alarm System
**File**: `phase3_proximity_alarm.ino`

### Requirements:
- Build on Phase 2 foundation (LED, button, and buzzer established)
- Add 4-digit 7-segment display with shift register (pins 4-6 for control, pins 9-12 for common cathodes)
- Connect HC-SR04 ultrasonic sensor to pins 2 (trigger) and 3 (echo)
- Power all components from breadboard rails (5V and ground)
- Implement proximity alarm functionality:
  - Continuously measure distance every 100ms
  - Display distance 0-9999cm on 4-digit 7-segment display
  - LED lights up when distance ≤5cm
  - Buzzer alarm when distance ≤10cm with car-like beeping pattern
  - Faster beeping as distance approaches 2cm, solid beep at ≤2cm
  - Button toggles buzzer mute/unmute (distance display and LED unaffected)
- Enhanced serial output showing distance readings, alarm status, and mute state

### Proximity Alarm Features:
- **Distance Range**: 0-9999cm displayed on 4-digit 7-segment display
- **Update Rate**: Distance measured every 100ms
- **LED Threshold**: Lights up when distance ≤5cm
- **Alarm Threshold**: Buzzer activates when ≤10cm
- **Beep Pattern**: Car reversing alarm style - faster beeping as distance decreases
- **Beep Timing**: 
  - 10cm: beep every 800ms
  - 5cm: beep every ~400ms
  - 2cm: beep every 100ms
  - <2cm: solid beep (continuous)
- **Mute Function**: Button toggles buzzer on/off, distance display and LED unaffected
- **Serial Output**: Distance readings, alarm status, mute state, beep interval

### Functions Required:
```cpp
void setup()
void loop()
void measureDistance()
void break_number()
void display_number()
void cathode_high()
void handleCarBackupBuzzer()
void readMuteButton()
void toggleMute()
```

## Code Structure Requirements

### All Phases Must Include:
1. **Header comments** with phase description and pin assignments
2. **Global variable definitions** with clear naming conventions
3. **Setup function** with proper initialization order and serial output
4. **Main loop** with non-blocking millis() timing
5. **Error handling** for hardware failures and invalid readings
6. **Serial debugging** output for troubleshooting and status updates
7. **Code comments** explaining each major section and function

### Coding Standards:
- Use consistent indentation (2 spaces)
- Meaningful variable names (camelCase)
- Function names that describe their purpose
- Constants defined with #define for pin assignments and thresholds
- Avoid blocking delays in main loop (use millis() timing)
- Include timeout handling for sensor reads
- Graceful degradation when hardware components fail

### Built-in Libraries Only:
```cpp
// No external library includes required
// All functionality implemented with standard Arduino functions
// 7-segment display controlled with digitalWrite()
// Timing controlled with millis()
// Random number generation with random()
// Button reading with digitalRead() and INPUT_PULLUP
```

### 7-Segment Display Control (Phase 3):
```cpp
// 4-digit 7-segment display with shift register
// Common cathode configuration
// Shift register control: Data=Pin4, Latch=Pin5, Clock=Pin6
// Common cathodes: Pins 9-12 (digits 1-4)
// Pattern arrays for digits 0-9 using byte notation
// Multiplexing for smooth display updates
```

### Testing Requirements:
Each phase must be independently testable and include:
- Hardware connection verification via serial output
- Component functionality testing on startup
- Clear error messages for connection issues
- Graceful handling of sensor read failures
- Serial confirmation of all major operations

## Documentation Requirements:
Each code file should include:
- **Header block** with phase description, components used, and pin assignments
- **ASCII wiring diagram** in comments showing connections
- **Expected behavior** description for troubleshooting
- **Serial output examples** showing normal operation
- **Common issues** section with solutions
- **Next phase preview** explaining what components will be added

## Hardware Connection Notes:
- **4-Digit 7-Segment Display** (Phase 3): Shift register data/latch/clock to pins 4-6, common cathodes to pins 9-12, segments via 220Ω resistors
- **Push Button**: Connect between pin 8 and ground, use INPUT_PULLUP mode
- **Ultrasonic Sensor** (Phase 3): VCC to 5V, GND to ground, Trig to pin 2, Echo to pin 3
- **Active Buzzer**: Positive to pin 7, negative to ground
- **LED** (Phase 1-3): Direct connection, positive lead to pin 13, negative to ground rail
- **Power Distribution**: Use breadboard power rails for clean power distribution (5V top rail, ground bottom rail)
