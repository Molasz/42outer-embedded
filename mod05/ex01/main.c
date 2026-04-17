/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/17 15:10:10 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const char	*hex = "0123456789abcdef";

volatile uint8_t	state = 0;
char				buff[3] = "  \0";

void uart_init()
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

void timer_init()
{
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 312;
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void	TIMER1_COMPA_vect() __attribute__((signal));

void	TIMER1_COMPA_vect()
{
	ADMUX &= ~((1 << MUX1) | (1 << MUX0));	// Input channel ADC0
	ADCSRA |= (1 << ADSC);
}

void	ADC_vect() __attribute__((signal));

void	ADC_vect()
{
	if (!state)								// Setup next ADC
	{
		ADMUX |= (1 << MUX0);				// Input channel ADC1
	}
	else if (state == 1)
	{
		ADMUX |= (1 << MUX1);				// Input channel ADC2
		ADMUX &= ~(1 << MUX0);
	}

	uart_printstr(int_hex(ADCH));
	if (state < 2)
	{
		uart_printstr(", ");
		ADCSRA |= (1 << ADSC);
		state++;
	}
	else
	{
		uart_printstr("\r\n");
		state = 0;
	}
}

int	main()
{																		// ADC 23.9
	ADMUX |= (1 << REFS0) | (1 << ADLAR);								// 0 - 1 AVCC max reference (23-3) | Align left

	ADCSRA |= (1 << ADEN) | (1 << ADIE);								// Enable ADC & ADC interrupt
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);				// 128 prescaler

	uart_init();
	timer_init();
	SREG |= (1 << SREG_I);

	while (1) {}

	return (0);
}
