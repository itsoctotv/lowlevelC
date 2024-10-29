#define F_CPU	8000000UL // 8 MHz clock speed 
#include <avr/io.h>       // Defines macros for register addresses
#include <util/delay.h>    // Defines _delay_ms() for delay


#define LED_PIN PB3       // Define the LED pin as PB0

void main(void) {
    // Set PB0 as an output
    DDRB |= (1 << LED_PIN);     // DDRB is the Data Direction Register for Port B
    
    // Main loop
    while (1) {
        // Turn LED on
        PORTB |= (1 << LED_PIN);  // Set PB0 high
        
        _delay_ms(1000);          // Delay for 1000 milliseconds

        // Turn LED off
        PORTB &= ~(1 << LED_PIN); // Set PB0 low
        
        _delay_ms(1000);          // Delay for 1000 milliseconds
    }
}
