/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:05:15 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/24 16:21:38 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "eeprom.h"
#include "uart.h"

static uint16_t	crc16(node_t *node)
{
	uint8_t		*ptr, *end;
	uint16_t	crc = 0;

	ptr = (uint8_t *)&(node->id);
	end = (uint8_t *)&(node->integ);
	while (ptr < end)
	{
		crc = _crc16_update(crc, *ptr);
		ptr++;
	}
	return (crc);
}

static void	node_init(node_t *node)
{
	node->magic = 0;
	node->id = 0;
	node->prio = 0;
	node->tag[0] = '\0';
	node->integ = 0;
}

static uint8_t	check_magic(uint16_t addr)
{
	int8_t		i = 3;
	uint32_t	magic = 0;

	while (i >= 0)
	{
		magic = eeprom_read(addr + i) | magic << 8;
		i--;
	}
	return (magic == MAGIC_NUM);
}

static uint8_t	read_node(node_t *node)
{
	uint8_t		*ptr, *end;
	uint16_t	addr;

	end = (uint8_t *)&(node->integ) + sizeof(uint16_t);
	ptr = (uint8_t *)&(node->magic);
	addr = SLOT_ADDR(slot);
	while (ptr < end)
	{
		*ptr = eeprom_read(addr);
		ptr++;
		addr++;
	}
	return (0);
}

void	find_node(node_t *node)
{
	uint8_t	i = 0;

	while (i < 4)
	{
		if (check_magic(SLOT_ADDR(i)))
		{
			uart_printstr("Node found ");
			uart_printuint(i);
			uart_printstr("\r\n");
			read_node(node);
			if (crc16(node) != node->integ)
				uart_printstr("CRITICAL: Data corruption detected!\r\n");
			else
			{
				slot = i;
				return;
			}
		}
		i++;
	}
	uart_printstr("No node found\r\n");
	node_init(node);
}

static void	write_node(node_t *node)
{
	uint8_t		*ptr, *end, corrupt = 0, slot_attm = 0;
	uint16_t	addr;

	if (slot < 0)
		slot = 0;
	end = (uint8_t *)&(node->integ) + sizeof(uint16_t);
	do
	{
		ptr = (uint8_t *)&(node->magic);
		addr = SLOT_ADDR(slot);
		while (ptr < end)
		{
			if (eeprom_safe_write(addr, *ptr))
			{
				if (corrupt)
					uart_printstr("Fail\r\n");
				else
				{
					uart_printstr("Corruption detected.\r\n");
					corrupt = 1;
				}
				if (slot >= 3)
					slot = 0;
				else
					slot++;
				slot_attm++;
				uart_printstr("Relocating config to slot ");
				uart_printuint(slot);
				uart_printstr("... ");
				break;
			}
			ptr++;
			addr++;
		}
	} while (ptr < end && slot_attm < 3);
	if (corrupt)
	{
		if (slot_attm < 3)
			uart_printstr("Success\r\nDone.");
		else
		{
			uart_printstr("Fail\r\nCRITICAL EEPROM FAILURE: No slots remaining");
			slot = 4;
		}
	}
}

void	update_node(node_t *node, enum CMD_TYPE cmd)
{
	if (cmd == STATUS) return;
	if (cmd != RESET)
		node->magic = MAGIC_NUM;
	node->integ = crc16(node);
	write_node(node);
}

