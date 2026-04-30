/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:33:16 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init(void)
{
	UCSR0A |= (1 << U2X0);
	UBRR0 = (F_CPU / (8UL * BAUD)) - 1;
	UCSR0B |= (1 << TXEN0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void	uart_printstr(const char *str)
{
	int			i = 0;

	while (str[i])
		uart_tx(str[i++]);
}

void timer_init(void)
{
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);		// Enable timer1 interruption 15.11.8
	OCR1A = 15624 * 2;
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void	__vector_11(void) __attribute__((section(".vector11"), signal, used));
// Call __vector_11 when interruption 11 TIMER1 COMPA (11.1) is launched

void	__vector_11(void)
{
	uart_printstr("Hello world!\r\n");
}

int	main(void)
{
	uart_init();
	timer_init();
	SREG |= (1 << SREG_I);			// Enable interruptions globaly

	while (1) {}

	return (0);
}
