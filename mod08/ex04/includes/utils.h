/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 20:56:20 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/27 21:40:34 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <avr/io.h>

int8_t	ft_strcmp(const char *str1, const char *str2);
uint8_t	validate_color(const char *str);
uint8_t	parse_hex(const char *hex);

#endif
