/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/16 20:08:28 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile int8_t	n = 0;

void	update_leds()
{
	PORTB = (PORTB & 0xE8) | (n & 0x07) | ((n & 0x08) << 1);
}

void	INT0_vect() __attribute__((signal));

void	INT0_vect()
{
	n++;
	update_leds();
	_delay_ms(200);
	EIFR = (1 << INTF0);
}

void	PCINT2_vect() __attribute__((signal));

void	PCINT2_vect()
{
	if (!(PIND & (1 << PIND4)))
	{
		n--;
		update_leds();
		_delay_ms(200);
	}
	PCIFR = (1 << PCIF2);
}

int	main()
{
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);

	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);
	PCICR |= (1 << PCIE2);		// Enable intruptions for all PD changes
	PCMSK2 |= (1 << PCINT20);	// Only allow PD4 to launch PCINT2 interruption
	SREG |= (1 << SREG_I);

	while (1) {}

	return (0);
}
