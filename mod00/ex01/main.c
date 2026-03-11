#include <avr/io.h>

int	main()
{
	DDRB |= (1 << PB0);

	PORTB |= (1 << PB0);

	while (1);

	return (0);
}
