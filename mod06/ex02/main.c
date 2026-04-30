/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:40:22 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <util/twi.h>
#include <util/delay.h>

#define DEBUG 0
#define HEX "0123456789ABCDEF"

uint8_t		last_read = 0;
uint8_t		last_status = 0;
uint8_t		last_data = 0;
uint8_t		aht20_state = 0;
uint8_t		data[5];
uint8_t		delay_flag = 0;

uint32_t	temp_his[3] = {0, 0, 0};
uint32_t	hum_his[3] = {0, 0, 0};

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

void	i2c_init(void)
{
	TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
	TWBR = 72;
	TWCR |= (1 << TWEN);
}

void	i2c_start(void)
{
	int8_t	start;

	start = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
	TWCR = start;
	while (!(TWCR & (1 << TWINT)));
	i2c_status(start, "Start: ");
}

void	i2c_stop(void)
{
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
	while (TWCR & (1 << TWSTO));
}

void	i2c_write(unsigned char data)
{
	TWDR = data;
	TWCR = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1 << TWINT)));
	i2c_status(data, "Write: ");
}

void	i2c_read(void)
{
	uint8_t	read;

	if (last_read)
		read = (1 << TWEN) | (1 << TWINT);
	else
		read = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);

	TWCR = read;
	while (!(TWCR & (1 << TWINT)));
	last_data = TWDR;
	i2c_status(read, "Read: ");
}

void	print_data(void)
{
	static uint8_t	i = 0;
	int32_t			temp;
	uint32_t		hum;

	if (i < 3)
		i++;
	temp = (temp_his[0] + temp_his[1] + temp_his[2]) / i;				// Temp average
	hum = (hum_his[0] + hum_his[1] + hum_his[2]) / i;					// Hum average

	hum = ((((uint64_t)hum * 10000) + (1UL << 19)) >> 20);				// Convert raw values using bitwise operations and round to nearest
	temp = ((((uint64_t)temp * 20000) + (1UL << 19)) >> 20) - 5000;

	uart_printstr("Temperature: ");										// Print real values with 2 decimals
	if (temp < 0)
	{
		uart_tx('-');
		temp = -temp;
	}
	uart_putnbr(temp / 100);
	uart_tx('.');
	temp %= 100;
	if (temp < 10)
		uart_tx('0');
	uart_putnbr(temp);
	uart_printstr("ºC, Humidity: ");
	uart_putnbr(hum / 100);
	uart_tx('.');
	hum %= 100;
	if (hum < 10)
		uart_tx('0');
	uart_putnbr(hum);
	uart_printstr("%\r\n");
}

void	save_data(void)													// Update history arrays
{
	temp_his[2] = temp_his[1];
	temp_his[1] = temp_his[0];
	temp_his[0] = (((uint32_t)data[2] & 0x0F) << 16) | ((uint32_t)data[3] << 8) | (data[4]);

	hum_his[2] = hum_his[1];
	hum_his[1] = hum_his[0];
	hum_his[0] = ((uint32_t)data[0] << 12) | ((uint32_t)data[1] << 4) | (data[2] >> 4);

	print_data();
}

uint8_t	i2c_validate_status(uint8_t exp)
{
	if (last_status != exp)
		return (1);
	return (0);
}

uint8_t	i2c_aht20_init(void)
{
	if (DEBUG)
		uart_printstr("AHT20 init\r\n");
	i2c_start();
	if (i2c_validate_status(TW_START)) return (1);
	i2c_write(0x38 << 1 | 1);
	if (i2c_validate_status(TW_MR_SLA_ACK)) return (1);
	last_read = 1;
	i2c_read();
	if (i2c_validate_status(TW_MR_DATA_NACK)) return (1);
	if (!(last_data & (1 << 3)))
	{
		if (DEBUG)
			uart_printstr("Start calibration\r\n");
		i2c_stop();
		i2c_start();
		if (i2c_validate_status(TW_START)) return (1);
		i2c_write(0x38 << 1);
		if (i2c_validate_status(TW_MT_SLA_ACK)) return (1);
		i2c_write(0xBE);
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

uint8_t	validate_crc(void)												// CRC8 Validation
{
	uint8_t	crc = 0xFF;													// CRC initial value | AHT20 datasheet
	uint8_t	raw_data[6] = {aht20_state, data[0], data[1], data[2], data[3], data[4]};
	uint8_t	i = 0, j;

	while (i < 6)
	{
		crc ^= raw_data[i];												// 'mix' actual CRC with current byte
		j = 0;
		while (j < 8)													// Read all 8 bits
		{
			if (crc & 0x80)												// Validate MSB (7) bit
				crc = (crc << 1) ^ 0x31;								// Divide by polynomial and shift | AHT20 datasheet
			else
				crc <<= 1;												// Only shift
			j++;
		}
		i++;
	}
	return (!(crc == last_data));										// If same as last byte readed data is correct
}

uint8_t	i2c_aht20_measurement(void)
{
	uint8_t	i;

	if (DEBUG)
		uart_printstr("\r\nStart measurement\r\n");
	i2c_start();
	if (i2c_validate_status(TW_START)) return (1);
	i2c_write(0x38 << 1);
	if (i2c_validate_status(TW_MT_SLA_ACK)) return (1);
	i2c_write(0xAC);
	if (i2c_validate_status(TW_MT_DATA_ACK)) return (1);
	i2c_write(0x33);
	if (i2c_validate_status(TW_MT_DATA_ACK)) return (1);
	i2c_write(0x00);
	if (i2c_validate_status(TW_MT_DATA_ACK)) return (1);
	i2c_stop();
	_delay_ms(80);
	i2c_start();
	if (i2c_validate_status(TW_START)) return (1);
	i2c_write(0x38 << 1 | 1);
	if (i2c_validate_status(TW_MR_SLA_ACK)) return (1);
	last_read = 0;
	do
	{
		if (delay_flag)
			_delay_ms(10);
		i2c_read();
		if (i2c_validate_status(TW_MR_DATA_ACK)) return (1);
		aht20_state = last_data;
		delay_flag = 1;
	}
	while (last_data & (1 << 7));
	delay_flag = 0;
	i = 0;
	while (i < 5)
	{
		i2c_read();
		if (i2c_validate_status(TW_MR_DATA_ACK)) return (1);
		data[i] = last_data;
		i++;
	}
	last_read = 1;
	i2c_read();
	if (i2c_validate_status(TW_MR_DATA_NACK)) return (1);
	if (validate_crc())													// Validate data integrity with CRC
		uart_printstr("CRC validation error | Corrupt data\r\n");
	else
		save_data();
	i2c_stop();
	return (0);
}

int	main(void)
{

	i2c_init();
	uart_init();

	_delay_ms(40);

	while (i2c_aht20_init())
	{
		i2c_stop();
		uart_printstr("AHT20 init error | Retrying in 1s\r\n");
		_delay_ms(1000);
	}

	while (1)
	{
		if (i2c_aht20_measurement())
		{
			uart_printstr("AHT20 measurement error | Retrying in 2s\r\n");
			i2c_stop();
		}
		_delay_ms(2000);
	}

	return (0);
}
