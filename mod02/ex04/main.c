/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:01 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/05 02:41:53 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

# define USER "molasz-a"
# define PASS "pass"

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

int	validate_str(const char *str, char hide)
{
	char	c = 0;
	int		r = 0;
	int		i = 0;

	while (c != '\r')
	{
		c = uart_rx();
		if (c > 31 && c != 127)
		{
			if (!r && str[i])
			{
				if (str[i])
					r |= str[i] != c;
				else
					r = 1;
			}
			i++;
		}

		if (hide && c > 31 && c != 127)
			uart_tx(hide);
		else
			uart_tx(c);
	}
	uart_printstr("\r\n");
	return (r);
}

int	main()
{
	int	r = 0;

	uart_init();

	while (1)
	{
		uart_printstr("Enter your login:\r\nusername: ");
		r |= validate_str(USER, 0);
		uart_printstr("password: ");
		r |= validate_str(PASS, '*');
		if (r)
			uart_printstr("Bad combination username/password\r\n");
		else
		{
			uart_printstr("Welcome!\r\n");
		}
		uart_printstr("\r\n");
	}

	return (0);
}
