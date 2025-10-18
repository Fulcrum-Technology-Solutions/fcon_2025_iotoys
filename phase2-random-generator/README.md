# Phase 2: Random Number Generator

## Overview
Build an interactive random number generator using a 7-segment display and push button. Press the button to generate and display random numbers 0-9.

## Components Required
- Arduino Uno R3 (with power from Phase 0)
- 1-digit 7-segment display (common cathode)
- Push button (momentary switch)
- 7× 220Ω resistors (for display segments)
- Jumper wires

## Pin Assignments
```
7-Segment Display:
Pin 2: Segment A (top)
Pin 3: Segment B (top right)
Pin 4: Segment C (bottom right)
Pin 5: Segment D (bottom)
Pin 6: Segment E (bottom left)
Pin 7: Segment F (top left)
Pin 8: Segment G (middle)

Button:
Pin 10: Push button (INPUT_PULLUP)
```

## Wiring Instructions
1. **7-Segment Display**: Connect segments A-G to pins 2-8 via 220Ω resistors
2. **Common Cathode**: Connect to breadboard ground rail
3. **Button**: One side to Pin 10, other side to ground rail
4. **Power**: All from breadboard rails (no external power needed)

## What This Does
- Boot sequence: Displays 0→9 at startup (500ms each)
- Shows blank display when ready
- Button press generates random number (0-9)
- Number stays displayed until next button press
- Serial output shows all button presses and numbers

## Expected Behavior
1. Power on: See digits 0-9 count up
2. Display goes blank
3. Press button: Random number appears
4. Press again: New random number
5. Serial Monitor logs all events

## Wiring Diagram
See `diagram_phase2_button_buzzer.png` or `docs/wiring-diagrams/phase2-random-generator-wiring.png`

## Upload Instructions
1. Wire all components as shown
2. Open `phase2_random_generator.ino` in Arduino IDE
3. Upload to Arduino
4. Watch boot sequence
5. Press button to test

## Troubleshooting
- Display not lighting? Check common cathode to ground
- Wrong segments? Verify A-G pin connections (2-8)
- Button not working? Check INPUT_PULLUP and ground connection
- Numbers wrong? Review segment mapping in code
- Multiple presses registered? Button bounce is normal

## Key Concepts
- 7-segment display control
- Digital input with pull-up resistors
- Random number generation
- Button debouncing
- Bit patterns for digit display

## Next Phase
**Phase 3**: Proximity Alarm - Add ultrasonic sensor and buzzer for distance-based alerts

