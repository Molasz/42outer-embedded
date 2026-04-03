#include <avr/io.h>

int	main()
{
	DDRB |= (1 << PB0); // Write 1
	DDRD &= ~(1 << PD2); // Read 0
	PORTD |= (1 << PD2); // Pull-up
						 // 13.2.3

	while (1)
	{
		if (PIND & (1 << PD2))
			PORTB &= ~(1 << PB0); // OFF 0
		else
			PORTB |= (1 << PB0); // ON 1
	}

	return (0);
}
