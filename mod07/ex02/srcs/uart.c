/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:08:29 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 15:12:42 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "uart.h"

void	uart_init()
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

void	uart_printnbr(uint16_t nb)
{
	if (nb >= 10)
		uart_printnbr(nb / 10);
	uart_tx((nb % 10) + '0');
}

void	uart_printhex(uint8_t n)
{
	uart_tx(HEX[n / 16]);
	uart_tx(HEX[n % 16]);
}

void	uart_printaddr(uint32_t addr)
{
	uint8_t		i = 8;

	while (i > 0)
	{
		uart_tx(HEX[(addr >> ((i - 1) * 4)) & 0x0F]);
		i--;
	}
}

