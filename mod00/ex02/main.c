#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	DDRB |= (1 << PB0);
	DDRD &= ~(1 << PD2);

	PORTB |= ~(1 << PB0);
	
	while (1)
	{
		if (PIND & (1 << PD2))
			PORTB &= ~(1 << PB0);
		else
			PORTB |= (1 << PB0);
		_delay_ms(100);
	}

	return (0);
}
