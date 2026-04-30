/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:37:19 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const char	*hex = "0123456789abcdef";

volatile uint8_t	flag = 0;
volatile uint8_t	n = 0;
volatile uint8_t	state = 0;
char				buff[3] = "  \0";

void uart_init(void)
{
	UCSR0A |= (1 << U2X0);
	UBRR0 = (F_CPU / (8UL * BAUD)) - 1;
	UCSR0B |= (1 << TXEN0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
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

char	*int_hex(uint8_t n)
{
	buff[1] = hex[n % 16];
	buff[0] = hex[n / 16];
	return (buff);
}

void timer_init(void)
{
	TCCR1B |= (1 << WGM12);
	OCR1A = 312;
	OCR1B = 312;
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void	ADC_vect(void) __attribute__((signal));

void	ADC_vect(void)
{
	if (!flag)
	{
		n = ADCH;
		flag = 1;
	}
}

int	main(void)
{
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRB |= (1 << ADTS2) | (1 << ADTS0);

	uart_init();
	timer_init();
	SREG |= (1 << SREG_I);

	while (1)
	{
		if (flag)
		{
			uart_printstr(int_hex(n));
			if (state < 2)
			{
				uart_printstr(", ");
				state++;
			}
			else
			{
				uart_printstr("\r\n");
				state = 0;
			}

			ADMUX = (ADMUX & 0xF0) | state;				// (ADMUX & 11110000) | 00000001 (ex 1)
			flag = 0;
			if (state > 0)
				ADCSRA |= (1 << ADSC);
			else
				TIFR1 |= (1 << OCF1B);
		}
	}

	return (0);
}
