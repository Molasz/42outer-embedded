/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/17 12:16:57 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void	INT0_vect() __attribute__((signal));
// Call INT0_vect when interruption INT0 (11.1) is launched

void	INT0_vect()
{
	PORTB ^= (1 << PORTB0);
	_delay_ms(200);
	EIFR = (1 << INTF0);			// Clear interrupt flag | Set 1 to INT0 flag and 0 to all others
}

int	main()
{
	DDRB |= (1 << DDB0);
	DDRD &= ~(1 << DDD2);			// External interrupts (12.2)
	EICRA |= (1 << ISC01);			// Launch INT0 interrupt when PD2 detects low (12.2.1)
	EIMSK |= (1 << INT0);			// Enable INT0 interrupt (12.2.2)
	SREG |= (1 << SREG_I);			// Enable interruptions globaly

	while (1) {}

	return (0);
}
