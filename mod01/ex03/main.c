/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:23 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/13 17:43:50 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/delay.h>

int	main()
{
	int			duty = 5;
	uint16_t	pass;

	DDRB	|= (1 << PB1);
	DDRD	&= ~((1 << PD2) | (1 << PD4));
	TCCR1A	|= (1 << COM1A1) | (1 << WGM11);
	TCCR1B	|= (1 << WGM12) | (1 << WGM13);
	ICR1	= 15625 - 1;
	OCR1A	= ICR1 * duty / 10;
	TCCR1B	|= (1 << CS12) | ( 1 << CS10);	// Prescalar 1024

	pass = ICR1 / 10;
	while (1)
	{
		if (duty < 10 && !(PIND & (1 << PD2)))
		{
			duty++;
			_delay_ms(200);
			while (!(PIND & (1 << PD2)));
		}
        if (duty > 1 && !(PIND & (1 << PD4)))
		{
			duty--;
			_delay_ms(200);
			while (!(PIND & (1 << PD4)));
		}
		OCR1A = pass * duty;
	}

	return (0);
}
