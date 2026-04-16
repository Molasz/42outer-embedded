/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/16 15:26:46 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void	init_timers()
{
	TCCR0A |= (1 << COM0A0);
	TCCR0B |= (1 << WGM02);
	OCR0A = 1250;							// 200 times each second
	TIMSK0 |= (1 << OCIE0A);
	TCCR0B |= (1 << CS00) | (1 << CS01);	// Prescaler 64

	TCCR1A |= (1 << COM1A0);
	TCCR1B |= (1 << WGM12);
	OCR1A = 31250 - 1;
	TCCR1B |= (1 << CS12);
}

void	__vector_14() __attribute__((section(".vector14"), signal, used));

void	__vector_14()
{
	// update duty
}

int	main()
{
	DDRB |= (1 << DDB0);
	SREG |= (1 << SREG_I);			// Enable interruptions globaly

	while (1) {}

	return (0);
}
