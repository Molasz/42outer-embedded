#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	int	t = 0;

	DDRB |= (1 << PB0);
	DDRB |= (1 << PB1);
	DDRB |= (1 << PB2);
	DDRB |= (1 << PB3);
	DDRD &= ~(1 << PD2);
	DDRD &= ~(1 << PD3);
	DDRD &= ~(1 << PD4);

	
	PORTD |= (1 << PD2);
	PORTD |= (1 << PD3);
	PORTD |= (1 << PD4);

	while (1)
    {
		if (!(PIND & (1 << PD2)))
		{
			t++;
			_delay_ms(200);
			while (!(PIND & (1 << PD2)));
		}
        if (!(PIND & (1 << PD3)))
        {
			t = 0;
			_delay_ms(200);
			while (!(PIND & (1 << PD3)));
		}
        if (!(PIND & (1 << PD4)))
		{
			t--;
			_delay_ms(200);
			while (!(PIND & (1 << PD4)));
		}

        PORTB = (PORTB & 0xF0) | (t & 0x0F);
    }
	return (0);
}
