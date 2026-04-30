/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 20:48:39 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:42:25 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"

void	uart_init(void)
{
	UCSR0A |= (1 << U2X0);
	UBRR0 = UART_RATE;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

char	uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
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

