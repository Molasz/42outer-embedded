#include <avr/io.h>

int	main()
{
	volatile long	n = 0;

	DDRB |= (1 << PB0);

	while (1)
	{
		n = 400000;
		while(n-- > 0)
		{
		}

		PORTB ^= (1 << PB0);
	}

	return (0);
}
