/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spi.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 20:47:54 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/28 13:30:29 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "spi.h"
#include "main.h"

void	spi_init(void)
{
	DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
	SPCR |= (1 << SPE) | (1 << MSTR);
}

static void	spi_send(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));
}

static void	update_led(color_t *leds, int8_t led)
{
	int	i;

	spi_send(0xE1);
	for (i = 0; i < 3; i++)
	{
		switch (i)
		{
			case 2:
				spi_send(leds[led].r);
				break;
			case 1:
				spi_send(leds[led].g);
				break;
			case 0:
				spi_send(leds[led].b);
				break;
		}
	}
}

void	update_leds(color_t *leds)
{
	uint8_t	i;

	for (i = 0; i < 4; i++)
		spi_send(0x00);
	for (i = 0; i < 3; i++)
		update_led(leds, i);
	for (i = 0; i < 4; i++)
		spi_send(0xFF);
}

