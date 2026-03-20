#include <avr/io.h>

int	main()
{
	DDRB |= (1 << PB1);
	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << WGM12);
	OCR1A = 31250 - 1;
	TCCR1B |= (1 << CS12);

	while (1)
	{
	}

	return (0);
}
