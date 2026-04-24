/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/24 21:28:25 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

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
	uint8_t	i = 0;

	spi_send(0xE1);													// 111 + 00001	
	while (i < 3)
	{
		update_color(l && i == c);
		i++;
	}
}

int	main()
{
	uint8_t	i = 0;

	DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);	// SS | MOSI | MISO | SCK | Output
	SPCR |= (1 << SPE) | (1 << MSTR);								// Enable SPI | Master

	spi_send(0x00);													// Start frame
	spi_send(0x00);
	spi_send(0x00);
	spi_send(0x00);

	while (i < 3)
	{
		update_led(!i, 2);											// 0 Blue | 1 Green | 2 Red
		i++;
	}

	spi_send(0xFF);													// End frame
	spi_send(0xFF);
	spi_send(0xFF);
	spi_send(0xFF);

	while (1);

	return (0);
}
