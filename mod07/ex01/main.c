/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:40:48 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/eeprom.h>

#define HEX "0123456789ABCDEF"

void uart_init(void)
{
	UCSR0A |= (1 << U2X0);
	UBRR0 = (F_CPU / (8UL * BAUD)) - 1;
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0);
	UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);
}

char	uart_rx(void)
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
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

void	uart_printnbr(uint16_t nb)
{
	if (nb >= 10)
		uart_printnbr(nb / 10);
	uart_tx((nb % 10) + '0');
}

void	uart_printhex(uint8_t n)
{
	uart_tx(HEX[n / 16]);
	uart_tx(HEX[n % 16]);
}

void	uart_printaddr(uint32_t addr)
{
	uint8_t		i = 8;

	while (i > 0)
	{
		uart_tx(HEX[(addr >> ((i - 1) * 4)) & 0x0F]);
		i--;
	}
}

uint8_t	char_hex(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	return (c - 'a' + 10);
}

uint16_t	uart_readhex(void)
{
	uint8_t		i = 0;
	uint16_t	n = 0;
	char	c;

	while (i < 3 && !(c == ' ' && i))
	{
		c = uart_rx();
		if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'))
		{
			uart_tx(c);
			n = (n * 16) + char_hex(c);
			i++;
		}
	}
	return (n);
}

uint8_t	eeprom_read(uint16_t addr)
{
	while (EECR & (1 << EEPE));					// Wait till other EEPROM operations end
	EEAR = addr;								// Addres to read
	EECR |= (1 << EERE);						// Trigger EEPROM read | CPU freeze till EEPROM read is complete
	return (EEDR);								// Return readed byte
}

void	eeprom_write(uint16_t addr, uint8_t data)
{
	if (eeprom_read(addr) == data) return;		// If is same value don't do nothing
	while (EECR & (1 << EEPE));					// Wait till other EEPROM operations end
	EEAR = addr;								// Addres to write
	EEDR = data;								// Data to write
	EECR |= (1 << EEMPE);
	EECR |= (1 << EEPE);						// Trigger EEPROM write
}

void	eeprom_hexdump_row(uint16_t addr, uint16_t new_addr)
{
	uint8_t	i = 0;
	uint8_t	data[16];

	uart_printaddr(addr);
	uart_tx(' ');
	while (i < 16)
	{
		data[i] = eeprom_read(addr + i);
		if (new_addr == addr + i)
		{
			uart_printstr("\x1b[31m");
			uart_printhex(data[i]);
			uart_printstr("\x1b[0m");
		}
		else
			uart_printhex(data[i]);
		uart_tx(' ');
		i++;
	}
	
	uart_tx('|');
	i = 0;
	while (i < 16)
	{
		if (data[i] >= 32 && data[i] <= 126)
			uart_tx(data[i]);
		else
			uart_tx('.');
		i++;
	}
	uart_printstr("|\r\n");
}

void	eeprom_hexdump(uint16_t addr)
{
	uint16_t	i = 0;

	while (i <= E2END)		// 0 to max EEPROM address
	{
		eeprom_hexdump_row(i, addr);
		i += 16;
	}
}

int	main(void)
{
	uint16_t	addr, data;

	uart_init();

	uart_printstr("> ");
	addr = uart_readhex();
	uart_tx(' ');
	data = uart_readhex();
	uart_printstr("\r\n");
	if (addr < 1024 && data < 256)
		eeprom_write(addr, data);
	else
		uart_printstr("Bad format\r\n");
	eeprom_hexdump(addr);

	while(1);

	return (0);
}
