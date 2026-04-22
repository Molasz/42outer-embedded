/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/22 12:21:44 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

#define DEBUG 0
#define HEX "0123456789ABCDEF"

uint8_t	last_read = 0;
uint8_t	last_status = 0;
uint8_t	last_data = 0;
uint8_t	data[7];
uint8_t	delay_flag = 0;

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

void uart_printhex(uint8_t n)
{
	uart_tx(HEX[n / 16]);
	uart_tx(HEX[n % 16]);
}

void	i2c_status(uint8_t data, char *str)
{
	last_status = TWSR & 0xF8;
	if (DEBUG)
	{
		uart_printstr(str);
		uart_printstr("0x");
		uart_printhex(data);
		uart_printstr(": 0x");
		uart_printhex(last_status);
		uart_printstr("\r\n");
	}
}

void	i2c_init()
{
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));				// Prescaler 1
	TWBR = 72;											// 16M / (16 + (2 * TWBR) * prescaler) = 100k (21.5.2)
	TWCR |= (1 << TWEN);								// Enable IC2
}

void	i2c_start()
{
	int8_t	start;

	start = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);	// START condition | Clear flag, say to IC2 can start next operation
	TWCR = start;
	while (!(TWCR & (1 << TWINT)));						// Wait until IC2 end operation
	i2c_status(start, "Start: ");
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
	i2c_status(data, "Write: ");
}

void	i2c_read()
{
	uint8_t	read;

	if (last_read)										// Is last byte to read?
		read = (1 << TWEN) | (1 << TWINT);
	else												// If not add ACK
		read = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);

	TWCR = read;
	while (!(TWCR & (1 << TWINT)));						// Wait until IC2 end operation
	last_data = TWDR;									// Save readed data
	i2c_status(read, "Read: ");
}

void	print_hex_value(char c)
{
	uint8_t	i = 0;

	data[6] = c;
	while (i < 7)
	{
		uart_printhex(data[i]);
		uart_printstr(" ");
		i++;
	}
	uart_printstr("\r\n");
}

uint8_t	i2c_validate_status(uint8_t exp)
{
	if (last_status != exp)
		return (1);
	return (0);
}

uint8_t	i2c_aht20_init()
{
	if (DEBUG)
		uart_printstr("AHT20 init\r\n");
	i2c_start();
	if (i2c_validate_status(0x08)) return (1);
	i2c_write(0x38 << 1 | 1);							// Read
	if (i2c_validate_status(0x40)) return (1);
	last_read = 1;
	i2c_read();
	if (i2c_validate_status(0x58)) return (1);
	i2c_stop();
	if (!(last_data & (1 << 3)))						// If 3rd bit is 0 | Calibrate
	{
		if (DEBUG)
			uart_printstr("Start calibration\r\n");
		i2c_start();
		if (i2c_validate_status(0x08)) return (1);
		i2c_write(0x38 << 1);							// Write
		if (i2c_validate_status(0x18)) return (1);
		i2c_write(0xBE);								// Calibration command
		if (i2c_validate_status(0x28)) return (1);
		i2c_write(0x08);
		if (i2c_validate_status(0x28)) return (1);
		i2c_write(0x00);
		if (i2c_validate_status(0x28)) return (1);
		_delay_ms(10);									// Calibration delay
		i2c_stop();
		if (DEBUG)
			uart_printstr("End calibration\r\n");
	}
	return (0);
}

uint8_t	i2c_aht20_measurement()
{
	uint8_t	i;

	if (DEBUG)
		uart_printstr("\r\nStart measurement\r\n");
	i2c_start();
	if (i2c_validate_status(0x08)) return (1);
	i2c_write(0x38 << 1);								// Write
	if (i2c_validate_status(0x18)) return (1);
	i2c_write(0xAC);									// Measurement command
	if (i2c_validate_status(0x28)) return (1);
	i2c_write(0x33);
	if (i2c_validate_status(0x28)) return (1);
	i2c_write(0x00);
	if (i2c_validate_status(0x28)) return (1);
	i2c_stop();
	_delay_ms(80);										// Measurement delay
	i2c_start();
	if (i2c_validate_status(0x08)) return (1);
	i2c_write(0x38 << 1 | 1);							// Read
	if (i2c_validate_status(0x40)) return (1);
	last_read = 0;
	do
	{
		if (delay_flag)
			_delay_ms(10);
		i2c_read();
		if (i2c_validate_status(0x50)) return (1);
		data[0] = last_data;
		delay_flag = 1;
	}
	while (last_data & (1 << 7));						// Wait until measurement is ready
	delay_flag = 0;
	i = 0;
	while (i < 5)										// Read rest of bytes
	{
		i2c_read();
		if (i2c_validate_status(0x50)) return (1);
		data[i + 1] = last_data;
		i++;
	}
	last_read = 1;
	i2c_read();											// Read last byte (CRC)
	if (i2c_validate_status(0x58)) return (1);
	i2c_stop();
	print_hex_value(last_data);
	return (0);
}

int	main()
{

	i2c_init();
	uart_init();

	_delay_ms(40);

	while (i2c_aht20_init())							// Validate AHT20 calibration
	{
		i2c_stop();
		uart_printstr("AHT20 init error | Retrying in 1s\r\n");
		_delay_ms(1000);
	}

	while (1)
	{
		if (i2c_aht20_measurement())					// Validate AHT20 measurement
		{
			i2c_stop();
			uart_printstr("AHT20 measurement error | Retrying in 2s\r\n");
		}
		_delay_ms(2000);
	}

	return (0);
}
