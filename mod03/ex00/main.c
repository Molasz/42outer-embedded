/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:08 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/14 12:52:13 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t	state = 0;

void	timer_init()
{
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 15624;
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void set_color(uint8_t pin)
{
	PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD6));	// Off all
	PORTD |= (1 << pin);								// On actual color
}

void	update_rgb()
{
	if (!state)
		set_color(PD5);
	else if (state == 1)
		set_color(PD6);
	else
		set_color(PD3);

	if (state == 2)
		state = 0;
	else
		state++;
}

ISR(TIMER1_COMPA_vect)
{
	update_rgb();
}

int	main()
{
	DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);		// Blue Red Green

	cli();
	timer_init();
	sei();

	while (1) {}

	return (0);
}
