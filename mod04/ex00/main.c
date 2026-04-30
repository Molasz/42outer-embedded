/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 12:57:50 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t	tick;

void	timer_init(void)
{
	tick = 0;
	TCNT0 = 0;
	OCR0A = 155;
	TCCR0A |= (1 << WGM01);
	TCCR0B |= (1 << CS02) | (1 << CS00);
	TIMSK0 |= (1 << OCIE0A);
}

void	timer_stop(void)
{
	TCCR0B = 0;
	TIMSK0 = 0;
}

void TIMER0_COMPA_vect(void) __attribute__((signal));

void TIMER0_COMPA_vect(void)
{
	if (++tick >= 20)
	{
		timer_stop();
		EIFR = (1 << INTF0);
		EIMSK |= (1 << INT0);
	}
}

void	INT0_vect(void) __attribute__((signal));
// Call INT0_vect when interruption INT0 (11.1) is launched

void	INT0_vect(void)
{
	EIMSK  &= ~(1 << INT0);			// Disable INT0
	PORTB ^= (1 << PORTB0);
	timer_init();
	EIFR = (1 << INTF0);			// Clear interrupt flag | Set 1 to INT0 flag and 0 to all others
}

int	main(void)
{
	DDRB |= (1 << DDB0);
	DDRD &= ~(1 << DDD2);			// External interrupts (12.2)
	EICRA |= (1 << ISC01);			// Launch INT0 interrupt when PD2 detects low (12.2.1)
	EIMSK |= (1 << INT0);			// Enable INT0 interrupt (12.2.2)
	SREG |= (1 << SREG_I);			// Enable interruptions globaly

	while (1) {}

	return (0);
}
