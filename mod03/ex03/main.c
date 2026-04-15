/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:08 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/15 14:54:58 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define BUFF_SIZE 8

void	init_rgb()
{
	DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
																			// 13.3 Alt port
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);	// PD5 & PD6
	TCCR0B |= (1 << CS00);													// 14.9

	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);					// PD3
	TCCR2B |= (1 << CS20);													// 17.11
}

void uart_init()
{
	UCSR0A |= (1 << U2X0);
	UBRR0 = (F_CPU / (8UL * BAUD)) - 1;
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
	int	i = 0;

	while (str[i])
		uart_tx(str[i++]);
}

int	validate_hex(char	*hex)
{
	int	i = 1;

	if (hex[0] != '#') return (1);
	while (hex[i])
	{
		if (!((hex[i] >= '0' && hex[i] <= '9') ||
			(hex[i] >= 'A' && hex[i] <= 'F') ||
			(hex[i] >= 'a' && hex[i] <= 'f')))
			return (1);
		i++;
	}
	return (0);
}

uint8_t	char_hex(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (c - 'a' + 10);
}

uint8_t	str_hex(char *hex)
{
	uint8_t	n;

	n = char_hex(hex[0]);
	n += char_hex(hex[1]);
	return (n);
}

void	set_rgb(char *hex)
{
	OCR0B = str_hex(hex);
	OCR0A = str_hex(hex + 2);
	OCR2B = str_hex(hex + 4);
}

void	read_hex()
{
	char	buff[BUFF_SIZE];
	char	c = 0;
	char	i = 0;

	while (c != '\r')
	{
		c = uart_rx();
		uart_tx(c);
		if (c != '\r')
			buff[i++] = c;
	}
	uart_printstr("\r\n");
	buff[i] = '\0';
	if (i < 7 || validate_hex(buff))
		return;
	set_rgb(buff + 1);
}

int	main()
{
	init_rgb();
	set_rgb("000000");
	uart_init();

	while (1)
	{
		read_hex();
	}

	return (0);
}
