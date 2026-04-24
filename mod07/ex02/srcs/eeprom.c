/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:55:09 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/24 15:51:43 by molasz-a         ###   ########.fr       */
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

uint8_t	eeprom_safe_write(uint16_t addr, uint8_t data)
{
	eeprom_write(addr, data);
	return (eeprom_read(addr) != data);
}
