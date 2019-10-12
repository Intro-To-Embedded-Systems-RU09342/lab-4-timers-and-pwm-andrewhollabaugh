# Button Debounce Blink
Toggles state of LED when button is pressed. Uses software debouncing to ensure the LED toggles exactly once when the button is pressed.

The LED and button pins can be changed with by chaning the #defines which defines generic names for GPIO registers and pins. The debounce threshold is currently at 5ms and can be changed by changing the TCCR0A value. This value can be calculated by dividing the desired threshold in seconds by the CPU speed (currently 1 MHz).

### Differences Between Implementation for each Processor
- The FR6989 needs the line `PM5CTL0 &= ~LOCKLPM5`, which disables the GPIO pins' default high impedance state
- The MSP-EXT430G2ET board (G2553 processor) includes a pullup resistor on the button. The MSP-EXP430FR6989 board does not. Therefore, the internal pullup resistor for the button gpio pin is enable in the FR6989 program.
