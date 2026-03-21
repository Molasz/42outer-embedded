#include <avr/io.h>

int	main()
{
	DDRB	|= (1 << PB1);
	TCCR1A	|= (1 << COM1A1) | (1 << WGM11);
	TCCR1B	|= (1 << WGM12) | (1 << WGM13);
	ICR1	= 62500 - 1;
	OCR1A	= ICR1 / 10;
	TCCR1B	|= (1 << CS12);

	while (1) {}

	return (0);
}
