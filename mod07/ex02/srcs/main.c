/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 19:04:14 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "eeprom.h"
#include "uart.h"
#include "utils.h"

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
	uint32_t	magic;

	magic = eeprom_read(addr);
	magic = magic << 8 | eeprom_read(addr + 1);
	magic = magic << 16 | eeprom_read(addr + 2);
	magic = magic << 24 | eeprom_read(addr + 3);

	return (magic == MAGIC_NUM);
}

uint8_t	find_node(void)
{
	uint8_t	i = 0;

	while (i < 4)
	{
		if (check_magic(SLOT))
			return (i + 1);
		i++;
	}
	return (0);
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
		uart_printuint(1);			// ?? SLOT
		uart_printstr("\r\nTAG: ");
		if (!node->tag[0])
			uart_printstr("Unconfigured");
		else
			uart_printstr(node->tag);
	}
}

void	update_integ(node_t *node)
{
	(void) node;
}

void	update_node(node_t *node)
{
	node->magic = MAGIC_NUM;
	update_integ(node);
}

uint8_t	set_id(node_t *node, char *str)
{
	uint32_t	id = ft_atou(str);

	if (id > UINT32_MAX)
		return (1);

	node->id = id;
	update_node(node);
	return (0);
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
			if (j) return (1);
			status(node);
			break;
		case ID:
			if (!j || j > 10 || value[0] == '-' || validate_int(value) || (j == 10 && ft_strcmp(value, "4294967295") > 0))
				return (1);
			return (set_id(node, value));
			break;
		case PRIO:
			if (!j || j > 9 || validate_int(value)) return (1);
			return (set_prio(node, value));
		case TAG:
			if (!j || j > 33 || validate_alnum(value)) return (1);
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
	char			cmd[16], value[33];
	uint8_t			i = 0, j = 0;
	enum CMD_TYPE	cmd_type;

	while (str[i] && str[i] != ' ')
	{
		cmd[i] = str[i];
		i++;
	}
	cmd[i] = '\0';
	cmd_type = check_cmd(cmd);
	while (str[i] && str[i] == ' ')
		i++;
	if (cmd_type == PRIO || cmd_type == ID)
	{
		while (str[i] && (str[i] == '0' && str[i + 1]))
			i++;
	}
	while (str[i])
		value[j++] = str[i++];
	value[j] = '\0';

	return (run_cmd(check_cmd(cmd), node, value, j));
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
	uint8_t	slot;

	uart_init();
	slot = find_node();
	if (!slot)
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
