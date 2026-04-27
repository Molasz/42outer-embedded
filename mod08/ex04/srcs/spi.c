/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spi.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 20:47:54 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/27 22:12:00 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "spi.h"
#include "uart.h"

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

void	update_led(uint8_t led, uint8_t r, uint8_t g, uint8_t b)
{
	int	i;

	spi_send(0xE1);
	for (i = 0; i < 3; i++)
	{
		if (led)
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
			spi_send(0x00);
	}
}

void	update_leds(int8_t led, uint8_t r, uint8_t g, uint8_t b)
{
	uint8_t	i;

	for (i = 0; i < 4; i++)
		spi_send(0x00);
	for (i = 0; i < 3; i++)
		update_led(led < 0 || led == i, r, g, b);
	for (i = 0; i < 4; i++)
		spi_send(0xFF);
}

