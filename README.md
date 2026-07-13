[![Version: 1.0 Release](https://img.shields.io/badge/Version-1.0%20Release-green.svg)](https://github.com/0x007e/hal-avr-mega-spi) ![Release](https://github.com/0x007e/hal-avr-mega-spi/actions/workflows/release.yml/badge.svg) [![License GPLv3](https://img.shields.io/badge/License-GPLv3-lightgrey)](https://www.gnu.org/licenses/gpl-3.0.html)

# `hal-avr-mega-spi` - AVR ATmega SPI Hardware Abstraction

[![Ask DeepWiki](https://deepwiki.com/badge.svg)](https://deepwiki.com/0x007E/hal-avr-mega-spi)

The `hal-avr-mega-spi` is a lightweight `spi` hardware abstraction library for AVR `ATmega` microcontrollers. It provides a clean interface for `spi` initialization and communication while hiding direct register-level interaction from higher software layers. The library is intended for projects that want to separate low-level device startup code from application logic and establish a small, reusable system layer for AVR targets.

## Features

- `SPI` master configuration for AVR `ATmega` devices.
- `SPI` master communication for AVR `ATmega` devices.
- Encapsulation of low-level register access.
- Compact and reusable API for embedded projects.
- Foundation for layered HAL architectures.

> The `hal-avr-mega-spi` library reduces coupling by providing a focused interface for essential `spi` system services. This improves portability inside a project, keeps startup code organized, and makes higher-level modules easier to maintain.

## File Structure

![File Structure](https://0x007e.github.io/hal-avr-mega-spi/spi_8c__incl.png)

```
hal/
├── common/
|   ├── enums/
|   |   └── SPI_enums.h
|   └── macros/
|       └── PORT_macros.h
└── avr/
    └── spi/
        ├── spi.c
        └── spi.h
```

## Downloads

The library can be downloaded (`zip` or `tar`), cloned or used as submodule in a project.

| Type      | File               | Description              |
|:---------:|:------------------:|:-------------------------|
| Library   | [zip](https://github.com/0x007E/hal-avr-mega-spi/releases/latest/download/library.zip) / [tar](https://github.com/0x007E/hal-avr-mega-spi/releases/latest/download/library.tar.gz) | AVR `spi` library |

### Using with `git clone`

```sh
mkdir -p ./hal/
git clone https://github.com/0x007E/hal-common.git ./hal
mv ./hal/hal-common ./hal/common

mkdir -p ./hal/avr
git clone https://github.com/0x007E/hal-avr-mega-spi.git ./hal/avr
mv ./hal/avr/hal-avr-mega-spi ./hal/avr/spi
```

### Using as `git submodule`

```sh
git submodule add https://github.com/0x007E/hal-common.git   ./hal/common
git submodule add https://github.com/0x007E/hal-avr-mega-spi.git ./hal/avr/spi
```

## Programming

Additional parameters like spi clock speed, prescaler, pin definitions and many more can be setup in the [header file](./spi.h). A user friendly description can be found [here](https://0x007e.github.io/hal-avr-mega-spi/spi_8h.html).

```c
#include "../hal/avr/spi/spi.h"

int main(void)
{
	spi_init(SPI_MSB, SPI_Rising, SPI_Rising);

    while (1)
    {
        spi_select(SPI_Enable);

        unsigned char data = 0x00;
        data = spi_transfer(data);

        spi_select(SPI_Disable);
    }
}
```

## Additional Information

| Type       | Link               | Description              |
|:----------:|:------------------:|:-------------------------|
| AVR-Series | [pdf](https://ww1.microchip.com/downloads/en/devicedoc/atmel-8154-8-bit-avr-atmega16a_datasheet.pdf) | ATmega16A datasheet |

---

R. GAECHTER