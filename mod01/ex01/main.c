#include <avr/io.h>

int	main()
{
	DDRB |= (1 << PB1);			// 15.11 Timer/Counter
	TCCR1A |= (1 << COM1A0);	// 15.2 0-1
	TCCR1B |= (1 << WGM12);		// FAST WGM 8-bit
	OCR1A = 31250 - 1;			// Compare register
	TCCR1B |= (1 << CS12);		// Prescaler 256

	while (1) {}

	return (0);
}
