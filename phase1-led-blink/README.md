# Phase 1: LED Blink System

## Overview
Learn basic digital output control by blinking an LED with non-blocking timing.

## Components Required
- Arduino Uno R3 (with power from Phase 0)
- Green LED
- 220Ω resistor
- Jumper wires

## Pin Assignments
```
Pin 9: Green LED (via 220Ω resistor to GND)
```

## Wiring Instructions
1. Insert LED into breadboard
2. Connect LED long leg (anode) to Pin 9 via jumper
3. Connect LED short leg (cathode) to 220Ω resistor
4. Connect resistor to breadboard ground rail

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
- Too dim/bright? Verify 220Ω resistor value
- Not blinking? Check Pin 9 connection
- No serial output? Verify baud rate is 9600

## Key Concepts
- Digital output with `digitalWrite()`
- Non-blocking timing with `millis()`
- Serial debugging
- LED current limiting with resistors

## Next Phase
**Phase 2**: Random Generator - Replace LED with 7-segment display and add button input

