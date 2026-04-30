/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:23:42 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:24:46 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

void	btn_debounce(uint8_t pin)
{
	_delay_ms(20);
	while (!(PIND & (1 << pin)));
	_delay_ms(20);
}

int	main(void)
{
	uint8_t	t = 0;

	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
	DDRD &= ~((1 << DDD2) | (1 << DDD4));

	while (1)
    {
		if (!(PIND & (1 << PIND2)))
		{
			if (t < 15)
				t++;
			btn_debounce(PIND2);
		}
        if (!(PIND & (1 << PIND4)))
		{
			if (t > 0)
				t--;
			btn_debounce(PIND4);
		}

		PORTB = (PORTB & 0xE8) | (t & 0x07) | ((t & 0x08) << 1); // Clear bits 0 1 2 4, assign 0 1 2 | 4
    }
	return (0);
}
