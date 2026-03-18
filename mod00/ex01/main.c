#include <avr/io.h>

int	main()
{
	DDRB |= (1 << PB0);
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
	DDRB |= (1 << PB3);

	PORTB |= (1 << PB0);
	PORTB |= (1 << PB1);
	PORTB |= (1 << PB2);
	PORTB |= (1 << PB3);

	while (1);

	return (0);
}
