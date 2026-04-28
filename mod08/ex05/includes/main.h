/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 20:57:54 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/28 13:10:42 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

typedef struct color_s
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
} color_t;

#endif
