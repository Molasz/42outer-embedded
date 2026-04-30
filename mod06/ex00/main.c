/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:38:41 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/twi.h>

// 2-wire Serial Interface (21)

#define HEX "0123456789ABCDEF"

uint8_t	status;

void uart_init(void)
{
	UCSR0A |= (1 << U2X0);
	UBRR0 = (F_CPU / (8UL * BAUD)) - 1;
	UCSR0B |= (1 << TXEN0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

void	uart_tx(char c)
{
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

void	uart_printstr(const char *str)
{
	int			i = 0;

	while (str[i])
		uart_tx(str[i++]);
}

void uart_printhex(uint8_t n)
{
	uart_tx(HEX[n / 16]);
	uart_tx(HEX[n % 16]);
}

void	i2c_init(void)
{
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));				// Prescaler 1
	TWBR = 72;											// 16M / (16 + (2 * TWBR) * prescaler) = 100k (21.5.2)
	TWCR |= (1 << TWEN);								// Enable IC2
}

void	i2c_start(void)
{
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);	// START condition | Clear flag, say to IC2 can start next operation
	while (!(TWCR & (1 << TWINT)));						// Wait until IC2 end operation
}

void	i2c_stop(void)
{
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);	// STOP condition | Clear flag, say to IC2 can start next operation
	while (TWCR & (1 << TWSTO));						// Wait until STOP complete
}

void	i2c_status(void)
{
	status = TWSR & 0xF8;								// Read only status bits from TWSR
	uart_printstr("0x");
	uart_printhex(status);
	uart_printstr("\r\n");
}

void	i2c_ping(uint8_t addr)
{
	TWDR = (addr << 1) | 0;								// Sensor addres + 0 Read
	TWCR = (1 << TWEN) | (1 << TWINT);					// Clear flag
	while (!(TWCR & (1 << TWINT)));						// Wait until IC2 end operation
}

uint8_t	i2c_test(void)
{
	i2c_start();
	i2c_status();
	if (status != 0x08)									// Validate START
	{
		uart_printstr("START error\r\n");
		return (1);
	}
	i2c_ping(0x38);										// AHT20 address
	i2c_status();
	if (status != 0x18)									// Validate write
	{
		uart_printstr("Ping error\r\n");
		return (1);
	}
	i2c_stop();
	return (0);
}

int	main(void)
{
	i2c_init();
	uart_init();

	while (1)
	{
		if (i2c_test())
			i2c_stop();
	}

	return (0);
}
