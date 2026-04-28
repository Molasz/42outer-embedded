/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:38:49 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/28 13:22:01 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "adc.h"

void	adc_init()
{
	ADMUX |= (1 << REFS0) | (1 << ADLAR);
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADATE);
}

void	ADC_vect() __attribute__((signal));

void	ADC_vect()
{
	GPIOR0 = ADCH;					// GPIOR0 RV1 value
	GPIOR1 = (1 << RV1_FLAG);		// GPIOR1 [0] RV1 flag
}

