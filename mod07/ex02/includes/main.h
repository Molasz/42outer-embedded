/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:57:19 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 15:57:52 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include <avr/io.h>
# include <util/crc16.h>

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
