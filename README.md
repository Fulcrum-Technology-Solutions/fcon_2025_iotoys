# Arduino IoT Workshop - Interactive Gaming and Sensor System

**FCON 2025 Workshop Materials**

Progressive 3-phase Arduino workshop teaching IoT fundamentals through hands-on sensor and control systems. Build from basic LED control to a complete car-like proximity alarm with distance display.

## Workshop Overview

Participants build an interactive sensor system across three progressive phases, with each phase introducing new concepts while building on previous work. All components remain connected throughout the workshop, demonstrating how complex systems grow from simple foundations.

### Learning Outcomes
- Digital I/O control (input/output)
- Non-blocking timing techniques
- Button debouncing and state management
- Ultrasonic distance sensing
- Multi-digit display multiplexing
- Shift register control
- Real-time sensor feedback systems

## Phase Breakdown

### Phase 0: Power Test and Setup
**Time: 10 minutes**
- Validate Arduino and breadboard power connections
- Verify serial communication
- Confirm IDE setup

[📁 Phase 0 Details →](phase0-power-test/)

---

### Phase 1: LED Blink System
**Time: 15 minutes**
- Control a single LED with timed blinking
- Learn non-blocking timing with `millis()`
- Master digital output basics

**Components:** Red LED  
**New Concepts:** `digitalWrite()`, `millis()`, non-blocking loops

[📁 Phase 1 Details →](phase1-led-blink/)

---

### Phase 2: Button + Buzzer Control
**Time: 20 minutes**
- Add button input with pull-up resistors
- Toggle LED state on button press
- Trigger buzzer beeps with timed control

**Components:** Active buzzer, push button  
**New Concepts:** `digitalRead()`, `INPUT_PULLUP`, state toggling, button debouncing

[📁 Phase 2 Details →](phase2-button-buzzer/)

---

### Phase 3: Proximity Alarm System
**Time: 30-40 minutes**
- Measure distance with ultrasonic sensor
- Display distance on 4-digit 7-segment display
- Implement car-like backup alarm with variable beep rates
- Control LED based on proximity threshold
- Toggle alarm mute with button

**Components:** HC-SR04 ultrasonic sensor, 4-digit 7-segment display with shift registers  
**New Concepts:** `pulseIn()`, shift registers, display multiplexing, distance calculation, dynamic timing

[📁 Phase 3 Details →](phase3-proximity-alarm/)

---

## Hardware Requirements

### Complete Component List
- 1× Arduino Uno R3
- 1× Breadboard (60+ columns)
- 1× Breadboard power supply module (5V/3.3V)
- 1× USB cable (Arduino to computer)
- 1× Red LED (5mm)
- 1× Active buzzer
- 1× Push button (momentary switch)
- 1× HC-SR04 ultrasonic distance sensor
- 1× 4-digit 7-segment display (common cathode with shift register)
- 8× 220Ω resistors (for 7-segment display segments)
- 15-20× Jumper wires (mixed male-male)
- Power adapter for breadboard module (7-10V DC)

### Tools Needed
- Computer with Arduino IDE installed
- USB port available
- (Optional) Multimeter for troubleshooting

## Software Requirements

