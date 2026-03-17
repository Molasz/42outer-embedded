#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	int	t = 0;

	DDRB |= (1 << PB0);
	DDRD &= ~(1 << PD2);

	while (1)
	{
		if (t == 0 && !(PIND & (1 << PD2)))
		{
			t = 1;
			PORTB ^= (1 << PB0);
		}
	
		if (!(PIND & (1 << PD2)) == 0)
			t = 0;
	}

	return (0);
}
