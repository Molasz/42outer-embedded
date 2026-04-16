/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:08 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/16 12:07:10 by molasz-a         ###   ########.fr       */
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
	PORTD &= ~((1 << PORT3) | (1 << PORT5) | (1 << PORT6));	// Off all
	PORTD |= (1 << pin);									// On actual color
}

void	update_rgb()
{
	if (!state)
		set_color(PORT5);									// Red
	else if (state == 1)
		set_color(PORT6);									// Green
	else
		set_color(PORT3);									// Blue

	if (state == 2)
		state = 0;
	else
		state++;
}

void	__vector_11() __attribute__((section(".vector11"), signal, used));

void	__vector_11()
{
	update_rgb();
}

int	main()
{
	DDRD |= (1 << DDD3) | (1 << DDD5) | (1 << DDD6);		// Blue Red Green

	timer_init();
	SREG |= (1 << SREG_I);

	while (1) {}

	return (0);
}
