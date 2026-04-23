/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:43:08 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 21:38:57 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EEPROM_H
# define EEPROM_H

# include <avr/io.h>
# include <avr/eeprom.h>

uint8_t	eeprom_read(uint16_t addr);
void	eeprom_write(uint16_t addr, uint8_t data);
uint8_t	eeprom_safe_write(uint16_t addr, uint8_t data);

#endif
