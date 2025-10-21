# Phase 2: Button + Buzzer Control

## Overview
Build interactive button control that toggles an LED and triggers a buzzer beep. Learn digital input with pull-up resistors and coordinated multi-output control.

## Components Required
- Arduino Uno R3 (with Phase 1 components still connected)
- Red LED (from Phase 1 - stays connected)
- Push button (momentary switch)
- Active buzzer
- Jumper wires

## Pin Assignments
```
From Phase 1 (keep connected):
Pin 13: Red LED (direct connection to GND)

New for Phase 2:
Pin 7: Active Buzzer (positive lead)
Pin 8: Push Button (INPUT_PULLUP)
```

## Wiring Instructions
1. **Keep Phase 1 LED connected** (Pin 13)
2. **Active Buzzer**:
   - Positive (+) lead → Pin 7
   - Negative (-) lead → Breadboard ground rail
3. **Push Button**:
   - One side → Pin 8
   - Other side → Breadboard ground rail
   - (Internal pull-up resistor used, no external resistor needed)

## What This Does
- LED starts in OFF state
- Press button: LED toggles on/off + buzzer beeps (200ms)
- LED state persists until next button press
- Each button press triggers single buzzer beep
- Serial Monitor logs all button events and LED states

## Expected Behavior
1. Power on: LED off, buzzer silent
2. Press button: LED turns ON + single beep
3. Press button again: LED turns OFF + single beep
4. LED remains in current state between presses
5. Serial output shows all state changes

## Wiring Diagram
See `diagram_phase2_button_buzzer.png` or `docs/wiring-diagrams/phase2-button-buzzer-wiring.png`

## Upload Instructions
1. Keep Phase 1 wiring intact, add button and buzzer
2. Open `phase2_button_buzzer.ino` in Arduino IDE
3. Upload to Arduino
4. Open Serial Monitor (9600 baud)
5. Press button to test

## Troubleshooting
- Button not responding? Check Pin 8 connection and ground
- LED not toggling? Verify Pin 13 connection still intact
- Buzzer not beeping? Check polarity (+ to Pin 7, - to GND)
- Buzzer too quiet? Verify active buzzer type (not passive)
- Multiple toggles per press? Button bounce is normal behavior

## Key Concepts
- Digital input with `digitalRead()`
- Internal pull-up resistors (`INPUT_PULLUP`)
- Button state detection
- Toggle state management
- Timed buzzer control with `millis()`
- Coordinated multi-component output

## Next Phase
**Phase 3**: Proximity Alarm - Add ultrasonic sensor and 4-digit display for distance measurement

