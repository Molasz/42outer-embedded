#include <avr/io.h>
#include <util/delay.h>

void	wait_button(int pin)
{
	_delay_ms(200);
	while (!(PIND & (1 << pin)));
}

int	main()
{
	int	duty = 5;

	DDRB	|= (1 << PB1);
	DDRD &= ~((1 << PD2) | (1 << PD3) | (1 << PD4));
	TCCR1A	|= (1 << COM1A1) | (1 << WGM11);
	TCCR1B	|= (1 << WGM12) | (1 << WGM13);
	ICR1	= 15625 - 1;
	OCR1A	= ICR1 * duty / 10;
	TCCR1B	|= (1 << CS12) | ( 1 << CS10);

	while (1)
	{
		if (duty < 10 && !(PIND & (1 << PD2)))
		{
			duty++;
			wait_button(PD2);
		}
        if (!(PIND & (1 << PD3)))
        {
			duty = 5;
			wait_button(PD3);
		}
        if (duty > 1 && !(PIND & (1 << PD4)))
		{
			duty--;
			wait_button(PD4);
		}

		OCR1A = ICR1 * duty / 10;
	}

	return (0);
}
