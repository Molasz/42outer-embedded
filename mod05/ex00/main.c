/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/17 21:23:09 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const char	*hex = "0123456789abcdef";
char		buff[3] = "  \0";

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
	OCR1A = 312;
	OCR1B = 312;
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void	ADC_vect() __attribute__((signal));

void	ADC_vect()
{
	uart_printstr(int_hex(ADCH));
	uart_printstr("\r\n");
	TIFR1 |= (1 << OCF1B);												// Clear timer flag
}

int	main()
{																		// ADC 23.9
	ADMUX &= ~((1 << MUX3) | (1 << MUX2) | (1 << MUX1) | (1 << MUX0));	// Input channel ADC0 (23-4)
	ADMUX |= (1 << REFS0) | (1 << ADLAR);								// 0 - 1 AVCC max reference (23-3) | Align left

	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADATE);					// Enable ADC | ADC interrupt | Enable Auto trigger (23-6)
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);				// 128 prescaler

	ADCSRB |= (1 << ADTS2) | (1 << ADTS0);								// Trigger ADC conversion on timer match

	uart_init();
	timer_init();
	SREG |= (1 << SREG_I);

	while (1) {}

	return (0);
}