### Arduino IDE Setup
1. Download and install [Arduino IDE](https://www.arduino.cc/en/software) (v1.8.19+ or v2.x)
2. Connect Arduino Uno via USB
3. Select **Board**: Tools → Board → Arduino Uno
4. Select **Port**: Tools → Port → (your Arduino's COM/serial port)
5. Test connection: File → Examples → 01.Basics → Blink

### No External Libraries Required
All code uses built-in Arduino functions only - no library installation needed.

## Getting Started

### Quick Start Guide
1. **Hardware Setup** (Phase 0)
   - Connect breadboard power supply
   - Wire Arduino 5V and GND to breadboard rails
   - Upload and verify power test code

2. **Progressive Building** (Phases 1-3)
   - Complete phases in order
   - Keep previous components connected
   - Test each phase before moving forward

3. **Troubleshooting**
   - Use Serial Monitor (9600 baud) for debugging
   - Verify all connections match wiring diagrams
   - Check [Troubleshooting Guide](docs/troubleshooting.md)

### File Structure
```
fcon_2025_iotoys/
├── README.md                    # This file
├── CLAUDE.md                    # Detailed requirements document
├── phase0-power-test/           # Phase 0: Power validation
│   ├── README.md
│   ├── phase0_power_test.ino
│   └── IoToys-Phase0-Diagram.png
├── phase1-led-blink/            # Phase 1: LED control
│   ├── README.md
│   ├── phase1_led_blink.ino
│   └── diagram_phase1_led_blink.png
├── phase2-button-buzzer/        # Phase 2: Button + buzzer
│   ├── README.md
│   ├── phase2_button_buzzer.ino
│   └── diagram_phase2_button_buzzer.png
├── phase3-proximity-alarm/      # Phase 3: Complete system
│   ├── README.md
│   ├── phase3_proximity_alarm.ino
│   └── diagram_phase3_proximity_alarm.png
└── docs/                        # Reference documentation
    ├── component-guide.md       # Component specs and pinouts
    ├── troubleshooting.md       # Common issues and solutions
    └── wiring-diagrams/         # Consolidated wiring diagrams
```

## Documentation

### Reference Materials
- **[Component Guide](docs/component-guide.md)** - Detailed component specifications, pinouts, and ratings
- **[Troubleshooting Guide](docs/troubleshooting.md)** - Common issues and solutions for each phase
- **[Wiring Diagrams](docs/wiring-diagrams/)** - Consolidated wiring diagrams (PNG/SVG formats)

### Each Phase Includes
- Complete Arduino sketch (.ino file)
- Phase-specific README with instructions
- Wiring diagram with pin assignments
- Expected behavior description
- Troubleshooting section

## Workshop Facilitation Tips

### Timing Recommendations
- **Total Workshop**: 90-120 minutes including breaks
- **Phase 0**: 10 min (setup validation)
- **Phase 1**: 15 min (basic output)
- **Phase 2**: 20 min (input/output)
- **Phase 3**: 40 min (complete system)
- **Buffer**: 15-30 min (troubleshooting, Q&A)

### Teaching Notes
1. **Test kits beforehand** - Verify all components work
2. **Pre-stage code** - Have .ino files ready on shared drive/GitHub
3. **Serial Monitor** - Emphasize its use for debugging
4. **Pair programming** - Consider 2 participants per kit if limited hardware
5. **Incremental testing** - Don't move forward until current phase works
6. **Common issues** - Review troubleshooting guide before workshop

### Safety Reminders
- Disconnect power when making connections
- Verify polarity for LEDs, buzzers, and sensors
- No resistor for LED (simplified for workshop) - note this omission
- Avoid short circuits between power rails

## Key Concepts Covered

### Arduino Fundamentals
- `setup()` and `loop()` structure
- Serial communication debugging
- Pin modes (INPUT, OUTPUT, INPUT_PULLUP)
- Digital I/O operations

### Timing Techniques
- Non-blocking timing with `millis()`
- Avoiding `delay()` in complex systems
- Dynamic interval calculations
- Display multiplexing timing

### Hardware Control
- LED current control (direct connection)
- Button debouncing strategies
- Ultrasonic sensor pulse timing
- Shift register control for displays
- Active vs passive components

### System Design
- Progressive complexity building
- State management
- Multi-component coordination
- Real-time sensor feedback
- User interface design (button, display, audio)

## Extensions and Next Steps

### After Workshop Ideas
- Add multi-colored LEDs for distance zones (green/yellow/red)
- Implement different alarm sounds or melodies
- Display distance in inches or feet
- Add min/max distance tracking
- Create parking assist game modes
- Integrate temperature/humidity sensors
- Build a smart doorbell system
- Create a security alarm with motion detection

### Additional Learning Resources
- [Arduino Official Documentation](https://www.arduino.cc/reference/en/)
- [HC-SR04 Sensor Guide](https://www.arduino.cc/en/Tutorial/BuiltInExamples/Ping)
- [Shift Register Tutorial](https://www.arduino.cc/en/Tutorial/Foundations/ShiftOut)

## License and Attribution

Workshop materials developed for FCON 2025. Free to use and modify for educational purposes.

## Support and Questions

- Check phase-specific READMEs for detailed instructions
- Review [troubleshooting guide](docs/troubleshooting.md) for common issues
- Use Serial Monitor (9600 baud) for real-time debugging
- Verify wiring against provided diagrams

---

**Happy Making! 🚀**

*Build → Test → Debug → Learn → Repeat*
