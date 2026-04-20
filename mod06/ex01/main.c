/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/20 20:32:01 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/twi.h>

// 2-wire Serial Interface (21)

const char	*hex = "0123456789abcdef";
char	buff[3] = "  \0";

void uart_init()
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

char	*int_hex(uint8_t n)
{
	buff[1] = hex[n % 16];
	buff[0] = hex[n / 16];
	return (buff);
}

void	i2c_init()
{
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));				// Prescaler 1
	TWBR = 72;											// 16M / (16 + (2 * TWBR) * prescaler) = 100k (21.5.2)
	TWCR |= (1 << TWEN);								// Enable IC2
}

void	i2c_start()
{
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);	// START condition | Clear flag, say to IC2 can start next operation
	while (!(TWCR & (1 << TWINT)));						// Wait until IC2 end operation
}

void	i2c_stop()
{
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);	// STOP condition | Clear flag
	while (TWCR & (1 << TWSTO));						// Wait until STOP complete
}

void	i2c_write(unsigned char data)
{
	TWDR = data;										// Set data
	TWCR = (1 << TWEN) | (1 << TWINT);					// Clear flag
	while (!(TWCR & (1 << TWINT)));						// Wait until IC2 end operation
}

void	i2c_read()
{

}

void	print_hex_value(char c)
{}

void	i2c_print_status()
{
	uart_printstr("0x");
	uart_printstr(int_hex(TWSR & 0xF8));
	uart_printstr("\r\n");
}

int	main()
{
	i2c_init();
	uart_init();

	while (1)
	{
		i2c_start();
		i2c_print_status();
		i2c_write(0x38 << 1);									// AHT20 address
		i2c_print_status();
		i2c_stop();
	}

	return (0);
}
