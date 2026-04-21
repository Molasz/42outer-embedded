/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/21 20:55:36 by molasz-a         ###   ########.fr       */
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
uint8_t	data[5];

uint8_t	temp_his[3];
uint8_t	hum_his[3];

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

void	uart_putnbr(uint32_t nb)
{
	if (nb >= 10)
		uart_putnbr(nb / 10);
	uart_tx((nb % 10) + '0');
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

	if (last_read)
		read = (1 << TWEN) | (1 << TWINT);
	else
		read = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);

	TWCR = read;
	while (!(TWCR & (1 << TWINT)));						// Wait until IC2 end operation
	last_data = TWDR;
	i2c_status(read, "Read: ");
}

void	print_data()
{
	static uint8_t	i = 0;
	uint32_t		temp, hum;
	int32_t			temp_fix;

	hum = ((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | (data[2] >> 4);
	temp = (((uint32_t)data[2] & 0x0F) << 16) | ((uint32_t)data[3] << 8) | (data[4]);

	hum = (uint32_t)((((uint64_t) hum * 10000) + (1UL << 19)) >> 20);
	temp_fix = (int32_t)((((uint64_t)temp * 20000) + (1UL << 19)) >> 20) - 5000;

	uart_printstr("Temperature: ");
	if (temp_fix < 0)
	{
		uart_tx('-');
		temp_fix = -temp_fix;
	}
	uart_putnbr(temp_fix / 100);
	uart_tx('.');
	temp_fix %= 100;
	if (temp_fix < 10)
		uart_tx('0');
	uart_putnbr(temp_fix);
	uart_printstr("C, Humidity: ");
	uart_putnbr(hum / 100);
	uart_tx('.');
	hum %= 100;
	if (hum < 10)
		uart_tx('0');
	uart_putnbr(hum);
	uart_printstr("%\r\n");
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
	if (i2c_validate_status(TW_START)) return (1);
	i2c_write(0x38 << 1 | 1);							// Read
	if (i2c_validate_status(TW_MR_SLA_ACK)) return (1);
	last_read = 1;
	i2c_read();
	if (i2c_validate_status(TW_MR_DATA_NACK)) return (1);
	if (!(last_data & (1 << 3)))						// 3rd bit 0? Calibrate
	{
		if (DEBUG)
			uart_printstr("Start calibration\r\n");
		i2c_stop();
		i2c_start();
		if (i2c_validate_status(TW_START)) return (1);
		i2c_write(0x38 << 1);							// Write
		if (i2c_validate_status(TW_MT_SLA_ACK)) return (1);
		i2c_write(0xBE);								// Calibration command
		if (i2c_validate_status(TW_MT_DATA_ACK)) return (1);
		i2c_write(0x08);
		if (i2c_validate_status(TW_MT_DATA_ACK)) return (1);
		i2c_write(0x00);
		if (i2c_validate_status(TW_MT_DATA_ACK)) return (1);
		_delay_ms(10);
		if (DEBUG)
			uart_printstr("End calibration\r\n");
	}
	i2c_stop();
	return (0);
}

uint8_t	i2c_aht20_measurement()
{
	uint8_t	i;

	if (DEBUG)
		uart_printstr("\r\nStart measurement\r\n");
	i2c_start();
	if (i2c_validate_status(TW_START)) return (1);
	i2c_write(0x38 << 1);								// Write
	if (i2c_validate_status(TW_MT_SLA_ACK)) return (1);
	i2c_write(0xAC);									// Measurement command
	if (i2c_validate_status(TW_MT_DATA_ACK)) return (1);
	i2c_write(0x33);
	if (i2c_validate_status(TW_MT_DATA_ACK)) return (1);
	i2c_write(0x00);
	if (i2c_validate_status(TW_MT_DATA_ACK)) return (1);
	i2c_stop();
	_delay_ms(80);
	i2c_start();
	if (i2c_validate_status(TW_START)) return (1);
	i2c_write(0x38 << 1 | 1);							// Read
	if (i2c_validate_status(TW_MR_SLA_ACK)) return (1);
	last_read = 0;
	do
	{
		i2c_read();
		if (i2c_validate_status(TW_MR_DATA_ACK)) return (1);
		_delay_ms(10);
	}
	while (last_data & (1 << 7));						// Wait until measurement is ready
	i = 0;
	while (i < 5)										// Read all bytes
	{
		i2c_read();
		if (i2c_validate_status(TW_MR_DATA_ACK)) return (1);
		data[i] = last_data;
		i++;
	}
	last_read = 1;
	i2c_read();
	if (i2c_validate_status(TW_MR_DATA_NACK)) return (1);
	print_data();
	i2c_stop();
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
		if (i2c_aht20_measurement())					// AHT20 measurement
		{
			uart_printstr("AHT20 measurement error | Retrying in 2s\r\n");
			i2c_stop();
		}
		_delay_ms(2000);
	}

	return (0);
}
