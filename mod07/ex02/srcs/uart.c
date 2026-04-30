/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:08:29 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:41:19 by molasz-a         ###   ########.fr       */
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

void	uart_printuint(uint32_t n)
{
	char	buff[11];
	uint8_t	i = 0;

	if (n == 0)
	{
		uart_tx('0');
		return;
	}

	while (n > 0)
	{
		buff[i] = (n % 10) + '0';
		n /= 10;
		i++;
	}

	while (i--)
		uart_tx(buff[i]);
}

void	uart_printint(int32_t n)
{
	if (n < 0)
	{
		uart_tx('-');
		n = -n;
	}
	uart_printuint(n);
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

