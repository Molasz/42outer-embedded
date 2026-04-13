/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:44 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/13 18:18:56 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	OCR1A = 15624;								// Compare register
	TCCR1B |= (1 << CS12) | (1 << CS10);		// Prescalar 1024
}

ISR(TIMER1_COMPA_vect)							// 11.1 Call on timer match
{
	uart_tx('Z');
}

int	main()
{
	cli();
	uart_init();
	timer_init();
	sei();

	while (1) {}

	return (0);
}
