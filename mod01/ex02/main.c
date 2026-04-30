/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:18 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:31:57 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int	main(void)
{
	DDRB	|= (1 << DDB1);						// 15.11 Timer/Counter
	TCCR1A	|= (1 << COM1A1) | (1 << WGM11);	// 15.3 1-0 Toggle PB1 on compare match
	TCCR1B	|= (1 << WGM12) | (1 << WGM13);		// 15.5 Fast PWM | When timer match ICR1 restarts
	ICR1	= 62500 - 1;						// Compare register | 16M (F_CPU) / 256 (Prescaler)
	OCR1A	= ICR1 / 10;						// 10% duty
	TCCR1B	|= (1 << CS12);						// Prescaler 256 | Timer starts

	while (1) {}

	return (0);
}
