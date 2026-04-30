/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: molasz-a <molasz.dev@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 01:21:38 by molasz-a          #+#    #+#             */
/*   Updated: 2026/04/30 15:40:34 by molasz-a         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <avr/io.h>
#include <avr/eeprom.h>

#define HEX "0123456789ABCDEF"

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

uint8_t	eeprom_read(uint16_t addr)
{
	while (EECR & (1 << EEPE));	// Wait till other EEPROM operations end
	EEAR = addr;				// Addres to read
	EECR |= (1 << EERE);		// Trigger EEPROM read | CPU freeze till EEPROM read is complete
	return (EEDR);				// Return readed byte
}

void	eeprom_hexdump(uint16_t addr)
{
	uint8_t	i = 0;
	uint8_t	data[16];

	uart_printaddr(addr);
	uart_tx(' ');
	while (i < 16)
	{
		data[i] = eeprom_read(addr + i);
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

int	main(void)
{
	uint16_t	i;

	uart_init();

	i = 0;
	while (i <= E2END)		// 0 to max EEPROM address
	{
		eeprom_hexdump(i);
		i += 16;
	}
	while (1);

	return (0);
}
