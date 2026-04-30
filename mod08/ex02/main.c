/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:41:49 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

// COLORS
#define RED 0b001
#define GREEN 0b010
#define BLUE 0b100
#define YELLOW 0b011
#define CYAN 0b110
#define MAGENTA 0b101
#define WHITE 0b111

uint8_t	state = 0;

const uint8_t colors[] = { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};

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
		update_color(l && (c & (1 << (2 - i))));
}

int	main(void)
{
	uint8_t	i, j = 0, state = 0;

	DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB4) | (1 << DDB5);
	SPCR |= (1 << SPE) | (1 << MSTR);

	while (1)
	{
		i = 0;
		for (i = 0; i < 4; i++)
			spi_send(0x00);
		for (i = 0; i < 3; i++)
			update_led(i == j, colors[state]);
		for (i = 0; i < 4; i++)
			spi_send(0xFF);

		_delay_ms(250);
		if (j < 3)
			j++;
		else
			j = 0;
		if (state < 6)
			state++;
		else
			state = 0;
	}

	return (0);
}
