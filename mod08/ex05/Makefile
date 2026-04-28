MCU			= atmega328p
F_CPU		= 16000000UL
BAUD		= 115200
PORT		= /dev/ttyUSB0
PROGRAMMER	= arduino

NAME		= main

CC			= avr-gcc
OBJCOPY		= avr-objcopy
AVRDUDE		= avrdude
RM			= rm -rf

SDIR		= srcs
ODIR		= obj

SRCS		= main.c spi.c adc.c
OBJS		= $(addprefix $(ODIR)/, $(SRCS:.c=.o))

CFLAGS		 = -mmcu=$(MCU) -DF_CPU=$(F_CPU) -DBAUD=$(BAUD) -Os -Wall -Wextra -Werror -Iincludes

all: dir hex flash

dir:
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME).bin: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME).bin

hex: $(NAME).bin
	$(OBJCOPY) -O ihex $(NAME).bin $(NAME).hex

flash: hex
	$(AVRDUDE) -c $(PROGRAMMER) -p $(MCU) -P $(PORT) -b $(BAUD) -U flash:w:$(NAME).hex:i

clean:
	$(RM) $(ODIR)

fclean: clean
	$(RM) $(NAME).bin $(NAME).hex

re: fclean all

.PHONY: all dir hex flash clean fclean re
