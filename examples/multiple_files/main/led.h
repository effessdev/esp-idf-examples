#pragma once

// Initialize the LED GPIO. Must be called once before led_toggle().
void led_init(void);

// Toggle the LED state (on -> off, off -> on).
void led_toggle(void);
