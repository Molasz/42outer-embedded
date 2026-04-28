/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spi.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/27 20:50:51 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/28 10:44:52 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPI_H
# define SPI_H

# include <avr/io.h>

void	spi_init(void);
void	spi_send(uint8_t data);
void	update_led(int8_t led, uint8_t aled, uint8_t r, uint8_t g, uint8_t b);
void	update_leds(int8_t led, uint8_t r, uint8_t g, uint8_t b);

#endif


