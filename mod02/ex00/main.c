#include <avr/io.h>
#include <util/delay.h>

void uart_init()
{
	UCSR0A |= (1 << U2X0);
	UBRR0 = (F_CPU / (8UL * BAUD)) - 1;
	UCSR0B |= (1 << TXEN0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void	uart_tc(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

int	main()
{
	uart_init();

	uart_tc('z');

	return (0);
}
