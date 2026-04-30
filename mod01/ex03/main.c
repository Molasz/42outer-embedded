/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:23 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:31:52 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int	main(void)
{
	int			duty = 5;
	uint16_t	pass;

	DDRB	|= (1 << DDB1);
	DDRD	&= ~((1 << DDD2) | (1 << DDD4));

	TCCR1A	|= (1 << COM1A1) | (1 << WGM11);
	TCCR1B	|= (1 << WGM12) | (1 << WGM13);
	ICR1	= 15625 - 1;					// 16M (F_CPU) / 1024 Prescalar
	OCR1A	= ICR1 * duty / 10;
	TCCR1B	|= (1 << CS12) | ( 1 << CS10);	// Prescalar 1024 | Timer starts

	pass = ICR1 / 10;
	while (1)
	{
		if (duty < 10 && !(PIND & (1 << PIND2)))
		{
			duty++;
			_delay_ms(100);
			while (!(PIND & (1 << PIND2)));
			_delay_ms(100);
		}
        if (duty > 1 && !(PIND & (1 << PIND4)))
		{
			duty--;
			_delay_ms(100);
			while (!(PIND & (1 << PIND4)));
			_delay_ms(100);
		}
		OCR1A = pass * duty;				// Updates duty dynamically
	}

	return (0);
}
