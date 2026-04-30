/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:12 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:32:03 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int	main(void)
{
	DDRB |= (1 << DDB1);		// 13.3.1 Alternate port function
								// 15.11 Timer/Counter
	TCCR1A |= (1 << COM1A0);	// 15.2 Output mode | 0-1 Toggle PB1 on compare match
	TCCR1B |= (1 << WGM12);		// CTC mode | When timer match OCR1A restarts
	OCR1A = 31250 - 1;			// Compare register | 16M (F_CPU) / 256 (Prescaler) / 2
	TCCR1B |= (1 << CS12);		// Prescaler 256 | Timer starts

	while (1) {}

	return (0);
}
