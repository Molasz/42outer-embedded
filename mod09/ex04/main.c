/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/29 10:34:16 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

#define S0 0b00111111
#define S1 0b00000110
#define S2 0b01011011
#define S3 0b01001111
#define S4 0b01100110
#define S5 0b01101101
#define S6 0b01111101
#define S7 0b00000111
#define S8 0b01111111
#define S9 0b01101111

const uint8_t numbers[] =  {S0, S1, S2, S3, S4, S5, S6, S7, S8, S9};

void	i2c_init(void)
{
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
	TWBR = 72;
	TWCR |= (1 << TWEN);
}

void	i2c_start(void)
{
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
}

void	i2c_stop(void)
{
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
	while (TWCR & (1 << TWSTO));
}

void	i2c_write(unsigned char data)
{
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
}

void	i2c_init_expander(void)
{
	i2c_start();
	i2c_write(0x40);
	i2c_write(0x06);
	i2c_write(0);				// IO00 - IO07 Output
	i2c_write(0);				// I100 - I207 Output
	i2c_stop();
}

void	i2c_segments_blank(void)
{
	i2c_start();
	i2c_write(0x40);
	i2c_write(0x02);
	i2c_write(0xFF);			// All digits
	i2c_write(0x00);			// All leds off
	i2c_stop();
}

void	i2c_segments(uint8_t n, uint8_t d)
{
	i2c_segments_blank();

	i2c_start();
	i2c_write(0x40);
	i2c_write(0x02);
	i2c_write(~(1 << d));		// Digit
	i2c_write(numbers[n]);		// Number
	i2c_stop();

	_delay_ms(5);
}

int	main()
{
	i2c_init();
	i2c_init_expander();

	while (1)
	{
		i2c_segments(4, 6);
		i2c_segments(2, 7);
	}

	return (0);
}
