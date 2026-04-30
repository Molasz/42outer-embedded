/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:43:53 by molasz-a         ###   ########.fr       */
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

volatile uint16_t	n = 0;
volatile uint8_t	digits[4] = {0, 0, 0, 0};

void	timer_init(void)
{
	TCCR1B |= (1 << WGM12);
	OCR1A = 62500;
	TCCR1B |= (1 << CS12);
	TIMSK1 |= (1 << OCIE1A);
}

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
	i2c_write(0);
	i2c_write(0);
	i2c_stop();
}

void	i2c_segments_blank(void)
{
	i2c_start();
	i2c_write(0x40);
	i2c_write(0x02);
	i2c_write(0xFF);
	i2c_write(0x00);
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

	_delay_ms(1);
}

void	update_digits(void)
{
	uint8_t		i;
	uint16_t	t;

	for (i = 0, t = n; i < 4; i++)
	{
		digits[i] = t % 10;
		t /= 10;
	}
}

void	TIMER1_COMPA_vect(void) __attribute__((signal));

void	TIMER1_COMPA_vect(void)
{
	if (n < 9999)
		n++;
	else
		n = 0;

	update_digits();
}

void	update_segment(void)
{
	if (digits[3] > 0)
		i2c_segments(digits[3], 4);
	if (digits[3] > 0 || digits[2] > 0)
		i2c_segments(digits[2], 5);
	if (digits[3] > 0 || digits[2] > 0 || digits[1] > 0)
		i2c_segments(digits[1], 6);
	i2c_segments(digits[0], 7);
}

int	main(void)
{
	i2c_init();
	i2c_init_expander();
	timer_init();
	SREG |= (1 << SREG_I);

	while (1)
		update_segment();

	return (0);
}
