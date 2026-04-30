/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:43:14 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

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

uint8_t	i2c_read(uint8_t last)
{
	if (last)
		TWCR = (1 << TWEN) | (1 << TWINT);
	else
		TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);

	while (!(TWCR & (1 << TWINT)));
	return (TWDR);
}

void	i2c_init_expander(void)
{
	i2c_start();
	i2c_write(0x40);
	i2c_write(0x06);
	i2c_write(0b00000001);		// IO00 - IO06 Output | IO07 Input
	i2c_stop();
}

uint8_t	i2c_read_btn(uint8_t pin)
{
	uint8_t	read;

	i2c_start();
	i2c_write(0x40);
	i2c_write(0x00);			// Port 0 Input mode
	i2c_start();
	i2c_write(0x41);			// I2c expander addr [0100000] + read [1]
	read = i2c_read(1);
	i2c_stop();
	return (read);
}

void	i2c_update_leds(uint8_t n)
{
	i2c_start();
	i2c_write(0x40);
	i2c_write(0x02);
	i2c_write((0xE & ~(n << 1)) | 0xF1);
	i2c_stop();
}

int	main(void)
{
	uint8_t	n = 0, read;

	i2c_init();
	i2c_init_expander();
	i2c_update_leds(n);

	while (1)
	{
		read = i2c_read_btn(0);	// SW3 | IO00
		if (!(1 & read))
		{
			if (n < 7)
				n++;
			else
				n = 0;
			i2c_update_leds(n);
			_delay_ms(50);
			do
				read = i2c_read_btn(0);
			while (!(1 & read));
			_delay_ms(50);
		}
	}

	return (0);
}
