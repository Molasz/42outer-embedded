/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:08 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/14 13:06:01 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

#define RED (1 << PD6)
#define GREEN (1 << PD5)
#define BLUE (1 << PD3)
#define YELLOW (RED | GREEN)
#define CYAN (GREEN | BLUE)
#define MAGENTA (RED | BLUE)
#define WHITE (RED | GREEN | BLUE)

uint8_t	state = 0;

const uint8_t colors[] = { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};

void	timer_init()
{
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 15624;
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void	update_rgb()
{
	PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD6));	// Off all
	PORTD |= colors[state];								// On actual color

	if (state == 6)
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
