#include <avr/io.h>

int	main()
{
	DDRB |= (1 << PB0); // READ / WRITE

	PORTB |= (1 << PB0); // ON / OFF

	while (1);

	return (0);
}
