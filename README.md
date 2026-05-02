# Piscine Embedded — 42 Paris

_This project has been created as part of the 42 curriculum by molasz-a._

> Part of [42 Barcelona — molasz-a](https://github.com/Molasz/42), a monorepo centralizing every project completed at 42 Barcelona.

## Description

A 3-week intensive embedded programming bootcamp held at 42 Paris. Working directly on ATmega328P-based devkits, every module builds progressively from bare-metal GPIO to multi-protocol sensor communication — all without Arduino abstractions, using only AVR registers and datasheets.

---

## Modules

| Module | Topic | Key concepts |
|--------|-------|--------------|
| [MOD00](#mod00--first-programs) | First Programs | Toolchain, GPIO, button debounce |
| [MOD01](#mod01--timers) | Timers | Timer1 CTC & Fast PWM, duty cycle |
| [MOD02](#mod02--uart-protocol) | UART Protocol | Serial TX/RX, interrupts, echo |
| [MOD03](#mod03--rgb--advanced-timers) | RGB & Advanced Timers | Multi-channel PWM, color wheel |
| [MOD04](#mod04--interruptions) | Interruptions | ISR, external interrupts, Timer0 |
| [MOD05](#mod05--analog) | Analog | ADC 8/10-bit, potentiometer, LDR, NTC, internal temp sensor |
| [MOD06](#mod06--i2c-protocol) | I2C Protocol | TWI peripheral, AHT20 sensor, temp & humidity |
| [MOD07](#mod07--eeprom) | EEPROM | Non-volatile storage, wear leveling, CRC integrity |
| [MOD08](#mod08--spi) | SPI | SPI master, WS2812-style RGB LEDs via shift registers |
| [MOD09](#mod09--7-segments-display) | 7-Segment Display | I2C I/O expander, multiplexing, digit rendering |

---

## Architecture

![Skills](https://skillicons.dev/icons?i=c,arduino&theme=dark)

```
├── mod00/   ex00/ ex01/ ex02/ ex03/ ex04/
├── mod01/   ex00/ ex01/ ex02/ ex03/
├── mod02/   ex00/ ex01/ ex02/ ex03/ ex04/
├── mod03/   ex01/ ex02/ ex03/ ex04/
├── mod04/   ex00/ ex01/ ex02/
├── mod05/   ex00/ ex01/ ex02/ ex03/ ex04/
├── mod06/   ex00/ ex01/ ex02/
├── mod07/   ex00/ ex01/ ex02/
├── mod08/   ex00/ ex01/ ex02/ ex03/ ex05/
└── mod09/   ex00/ ex01/ ex02/ ex03/ ex04/ ex05/ ex06/
```

Each exercise directory contains a `Makefile` with `all`, `hex`, `flash` and `clean` rules targeting the ATmega328P at 16 MHz over a USB serial programmer at 115200 baud.

---

_molasz-a · 42 Paris_
