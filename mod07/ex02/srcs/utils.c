/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:30:21 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 16:00:23 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>

uint8_t	ft_strcmp(const char *str1, const char *str2)
{
	uint8_t i = 0;

	while (str1[i] && str2[i])
	{
		if (!str1[i] || !str2[i] || str1[i] != str2[i])
			break;
		i++;
	}
	return (str1[i] - str2[i]);
}

uint8_t	validate_int(char *str)
{
	uint8_t	i = 0;

	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (1);
		i++;
	}
	return (0);
}

uint8_t	validate_alnum(char *str)
{
	uint8_t	i = 0;

	while (str[i])
	{
		if (!((str[i] >= '0' && str[i] <= '9')
			|| (str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z')
			|| str[i] == '-' || str[i] == '_'
		))
			return (1);
		i++;
	}
	return (0);
}
