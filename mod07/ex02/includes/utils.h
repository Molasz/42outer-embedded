/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:31:12 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 18:11:16 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <avr/io.h>

uint8_t	ft_strcmp(const char *str1, const char *str2);
int64_t	ft_atou(const char *str);
uint8_t	validate_int(char *str);
uint8_t	validate_alnum(char *str);

#endif
