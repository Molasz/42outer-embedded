#include <avr/io.h>

int	main()
{
	DDRB	|= (1 << PB1);						// 15.11 Timer/Counter
	TCCR1A	|= (1 << COM1A1) | (1 << WGM11);	// 15.3 1-0
	TCCR1B	|= (1 << WGM12) | (1 << WGM13);		// 15.5 Fast PWM
	ICR1	= 62500 - 1;						// Compare register
	OCR1A	= ICR1 / 10;						// 10% duty
	TCCR1B	|= (1 << CS12);						// Prescaler 256

	while (1) {}

	return (0);
}
