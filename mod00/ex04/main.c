/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:23:42 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/13 14:28:20 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	uint8_t	t = 0;

	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
	DDRD &= ~((1 << PD2) | (1 << PD4));
	PORTD |= (1 << PD2) | (1 << PD4);

	while (1)
    {
		if (!(PIND & (1 << PD2)))
		{
			t++;
			_delay_ms(200);
			while (!(PIND & (1 << PD2)));
		}
        if (!(PIND & (1 << PD4)))
		{
			t--;
			_delay_ms(200);
			while (!(PIND & (1 << PD4)));
		}

		PORTB = (PORTB & 0xE8) | (t & 0x07) | ((t & 0x08) << 1); // Clear bits 0 1 2 4, assign 0 1 2 | 4
    }
	return (0);
}
