/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:57:19 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/24 15:46:46 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <avr/io.h>
# include <util/crc16.h>
# include <avr/eeprom.h>

typedef struct node_s
{
	uint32_t	magic;
	uint32_t	id;
	int16_t		prio;
	char		tag[33];
	uint16_t	integ;
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
# define NODE_SIZE sizeof(node_t)
# define NSLOTS 4
# define SLOTS_START ((E2END + 1) - (NODE_SIZE * NSLOTS))

# define SLOT_ADDR(i) (SLOTS_START + (i * NODE_SIZE))

extern int8_t	slot;

void	find_node(node_t *node);
void	update_node(node_t *node, enum CMD_TYPE cmd);
void	read_input(node_t *node);

#endif
