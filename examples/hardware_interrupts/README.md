# ESP-IDF interrupts

## High-level overview

1. Define macros for pins.
2. Define an ISR handler to handle the interrupt.
3. Define the FreeRTOS task for toggling the LED, which waits in an infinite whle loop until the ISR handler sends a signal.
4. Create the main function. Inside it,
   - Create a semaphore to signal the task from the ISR
   - Configure pins for LED and button
   - Create the task for toggling the LED
   - Attach the ISR handler to the button pin

## Circuit

- GPIO4 -> push button

If you don't have a push button, connect a wire to GPIO4. Touching GND with the other end of this wire will act like a button press.

## Notes

### `portYIELD_FROM_ISR`

`portYIELD_FROM_ISR()` is essentially just a trigger button that tells the CPU, "Okay, interrupt is done! Now switch to whichever task FreeRTOS marked as highest priority."
