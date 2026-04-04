/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:22 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/05 01:27:03 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init()
{
	UCSR0A |= (1 << U2X0);
	UBRR0 = (F_CPU / (8UL * BAUD)) - 1;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

ISR(USART_RX_vect)
{
	char	c;

	c = UDR0;
	if (c >= 'a' && c <= 'z')
		c -= 32;
	else if (c >= 'A' && c <= 'Z')
		c += 32;
	uart_tx(c);
}

int	main()
{
	cli();
	uart_init();
	sei();

	while (1) {}

	return (0);
}
