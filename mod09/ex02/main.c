/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/29 09:57:38 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

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
	i2c_write(0);		// IO00 - IO07 Output
	i2c_write(0);		// I100 - I207 Output
	i2c_stop();
}

void	i2c_segments(void)
{
	i2c_start();
	i2c_write(0x40);
	i2c_write(0x02);
	i2c_write(0x7F);	// 01111111
	i2c_write(0x5B);	// 01011011
	i2c_stop();
}

int	main()
{
	i2c_init();
	i2c_init_expander();
	i2c_segments();

	while (1);

	return (0);
}
