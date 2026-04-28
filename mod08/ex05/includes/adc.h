/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adc.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 12:42:26 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/28 12:50:38 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ADC_H
# define ADC_H

#include <avr/io.h>
#include <avr/interrupt.h>

void	adc_init(void);

#define RV1_FLAG 0

#endif
