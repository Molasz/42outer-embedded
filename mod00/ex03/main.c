/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:23:36 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:24:59 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int	main(void)
{
	DDRB |= (1 << DDB0);
	DDRD &= ~(1 << DDD2);

	while (1)
	{
		if (!(PIND & (1 << PIND2)))
		{
			PORTB ^= (1 << PORTB0);			// !PB0
			_delay_ms(20);
			while (!(PIND & (1 << PIND2)));
			_delay_ms(20);
		}
	}

	return (0);
}
