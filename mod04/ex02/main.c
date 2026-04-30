/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:18:51 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile uint8_t	n = 0;
volatile uint8_t	tick;

volatile uint8_t	db_int0 = 0;
volatile uint8_t	db_pcint2 = 0;

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
		if (db_int0)
		{
			EIFR = (1 << INTF0);
			EIMSK |= (1 << INT0);
			db_int0 = 0;
		}

		if (db_pcint2)
		{
			PCIFR = (1 << PCIF2);
			PCMSK2 |= (1 << PCINT20);
			db_pcint2 = 0;
		}
	}
}

void	update_leds(void)
{
	PORTB = (PORTB & 0xE8) | (n & 0x07) | ((n & 0x08) << 1);
}

void	INT0_vect(void) __attribute__((signal));

void	INT0_vect(void)
{
	EIMSK &= ~(1 << INT0);		// Disable INT0
	db_int0 = 1;
	n++;
	update_leds();
	timer_init();
}

void	PCINT2_vect(void) __attribute__((signal));

void	PCINT2_vect(void)
{
	if (!(PIND & (1 << PIND4)))
	{
		PCMSK2 &= ~(1 << PCINT20);
		db_pcint2 = 1;
		n--;
		update_leds();
		timer_init();
	}
}

int	main(void)
{
	DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB2) | (1 << DDB4);

	EICRA |= (1 << ISC01);		// Launch INT0 interrupt when PD2 detects low (12.2.1)
	EIMSK |= (1 << INT0);		// Enable INT0 interrupt (12.2.2)
	PCICR |= (1 << PCIE2);		// Enable intruptions for all PD changes (12.2.4)
	PCMSK2 |= (1 << PCINT20);	// Only allow PD4 to launch PCINT2 interruption (12.2.6)
	SREG |= (1 << SREG_I);

	while (1) {}

	return (0);
}
