/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:44 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/15 19:27:23 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

void uart_init()
{												// 19.10
	UCSR0A |= (1 << U2X0);						// USART speed x2
	UBRR0 = (F_CPU / (8UL * BAUD)) - 1;			// Baud speed | UBBR0H & UBBR0L 19.10.5
	UCSR0B |= (1 << TXEN0);						// Enable USART
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);	// 8 bits           8
	UCSROC &= ~((1 << UPM00) | (1 << UPM01));	// Parity disabled  N
	UCSR0C &= ~(1 << USBS0);					// 1-bit stop       1
}

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));			// Wait until USART TX is available
	UDR0 = c;									// Send a char
}

int	main()
{
	uart_init();

	while (1)
	{
		uart_tx('Z');
		_delay_ms(1000);
	}

	return (0);
}
