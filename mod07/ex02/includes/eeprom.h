/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eeprom.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 11:43:08 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 15:58:27 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EEPROM_H
# define EEPROM_H

# include <avr/io.h>
# include <avr/eeprom.h>

uint8_t	eeprom_read(uint16_t addr);
void	eeprom_write(uint16_t addr, uint8_t data);
typedef struct node_s
{
	uint32_t	magic;
	uint32_t	id;
	int16_t		priority;
	char		tag[32];
	uint16_t	integrity;
} node_t;

enum CMD_TYPE
{
	STATUS,
	ID,
	PRIO,
	TAG,
	RESET,
	UNKNOWN
};

# define MAGIC_NUM 0xDEADBEEF
# define NODE_SIZE sizeof(node_t)	// 44 bytes

#endif
