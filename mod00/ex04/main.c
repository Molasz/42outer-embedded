/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:23:42 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/15 16:27:40 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	uint8_t	t = 0;

	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);
	DDRD &= ~((1 << DDD2) | (1 << DDD4));

	while (1)
    {
		if (!(PIND & (1 << PIND2)))
		{
			t++;
			_delay_ms(100);
			while (!(PIND & (1 << PIND2)));
			_delay_ms(100);
		}
        if (!(PIND & (1 << PIND4)))
		{
			t--;
			_delay_ms(100);
			while (!(PIND & (1 << PIND4)));
			_delay_ms(100);
		}

		PORTB = (PORTB & 0xE8) | (t & 0x07) | ((t & 0x08) << 1); // Clear bits 0 1 2 4, assign 0 1 2 | 4
    }
	return (0);
}
