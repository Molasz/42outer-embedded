#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	DDRB |= (1 << PB0);
	DDRD &= ~(1 << PD2);
	PORTD |= (1 << PD2);

	while (1)
	{
		if (!(PIND & (1 << PD2)))
		{
			PORTB ^= (1 << PB0);
			_delay_ms(200);
			while (!(PIND & (1 << PD2)));
		}
	}

	return (0);
}
