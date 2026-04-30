/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:41:07 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "eeprom.h"
#include "uart.h"
#include "utils.h"

int8_t	slot = 0;

int	main(void)
{
	node_t	node;

	uart_init();
	find_node(&node);

	while(1)
	{
		if (slot < 4)
			read_input(&node);
	}
	return (0);
}
