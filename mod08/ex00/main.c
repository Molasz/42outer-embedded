/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/28 14:57:55 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

void	spi_send(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & (1 << SPIF)));									// Wait till operation end
}

void	update_color(uint8_t i)
{
	if (i)
		spi_send(0xFF);												// Turn on
	else
		spi_send(0x00);												// Turn off
}

void	update_led(uint8_t l, uint8_t c)
{
	uint8_t	i;

	spi_send(0xE1);													// 111 + 00001
	for (i = 0; i < 3; i++)
		update_color(l && i == c);									// Only 1r led & red
}

int	main()
{
	uint8_t	i;

	DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);	// SS | MOSI | MISO | SCK | Output
	SPCR |= (1 << SPE) | (1 << MSTR);								// Enable SPI | Master

	for (i = 0; i < 4; i++)											// Start frame 0*32
		spi_send(0x00);

	for (i = 0; i < 3; i++)
		update_led(!i, 2);											// 0 Blue | 1 Green | 2 Red

	for (i = 0; i < 4; i++)											// End frame 1*32
		spi_send(0xFF);

	while (1);

	return (0);
}
