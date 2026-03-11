#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	int	n = 0;

	DDRB |= (1 << PB0);
	DDRD &= ~(1 << PD2);

	PORTB |= ~(1 << PB0);
	
	while (1)
	{
		if (!(PIND & (1 << PD2)))
		{
			if (n)
				n = 0;
			else
				n = 1;
		}
	
		if (n)
			PORTB |= (1 << PB0);
		else
			PORTB &= ~(1 << PB0);
		_delay_ms(100);
	}

	return (0);
}
