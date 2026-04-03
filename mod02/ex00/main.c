#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init()
{												// 19.10
	UCSR0A |= (1 << U2X0);						// USART speed x2
	UBRR0 = (F_CPU / (8UL * BAUD)) - 1;			// Baud speed
	UCSR0B |= (1 << TXEN0);						// Enable USART
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);	// 8 bits
}

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));			// Wait until USART is available
	UDR0 = c;
}

void timer_init()
{
	TCCR1B |= (1 << WGM12);						// CTC
	TIMSK1 |= (1 << OCIE1A);					// Enable interrupt
	OCR1A = 15624;								// Duty
	TCCR1B |= (1 << CS12) | (1 << CS10);		// Prescalar 1024
}

ISR(TIMER1_COMPA_vect)							// Call on timer done
{
	uart_tx('Z');
}

int	main()
{
	cli();
	uart_init();
	timer_init();
	sei();

	while (1)
	{}

	return (0);
}
