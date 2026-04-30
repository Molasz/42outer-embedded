/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:22:08 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:31:45 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

int	main(void)
{
	volatile long	n = 0;		// Volatile disable preprocessor optimizations on n

	DDRB |= (1 << DDB1);

	while (1)
	{
		n = 400000;				// 16M (F_CPU) * 0.5s / ~20 (While instructions)
		while(n-- > 0) {}
		PORTB ^= (1 << PORTB1);	// Bitwise operation !PB0
	}

	return (0);
}
