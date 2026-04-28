/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spi.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 20:47:54 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/28 15:51:09 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "spi.h"
#include "uart.h"
#include "main.h"

void	spi_init(void)
{
	DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
	SPCR |= (1 << SPE) | (1 << MSTR);
}

void	spi_send(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
}

void	update_led(int8_t led, uint8_t aled, uint8_t r, uint8_t g, uint8_t b)
{
	int	i;

	spi_send(0xE1);
	for (i = 0; i < 3; i++)
	{
		if (led < 0 || led == aled)
		{
			switch (i)
			{
				case 2:
					spi_send(r);
					break;
				case 1:
					spi_send(g);
					break;
				case 0:
					spi_send(b);
					break;
			}
		}
		else
			spi_send(leds[aled].raw[2 - i]);
	}
}

void	update_leds(int8_t led, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t	i;

	for (i = 0; i < 4; i++)
		spi_send(0x00);
	for (i = 0; i < 3; i++)
	{
		update_led(led, i, r, g, b);
		if (led == i)
		{
			leds[i].r = r;
			leds[i].g = g;
			leds[i].b = b;
		}
	}
	for (i = 0; i < 4; i++)
		spi_send(0xFF);
}

