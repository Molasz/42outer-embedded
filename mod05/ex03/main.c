/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/20 12:15:00 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define TS_OFFSET 324
#define TS_GAIN 1

volatile uint8_t	flag = 0;
volatile uint16_t	n = 0;

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

void	uart_putnbr(uint16_t nb)
{
	if (nb >= 10)
		uart_putnbr(nb / 10);
	uart_tx((nb % 10) + '0');
}

void timer_init()
{
	TCCR1B |= (1 << WGM12);
	OCR1A = 312;
	OCR1B = 312;
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void	ADC_vect() __attribute__((signal));

void	ADC_vect()
{
	if (!flag)
	{
		n = ADCL;
		n |= (ADCH << 8);
		flag = 1;
	}
	TIFR1 |= (1 << OCF1B);
}

int	main()
{
	ADMUX |= (1 << REFS0) | (1 << REFS1) | (1 << MUX3);		// 1.1v reference (23.8) | ADC8 Temperature
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRB |= (1 << ADTS2) | (1 << ADTS0);

	uart_init();
	timer_init();
	SREG |= (1 << SREG_I);

	while (1)
	{
		if (flag)
		{
			uart_putnbr((n - TS_OFFSET) / TS_GAIN);			// Convert to celsius
			uart_printstr("\r\n");
			flag = 0;
		}
	}

	return (0);
}
