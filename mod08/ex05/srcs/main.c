/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:42:52 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "adc.h"
#include "spi.h"

static void	leds_init(color_t *leds)
{
	int	i;

	for (i = 0; i < 3; i++)
	{
		leds[i].r = 0x00;
		leds[i].g = 0x00;
		leds[i].b = 0x00;
	}
}

static void	btn_delay(uint8_t pin)
{
	_delay_ms(50);
	while (!(PIND & (1 << pin)));
	_delay_ms(50);
}

int	main(void)
{
	color_t	leds[3];
	uint8_t	curr_color = 0, curr_led = 0;

	leds_init(leds);
	spi_init();
	adc_init();

	SREG |= (1 << SREG_I);
	ADCSRA |= (1 << ADSC);		// Starts first ADC conversion
	
	while (1)
	{
		if (!(PIND & (1 << PIND2)))
		{
			if (curr_color < 2) curr_color++;
			else curr_color = 0;
			btn_delay(PIN2);
		}

		if (!(PIND & (1 << PIND4)))
		{
			if (curr_led < 2) curr_led++;
			else curr_led = 0;
			curr_color = 0;
			btn_delay(PIN4);
		}

		if (GPIOR1 & (1 << RV1_FLAG))
		{
			leds[curr_led].raw[curr_color] = GPIOR0;
			update_leds(leds);
			GPIOR1 &= ~(1 << RV1_FLAG);
		}
	}

	return (0);
}
