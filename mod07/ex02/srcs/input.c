/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:02:55 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/24 15:45:17 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "utils.h"
#include "uart.h"

static enum CMD_TYPE	check_cmd(char *cmd)
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

static void	status(node_t *node)
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

static void	set_id(node_t *node, char *str)
{
	uint32_t	id = ft_atou(str);

	node->id = id;
}

static uint8_t	set_prio(node_t *node, char *str)
{
	int32_t	prio;

	if (str[0] == '-')
		prio = ft_atou(str + 1) * -1;
	else
		prio = ft_atou(str);

	if (prio < INT16_MIN || prio > INT16_MAX)
		return (1);
	node->prio = prio;
	return (0);
}

static void	set_tag(node_t *node, char *str)
{
	uint8_t	i = 0;

	while (str[i])
	{
		node->tag[i] = str[i];
		i++;
	}
	node->tag[i] = '\0';
}

static uint8_t	run_cmd(enum CMD_TYPE cmd, node_t *node, char *value, uint8_t j)
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
			node->magic = 0xFFFFFFFF;
			break;
		default:
			return (1);
	}
	update_node(node, cmd);
	return (0);
}

static uint8_t	parse_input(char *str, node_t *node)
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
	return (run_cmd(cmd_type, node, str + i, j));
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
		if (c >= ' ' && c <= '~' && i < 48)
		{
			uart_tx(c);
			buff[i++] = c;
		}
	}
	while (c != '\r');
	buff[i] = '\0';
	uart_printstr("\r\n");
	if (parse_input(buff, node))
		uart_printstr("Bad user input");
	uart_printstr("\r\n");
}


