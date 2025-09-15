# Arduino IoT Workshop - Code Requirements Document

## Project Overview
3-phase Arduino workshop building an Interactive Gaming and Sensor System. Each phase has standalone code that participants will download and upload to their Arduino Uno.

## File/Folder Hierarchy
```
fcon_2025_iotoys/
├── README.md
├── docs/
│   ├── wiring-diagrams/
│   │   ├── phase1-led-wiring.png
│   │   ├── phase2-7segment-button-wiring.png
│   │   └── phase3-proximity-alarm-wiring.png
│   ├── troubleshooting.md
│   └── component-guide.md
├── phase1-led-blink/
│   ├── phase1_led_blink.ino
│   ├── README.md
│   └── wiring-diagram.txt
├── phase2-random-generator/
│   ├── phase2_random_generator.ino
│   ├── README.md
│   └── wiring-diagram.txt
├── phase3-proximity-alarm/
│   ├── phase3_proximity_alarm.ino
│   ├── README.md
│   └── wiring-diagram.txt
└── resources/
    ├── component-datasheets/
    │   ├── HC-SR04-datasheet.pdf
    │   ├── 7segment-display-pinout.pdf
    │   └── arduino-uno-pinout.pdf
    └── workshop-presentation/
        ├── workshop-slides.pdf
        └── component-overview.pdf
```

### File Descriptions:
- **Main README.md**: Workshop overview, prerequisites, and getting started guide
- **Phase folders**: Each contains standalone Arduino code (.ino file) with phase-specific documentation
- **docs/**: Comprehensive documentation including wiring diagrams and troubleshooting guides
- **resources/**: Reference materials including component datasheets and presentation slides
- **wiring-diagram.txt**: ASCII art wiring diagrams in each phase folder for offline reference

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
  - Green LED (Phase 1 only) - 220Ω resistor
  - 1-digit 7-segment display (common cathode)
  - Active buzzer
- **Connectivity**: Direct Arduino connections, breadboard for power distribution

## Pin Assignments
```
// Power Connections
Arduino 5V  ----->  Breadboard Top Rail (Positive/Red Side)
Arduino GND ----->  Breadboard Bottom Rail (Negative/Blue Side)

// Phase 1: LED Control
Pin 9: Green LED (via 220Ω resistor to ground rail)

// Phase 2-3: 7-Segment Display (Common Cathode)
Pin 2: 7-Segment A (top)
Pin 3: 7-Segment B (top right)
Pin 4: 7-Segment C (bottom right)
Pin 5: 7-Segment D (bottom)
Pin 6: 7-Segment E (bottom left)
Pin 7: 7-Segment F (top left)
Pin 8: 7-Segment G (middle)

// Phase 2-3: Input/Output
Pin 10: Push Button (with internal pull-up resistor)

// Phase 3: Ultrasonic Sensor and Buzzer
Pin 11: Ultrasonic Trigger (HC-SR04)
Pin 12: Ultrasonic Echo (HC-SR04)
Pin 13: Active Buzzer

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
- Initialize green LED on Pin 9
- Connect LED via 220Ω resistor to breadboard ground rail
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

## Phase 2: Random Number Generator
**File**: `phase2_random_generator.ino`

### Requirements:
- Remove all LED functionality from Phase 1
- Connect 1-digit 7-segment display to pins 2-8 (segments A-G)
- Connect push button to pin 10 (use internal pull-up resistor)
- Power all components from breadboard rails (5V and ground)
- Implement random number generator functionality:
  - Boot sequence: Display 0→1→2→3→4→5→6→7→8→9 (500ms each)
  - After boot sequence, show blank display
  - Button press generates and displays random number (0-9)
  - Number stays displayed until next button press
- Enhanced serial output showing button presses and generated numbers

### Boot Sequence:
- **Startup**: Display digits 0-9 sequentially, 500ms each
- **Ready State**: Blank display after boot sequence
- **Button Press**: Immediately show new random number (0-9)
- **Serial Output**: Display generated numbers and button press events

### Functions Required:
```cpp
void setup()
void loop()
void bootSequence()
void readButton()
void generateRandomNumber()
void displayNumber(int number)
void clearDisplay()
```

## Phase 3: Proximity Alarm System
**File**: `phase3_proximity_alarm.ino`

### Requirements:
- Build on Phase 2 foundation (7-segment display and button established)
- Connect HC-SR04 ultrasonic sensor to pins 11 (trigger) and 12 (echo)
- Connect active buzzer to pin 13
- Power all components from breadboard rails (5V and ground)
- Implement proximity alarm functionality:
  - Continuously measure distance every 200ms
  - Display distance 0-9cm on 7-segment display
  - Show blank display if distance >9cm
  - Buzzer alarm when distance ≤2cm with car-like beeping pattern
  - Faster beeping as distance approaches 0cm
  - Button toggles buzzer mute/unmute (distance still displays)
- Enhanced serial output showing distance readings, alarm status, and mute state

### Proximity Alarm Features:
- **Distance Range**: 0-9cm displayed on 7-segment, blank if >9cm
- **Update Rate**: Distance measured every 200ms
- **Alarm Threshold**: Buzzer activates when ≤2cm
- **Beep Pattern**: Car reversing alarm style - faster beeping as distance decreases
- **Beep Timing**: 
  - 2cm: beep every 800ms
  - 1cm: beep every 400ms  
  - 0cm: beep every 200ms
- **Mute Function**: Button toggles buzzer on/off, distance display unaffected
- **Serial Output**: Distance readings, alarm status, mute state

### Functions Required:
```cpp
void setup()
void loop()
void measureDistance()
void updateDisplay()
void handleBuzzerAlarm()
void readButton()
void toggleMute()
void calculateBeepInterval(int distance)
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

### 7-Segment Display Number Patterns:
```cpp
// Define digit patterns for 7-segment display (segments A-G)
// Common cathode configuration
// Segment mapping: A=Pin2, B=Pin3, C=Pin4, D=Pin5, E=Pin6, F=Pin7, G=Pin8
// Pattern arrays for digits 0-9
// Clear display function for blank state
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
- **7-Segment Display**: Connect common cathode to ground, segments A-G to pins 2-8 via 220Ω resistors
- **Push Button**: Connect between pin 10 and ground, use INPUT_PULLUP mode
- **Ultrasonic Sensor**: VCC to 5V, GND to ground, Trig to pin 11, Echo to pin 12
- **Active Buzzer**: Positive to pin 13, negative to ground
- **LED (Phase 1 only)**: Connect via 220Ω resistor to ground, positive lead to pin 9
- **Power Distribution**: Use breadboard power rails for clean power distribution (5V top rail, 3.3V bottom rail, both with GND)
