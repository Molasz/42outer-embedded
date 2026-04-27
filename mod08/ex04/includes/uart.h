/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 20:51:23 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/27 20:52:12 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_H

# define UART_H

# include <avr/io.h>

# define UART_RATE (F_CPU / (8UL * BAUD)) - 1

void	uart_init();
char	uart_rx(void);
void	uart_tx(char c);
void	uart_printstr(const char *str);

#endif

