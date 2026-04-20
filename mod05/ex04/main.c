/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/20 12:16:39 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t	n;
volatile uint8_t	flag = 0;

void	rgb_init()
{
	DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);
	TCCR0A |= (1 << WGM00) | (1 << WGM01) | (1 << COM0A1) | (1 << COM0B1);
	TCCR0B |= (1 << CS00);
	TCCR2A |= (1 << WGM20) | (1 << WGM21) | (1 << COM2B1);
	TCCR2B |= (1 << CS20);
}

void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	OCR0A = g;
	OCR0B = r;
	OCR2B = b;
}

void	wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
		set_rgb(255 - pos * 3, 0, pos * 3);
	else if (pos < 170)
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

void	adc_init()
{
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADATE);										// Enable auto trigger mode
	ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0));	// Free running mode
}

void	update_led(uint8_t n, uint8_t lim, uint8_t pin)
{
	if (n >= lim)
		PORTB |= (1 << pin);
	else
		PORTB &= ~(1 << pin);
}

void	update_leds()
{
	update_led(n, 63, PORTB0);
	update_led(n, 127, PORTB1);
	update_led(n, 191, PORTB2);
	update_led(n, 255, PORTB4);

	wheel(n);
}

void	ADC_vect() __attribute__((signal));

void	ADC_vect()
{
	n = ADCH;
	flag = 1;
}

int	main()
{
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);

	rgb_init();
	adc_init();
	
	SREG |= (1 << SREG_I);
	ADCSRA |= (1 << ADSC);										// Starts first ADC conversion

	while (1)
	{
		if (flag)
		{
			update_leds();
			flag = 0;
		}
	}

	return (0);
}
