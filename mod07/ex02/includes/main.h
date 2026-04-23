/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:57:19 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 19:48:33 by molasz-a         ###   ########.fr       */
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
# define NODE_SIZE sizeof(node_t)	// 44 bytes
# define SLOT E2END - (NODE_SIZE * 4)

#endif
