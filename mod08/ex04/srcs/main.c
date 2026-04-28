/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/28 13:33:34 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "uart.h"
#include "spi.h"
#include "utils.h"

color_t	leds[3];

void	set_rgb(uint8_t r, uint8_t g, uint8_t b)
{
	update_leds(-1, r, g ,b);
}

void	wheel(uint8_t pos)
{
	pos = 255 - pos;
	if (pos < 85)
		set_rgb(255 - pos * 3, 0, pos * 3);
	else if (pos < 170)
	{
		pos = pos - 85;
		set_rgb(0, pos * 3, 255 - pos * 3);
	}
	else
	{
		pos = pos - 170;
		set_rgb(pos * 3, 255 - pos * 3, 0);
	}
}

void	call_wheel(void)
{
	uint8_t	i;
	
	for (i = 1; i > 0; i++)
	{
		wheel(i);
		_delay_ms(8);
	}
	update_leds(0, leds[0].r, leds[0].g, leds[0].b);
}

void	call_hex(char *str)
{
	uint8_t r, g, b, led;

	r = parse_hex(str);
	g = parse_hex(str + 2);
	b = parse_hex(str + 4);
	led = str[7] - '6';

	update_leds(led, r, g , b);
}

void	user_input(void)
{
	char	c, buff[12];
	uint8_t	i = 0;

	uart_tx('#');
	do
	{
		c = uart_rx();
		if (c == '#' && !i)
			continue;
		if (c == 127 && i)
		{
			i--;
			uart_printstr("\b \b");
		}
		if ((c >= 32 && c <= 126) && i < 12)
		{
			uart_tx(c);
			buff[i++] = c;
		}
	}while (c != '\r');
	buff[i] = '\0';
	uart_printstr("\r\n");
	if (!ft_strcmp(buff, "FULLRAINBOW"))
		call_wheel();
	else if (i == 8 && !validate_color(buff))
		call_hex(buff);
	else
		uart_printstr("Bad user input\r\n");
}

void	leds_init(void)
{
	int	i;

	for (i = 0; i < 3; i++)
	{
		leds[i].r = 0x00;
		leds[i].g = 0x00;
		leds[i].b = 0x00;
	}
}

int	main()
{	
	spi_init();
	uart_init();
	leds_init();
	update_leds(0, 0, 0, 0);

	while (1)
		user_input();

	return (0);
}
