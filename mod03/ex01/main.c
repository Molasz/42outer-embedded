/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:08 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:34:31 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

// COLORS
#define RED (1 << PORTD5)
#define GREEN (1 << PORTD6)
#define BLUE (1 << PORTD3)
#define YELLOW (RED | GREEN)
#define CYAN (GREEN | BLUE)
#define MAGENTA (RED | BLUE)
#define WHITE (RED | GREEN | BLUE)

uint8_t	state = 0;

// Color sequence
const uint8_t colors[] = { RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA, WHITE};

void	timer_init(void)
{
	TCCR1B |= (1 << WGM12);
	TIMSK1 |= (1 << OCIE1A);
	OCR1A = 15624;
	TCCR1B |= (1 << CS12) | (1 << CS10);
}

void	update_rgb(void)
{
	PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD6));
	PORTD |= colors[state];								// On actual color

	if (state == 6)
		state = 0;
	else
		state++;
}

void	__vector_11(void) __attribute__((section(".vector11"), signal, used));

void	__vector_11(void)
{
	update_rgb();
}

int	main(void)
{
	DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);

	timer_init();
	SREG |= (1 << SREG_I);

	while (1) {}

	return (0);
}
