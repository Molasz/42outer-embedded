/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:22 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/15 19:38:44 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

void uart_init()
{
	UCSR0A |= (1 << U2X0);
	UBRR0 = (F_CPU / (8UL * BAUD)) - 1;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);	// Enable RX interruption
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

char	update_case(char c)
{
	if (c >= 'a' && c <= 'z')
		c -= 32;
	else if (c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}

void	__vector_18() __attribute__((section(".vector18"), signal, used));
// Call __vector_18 when interruption 18 RX complete (11.1) is launched

void __vector_18()
{
	uart_tx(update_case(UDR0));								// Reads from UDR0 directly
}

int	main()
{
	uart_init();
	SREG |= (1 << SREG_I);

	while (1) {}

	return (0);
}
