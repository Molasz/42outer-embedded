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
volatile uint8_t	btn_read = 1;
volatile uint8_t	count[4] = {0, 0, 0, 0};
volatile uint8_t	poten[4] = {0, 0, 0, 0};

void	adc_init(void)
{
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADATE);
}

void	timer_init(void)
{
	TCCR1B |= (1 << WGM12);
	OCR1A = 625;
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
	i2c_write(0x01);
	i2c_write(0);
	i2c_stop();
}

void	i2c_segments_blank(void)
{
	i2c_start();
	i2c_write(0x42);
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
	i2c_write(~(1 << d));
	i2c_write(numbers[n]);
	i2c_stop();

	i2c_start();
	i2c_write(0x42);
	i2c_write(0x02);
	i2c_write(~(1 << d));
	i2c_write(numbers[n]);
	i2c_stop();

	_delay_ms(1);
}

uint8_t	i2c_read_btn(void)
{
	uint8_t	read;

	i2c_start();
	i2c_write(0x40);
	i2c_write(0x00);
	i2c_start();
	i2c_write(0x41);
	read = i2c_read(1);
	i2c_stop();
	return (read);
}

void	update_digits(uint16_t num, volatile uint8_t *digits)
{
	uint8_t		i;
	uint16_t	t;

	for (i = 0, t = num; i < 4; i++)
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

	update_digits(n, count);
}

void	ADC_vect(void) __attribute__((signal));

void	ADC_vect(void)
{
	uint16_t adc;
	adc = ADCL;
	adc |= (ADCH << 8);
	update_digits(adc, poten);
}

void	update_segment(void)
{
	if (btn_read)
	{
		if (count[3] > 0)
			i2c_segments(count[3], 4);
		if (count[3] > 0 || count[2] > 0)
			i2c_segments(count[2], 5);
		if (count[3] > 0 || count[2] > 0 || count[1] > 0)
			i2c_segments(count[1], 6);
		i2c_segments(count[0], 7);
	}
	else
	{
		i2c_segments(poten[3], 4);
		i2c_segments(poten[2], 5);
		i2c_segments(poten[1], 6);
		i2c_segments(poten[0], 7);
	}
}

int	main(void)
{
	uint8_t	read = 0;

	i2c_init();
	i2c_init_expander();
	adc_init();
	timer_init();
	SREG |= (1 << SREG_I);
	ADCSRA |= (1 << ADSC);

	while (1)
	{
		read = i2c_read_btn();
		if (!(1 & read))
		{
			btn_read = !btn_read;
			_delay_ms(50);
			do
			{
				update_segment();
				read = i2c_read_btn();
			}
			while (!(1 & read));
			_delay_ms(50);
		}

		update_segment();
	}

	return (0);
}
