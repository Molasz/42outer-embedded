/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 20:57:02 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/27 22:13:43 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int8_t	ft_strcmp(const char *str1, const char *str2)
{
	uint8_t i = 0;

	while (str1[i] && str2[i])
	{
		if (str1[i] != str2[i])
			break;
		i++;
	}
	return (str1[i] - str2[i]);
}

static uint8_t	validate_hex(char c)
{
	return ((c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') || (c >= '0' && c <= '9'));
}

uint8_t	validate_color(const char *str)
{
	uint8_t	i = 0;

	while (i < 6)
		if (!validate_hex(str[i++]))
			return (1);
	if (!(str[i] == 'D' || str[i] == 'd'))
		return (1);
	i++;
	return (!(str[i] >= '6' && str[i] <= '8'));
}

static uint8_t	char_hex(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (c - 'a' + 10);
}

uint8_t	parse_hex(const char *hex)
{
	return (char_hex(hex[0]) * 16 + char_hex(hex[1]));
}
