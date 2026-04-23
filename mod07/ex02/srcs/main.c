/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 22:09:29 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "eeprom.h"
#include "uart.h"
#include "utils.h"

int8_t	slot = 0;

void	node_init(node_t *node)
{
	node->magic = 0;
	node->id = 0;
	node->prio = 0;
	node->tag[0] = '\0';
	node->integ = 0;
}

uint8_t	check_magic(uint16_t addr)
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

int8_t	find_node(void)
{
	uint8_t	i = 0;

	while (i < 4)
	{
		if (check_magic(SLOT_ADDR(i)))
			return (i);
		i++;
	}
	return (-1);
}

enum CMD_TYPE	check_cmd(char *cmd)
{
	if (!ft_strcmp(cmd, "STATUS"))
		return (STATUS);
	else if (!ft_strcmp(cmd, "SET_ID"))
		return (ID);
	else if (!ft_strcmp(cmd, "SET_PRIO"))
		return (PRIO);
	else if (!ft_strcmp(cmd, "SET_TAG"))
		return (TAG);
	else if (!ft_strcmp(cmd, "FACTORY_RESET"))
		return (RESET);
	return (UNKNOWN);
}

void	status(node_t *node)
{
	if (node->magic != MAGIC_NUM)
		uart_printstr("Node unconfigured");
	else
	{
		uart_printstr("NODE ID: ");
		uart_printuint(node->id);
		uart_printstr("\r\nPriority: ");
		uart_printint(node->prio);
		uart_printstr("\r\nSlot: ");
		uart_printuint(slot);
		uart_printstr("\r\nTAG: ");
		if (!node->tag[0])
			uart_printstr("Unconfigured");
		else
			uart_printstr(node->tag);
	}
}

void	update_integ(node_t *node)
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
	node->integ = crc;
}

void	write_node(node_t *node)
{
	uint16_t		*ptr, *end, addr, corrupt = 0;

	if (slot < 0)
		slot = 0;
	end = (uint16_t *)&(node->integ) + sizeof(uint16_t);
	ptr = (uint16_t *)&(node->magic);
	while (ptr < end && slot < 4)
	{
		ptr = (uint16_t *)&(node->magic);
		addr = SLOT_ADDR(slot);
		while (ptr < end)
		{
			if (eeprom_safe_write(addr, *ptr))
			{
				if (corrupt)
					uart_printstr("Fail\r\n");
				else
					uart_printstr("Corruption detected.\r\nRelocating config to slot ");
				corrupt = 1;
				uart_printuint(slot);
				uart_printstr("... ");
				slot++;
				break;
			}
			ptr++;
			addr++;
		}
		if (corrupt)
			uart_printstr("Success\r\nDone.");
	}
}

void	update_node(node_t *node)
{
	node->magic = MAGIC_NUM;
	update_integ(node);
	write_node(node);
}

void	set_id(node_t *node, char *str)
{
	uint32_t	id = ft_atou(str);

	node->id = id;
	update_node(node);
}

uint8_t	set_prio(node_t *node, char *str)
{
	int32_t	prio;

	if (str[0] == '-')
		prio = ft_atou(str + 1) * -1;
	else
		prio = ft_atou(str);

	if (prio < INT16_MIN || prio > INT16_MAX)
		return (1);
	node->prio = prio;
	update_node(node);
	return (0);
}

void	set_tag(node_t *node, char *str)
{
	uint8_t	i = 0;

	while (str[i])
	{
		node->tag[i] = str[i];
		i++;
	}
	node->tag[i] = '\0';
	update_node(node);
}

uint8_t	run_cmd(enum CMD_TYPE cmd, node_t *node, char *value, uint8_t j)
{
	switch (cmd)
	{
		case STATUS:
			if (j) 
			{
				uart_printuint(j);
				return (1);
			}
			status(node);
			break;
		case ID:
			if (!j || j > 10 || value[0] == '-' || validate_int(value) || (j == 10 && ft_strcmp(value, "4294967295") > 0))
				return (1);
			set_id(node, value);
			break;
		case PRIO:
			if (!j || j > 9 || validate_int(value)) return (1);
			return (set_prio(node, value));
		case TAG:
			if (!j || j > 32 || validate_alnum(value)) return (1);
			set_tag(node, value);
			break;
		case RESET:
			node_init(node);
			break;
		default:
			return (1);
	}
	return (0);
}

uint8_t	parse_input(char *str, node_t *node)
{
	uint8_t			i = 0, j = 0, sign = 0;
	enum CMD_TYPE	cmd_type;

	while (str[i] && str[i] != ' ')
		i++;
	if (str[i] == ' ')
		str[i++] = '\0';
	cmd_type = check_cmd(str);
	while (str[i] && str[i] == ' ')
		i++;
	if (str[i] == '-')
	{
		sign = 1;
		i++;
	}
	if (cmd_type == PRIO || cmd_type == ID)
		while (str[i] && (str[i] == '0' && str[i + 1]))
			i++;
	if (sign)
		str[i-- - 1] = '-';
	while (str[i + j])
		j++;
	if (sign && j == 1)
		return (1);
	return (run_cmd(check_cmd(str), node, str + i, j));
}

void	read_input(node_t *node)
{
	char	buff[48];
	char	c;
	uint8_t	i = 0;

	uart_printstr("> ");
	do
	{
		c = uart_rx();
		if ((c == '\b' || c == 127) && i)
		{
			i--;
			uart_printstr("\b \b");
		}
		if (c >= ' ' && c <= '~')
		{
			uart_tx(c);
			buff[i++] = c;
		}
	}
	while (c != '\r' && i < 48);
	buff[i] = '\0';
	uart_printstr("\r\n");
	if (parse_input(buff, node))
		uart_printstr("Bad user input");
	uart_printstr("\r\n");
}

int	main()
{
	node_t	node;

	uart_init();
	slot = find_node();
	if (slot < 0)
	{
		uart_printstr("No node found\r\n");
		node_init(&node);
	}
	else
	{
		uart_printstr("Node found\r\n");
		// Read saved node
	}

	while(1)
	{
		read_input(&node);
	}

	return (0);
}
