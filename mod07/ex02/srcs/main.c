/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 15:58:27 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "eeprom.h"
#include "uart.h"
#include "utils.h"

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

void	status(void)
{}

void	set_id(char *str)
{
	(void) str;
}

void	set_prio(char *str)
{
	(void) str;
}

void	set_tag(char *str)
{
	(void) str;
}

void	reset(void)
{}

uint8_t	read_input(char *str)
{
	char			cmd[16], value[33];
	uint8_t			i = 0, j = 0;

	while (str[i] && str[i] != ' ')
	{
		cmd[i] = str[i];
		i++;
	}
	cmd[i] = '\0';
	while (str[i] && str[i] == ' ')
		i++;
	while (str[i])
		value[j++] = str[i++];
	value[j] = '\0';

	switch (check_cmd(cmd))
	{
		case STATUS:
			if (j) return (1);
			status();
			break;
		case ID:
			if (!j || validate_int(value)) return (1);
			set_id(value);
			break;
		case PRIO:
			if (!j || validate_int(value)) return (1);
			set_prio(value);
			break;
		case TAG:
			if (!j || j > 33 || validate_alnum(value)) return (1);
			set_tag(value);
			break;
		case RESET:
			reset();
			break;
		default:
			return (1);
	}
	return (0);
}

void	uart_input(void)
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
	if (read_input(buff))
		uart_printstr("Bad user input");
	uart_printstr("\r\n");
}

int	main()
{
	uart_init();

	while(1)
	{
		uart_input();
	}

	return (0);
}
