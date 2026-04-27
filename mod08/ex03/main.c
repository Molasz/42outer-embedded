/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/27 20:55:05 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t volatile flag = 0;
uint8_t volatile n = 0;

void	adc_init()
{
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADATE);										// Enable auto trigger mode
}

void	spi_send(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
}

void	update_color(uint8_t i)
{
	if (i)
		spi_send(0xFF);
	else
		spi_send(0x00);
}

void	update_led(uint8_t l, uint8_t c)
{
	uint8_t	i;

	spi_send(0xE1);
	for (i = 0; i < 3; i++)
		update_color(l && c == i);
}

void	ADC_vect() __attribute__((signal));

void	ADC_vect()
{
	n = ADCH;
	flag = 1;
}

int	main()
{
	uint8_t	i;
	int8_t	j;

	DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
	SPCR |= (1 << SPE) | (1 << MSTR);

	adc_init();

	SREG |= (1 << SREG_I);
	ADCSRA |= (1 << ADSC);										// Starts first ADC conversion

	while (1)
	{
		if (flag)
		{
			if (n == 255) j = 2;
			else if (n >= 170) j = 1;
			else if (n >= 85) j = 0;
			else j = -1;

			for (i = 0; i < 4; i++)
				spi_send(0x00);
			for (i = 0; i < 3; i++)
				update_led(j >= i, 2);
			for (i = 0; i < 4; i++)
				spi_send(0xFF);
			flag = 0;
		}
	}

	return (0);
}
