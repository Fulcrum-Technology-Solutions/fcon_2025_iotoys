# Phase 1: LED Blink System

## Overview
Learn basic digital output control by blinking an LED with non-blocking timing.

## Components Required
- Arduino Uno R3 (with power from Phase 0)
- Red LED
- Jumper wires

## Pin Assignments
```
Pin 13: Red LED (direct connection to GND)
```

## Wiring Instructions
1. Insert LED into breadboard
2. Connect LED long leg (anode) to Pin 13 via jumper
3. Connect LED short leg (cathode) directly to breadboard ground rail

## What This Does
- Blinks LED every 500ms (on/off cycle)
- Uses non-blocking `millis()` timing
- Displays LED state changes in Serial Monitor
- Demonstrates basic digital output

## Expected Behavior
- LED blinks continuously: 500ms ON, 500ms OFF
- Serial Monitor shows timestamps and LED states
- No delays blocking the main loop

## Wiring Diagram
See `diagram_phase1_led_blink.png` or `docs/wiring-diagrams/phase1-led-wiring.png`

## Upload Instructions
1. Wire components as shown in diagram
2. Open `phase1_led_blink.ino` in Arduino IDE
3. Upload to Arduino
4. Open Serial Monitor (9600 baud)
5. Observe LED blinking

## Troubleshooting
- LED not lighting? Check polarity (long leg = positive)
- Not blinking? Check Pin 13 connection
- No serial output? Verify baud rate is 9600
- LED always on/off? Check ground connection

## Key Concepts
- Digital output with `digitalWrite()`
- Non-blocking timing with `millis()`
- Serial debugging
- Basic LED control

## Next Phase
**Phase 2**: Button + Buzzer - Add button input and buzzer output with LED toggle

