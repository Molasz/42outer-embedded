/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/17 12:21:18 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int	state = 0;
volatile int8_t	direction = 0;

void	init_timers()
{
	TCCR0A |= (1 << COM0A1) | (1 << WGM01);
	OCR0A = 78;								// 16M / 1024 / 200 | 200Hz
	TIMSK0 |= (1 << OCIE0A);
	TCCR0B |= (1 << CS02) | (1 << CS00);

	TCCR1A |= (1 << COM1A1) | (1 << WGM10);	// Fast PWM Top 255
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS10);
}

void	TIMER0_COMPA_vect() __attribute__((signal));

void	TIMER0_COMPA_vect()
{
	if ((!state && direction) || (state >= 25500 && !direction))
		direction = !direction;

	if (!direction)
		state += 255;
	else
		state -= 255;
	OCR1A = state / 100;
}

int	main()
{
	DDRB |= (1 << DDB1);

	init_timers();
	SREG |= (1 << SREG_I);

	while (1) {}

	return (0);
}
