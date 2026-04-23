/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uart.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/23 15:09:11 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/23 19:02:54 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UART_H

# define UART_H

# include <avr/io.h>

# define HEX "0123456789ABCDEF"
# define UART_RATE (F_CPU / (8UL * BAUD)) - 1

void	uart_init();
char	uart_rx(void);
void	uart_tx(char c);
void	uart_printstr(const char *str);
void	uart_printuint(uint32_t n);
void	uart_printint(int32_t n);
void	uart_printhex(uint8_t n);
void	uart_printaddr(uint32_t addr);

#endif
