/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:12 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/13 16:49:26 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int	main()
{								// 13.3 Alternate port function
	DDRB |= (1 << PB1);			// 15.11 Timer/Counter
	TCCR1A |= (1 << COM1A0);	// 15.2 0-1
	TCCR1B |= (1 << WGM12);		// CTC
	OCR1A = 31250 - 1;			// Compare register
	TCCR1B |= (1 << CS12);		// Prescaler 256

	while (1) {}

	return (0);
}
