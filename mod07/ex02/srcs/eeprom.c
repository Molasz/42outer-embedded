/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:55:09 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 16:25:09 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "eeprom.h"

uint8_t	eeprom_read(uint16_t addr)
{
	while (EECR & (1 << EEPE));
	EEAR = addr;
	EECR |= (1 << EERE);
	return (EEDR);
}

void	eeprom_write(uint16_t addr, uint8_t data)
{
	if (eeprom_read(addr) == data) return;
	while (EECR & (1 << EEPE));
	EEAR = addr;
	EEDR = data;
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);
}
