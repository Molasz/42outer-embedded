/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:01 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/13 19:12:54 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

# define USER "molasz-a"
# define PASS "pass"
# define BUFF_SIZE 32

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

int	strcmp(const char *a, const char *b)
{
	int	i = 0;

	while (a[i] && b[i])
	{
		if (a[i] != b[i])
			return (0);
		i++;
	}
	return (a[i] == b[i]);
}

int	validate_str(const char *str, char hide)
{
	char	buf[BUFF_SIZE];
	char	c = 0;
	int		i = 0;

	while (c != '\r')
	{
		c = uart_rx();
		if (c == 127 || c == '\b')
		{
			if (i)
			{
				i--;
				uart_printstr("\b \b");
			}
		}
		else if (c != '\r' && i < BUFF_SIZE - 1)
		{
			buf[i++] = c;
			if (hide)
				c = hide;
			uart_tx(c);
		}
	}
	buf[i] = '\0';
	return (!strcmp(buf, str));
}

void	blink_leds(uint8_t rg, char *str)
{
	int	i;

	i = 0;
	uart_printstr(str);
	while (i < 50)
	{
		PORTB ^= (1 << rg);
		_delay_ms(70);
		i++;
	}

	PORTB &= ~(1 << rg);
	uart_printstr("\r\n\r\n");
}

int	main()
{
	int	r;

	DDRB |= (1 << PB0);

	uart_init();
	while (1)
	{
		r = 0;
		uart_printstr("Enter your login:\r\nusername: ");
		r |= validate_str(USER, 0);
		uart_printstr("\r\npassword: ");
		r |= validate_str(PASS, '*');
		uart_printstr("\r\n");
		if (r)
			blink_leds(PB1, "Bad combination username/password");
		else
			blink_leds(PB0, "Welcome!");
	}
	return (0);
}
