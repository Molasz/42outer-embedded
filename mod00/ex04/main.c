#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	int	t = 0, n = 0;

	DDRB |= (1 << PB0);
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
	DDRB |= (1 << PB3);
	DDRD &= ~(1 << PD2);
	DDRD &= ~(1 << PD3);

	while (1)
	{
		if (!n)
		{
			n = 1;
			if (!(PIND & (1 << PD2)) && t < 4)
				t++;
			if (!(PIND & (1 << PD3)) && t > 0)
				t--;
		}

		if (PIND & (1 << PD2) && (PIND & (1 << PD3)) && n)
			n = 0;
	
		switch (t)
		{
			case 0:
				PORTB &= ~(1 << PB0);
				PORTB &= ~(1 << PB1);
				PORTB &= ~(1 << PB2);
				PORTB &= ~(1 << PB3);
				break;
			case 1:
				PORTB |= (1 << PB0);
				PORTB &= ~(1 << PB1);
				PORTB &= ~(1 << PB2);
				PORTB &= ~(1 << PB3);
				break;
			case 2:
				PORTB |= (1 << PB0);
				PORTB |= (1 << PB1);
				PORTB &= ~(1 << PB2);
				PORTB &= ~(1 << PB3);
				break;
			case 3:
				PORTB |= (1 << PB0);
				PORTB |= (1 << PB1);
				PORTB |= (1 << PB2);
				PORTB &= ~(1 << PB3);
				break;
			case 4:
				PORTB |= (1 << PB0);
				PORTB |= (1 << PB1);
				PORTB |= (1 << PB2);
				PORTB |= (1 << PB3);
				break;
		}
	}

	return (0);
}
