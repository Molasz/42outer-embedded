#include <avr/io.h>

int	main()
{
	DDRB |= (1 << PB0);
	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);

	while (1)
	{
		if (PIND & (1 << PD2))
			PORTB &= ~(1 << PB0);
		else
			PORTB |= (1 << PB0);
	}

	return (0);
}
