/**
 * @file spi.h
 * @brief Header file with declarations and macros for hardware SPI.
 * 
 * This file provides function prototypes, type definitions, and constants
 * for hardware-based SPI communication on AVR0 microcontrollers.
 * 
 * @author g.raf
 * @date 2025-09-27
 * @version 1.0 Release
 * @copyright
 * Copyright (c) 2025 g.raf
 * Released under the GPLv3 License. (see LICENSE in repository)
 * 
 * @note This file is part of a larger project and subject to the license specified in the repository. For updates and the complete revision history, see the GitHub repository.
 *
 * @see https://github.com/0x007e/hal-avr0-mega "AVR ATmega GitHub Repository"
 */

#ifndef SPI_H_
#define SPI_H_

    #ifndef SPI2X_ENABLE
        /**
         * @def SPI2X_ENABLE
         * @brief Enables the SPI double speed mode.
         *
         * @details
         * When defined, this macro activates the `SPI_CLK2X` bit, which reduces the SPI clock prescaler from `4` to `2`, effectively doubling the SPI clock speed. The SPI2X_ENABLE mode is useful when higher SPI data transfer speeds are required. It modifies the SPI clock frequency by changing the division factor applied to the peripheral clock (`F_PER`).
         *
         * @note Typically used in conjunction with the SPI clock prescaler bits (`PRESC[1:0]`) in the SPI control register (`CTRLA`).
         */
        //#define SPI2X_ENABLE

        #ifdef _DOXYGEN_    // Used for documentation, can be ignored
            #define SPI2X_ENABLE
        #endif
    #endif

    #ifndef SPI_CLOCK
        /**
         * @def SPI_CLOCK
         * @brief Default SPI clock prescaler setting.
         *
         * @details
         * This macro sets the SPI clock prescaler to configure the SPI clock frequency. If not previously defined, it defaults to `/4`. The prescaler divides the peripheral clock frequency (`F_CPU`) according to the table below, influenced by the `SPI2X` bit (double speed mode):
         *
         * // +---------------+-------+------+
         * // |         SPI2X |   0   |   1  |
         * // +---------------+-------+------+
         * // | 0x00 -> F_CPU | / 4   | / 2  |
         * // | 0x01 -> F_CPU | / 16  | / 8  |
         * // | 0x02 -> F_CPU | / 64  | / 32 |
         * // | 0x03 -> F_CPU | / 128 | / 64 |
         * // +---------------+-------+------+
         *
         * Where `F_CPU` is the peripheral clock frequency.
         *
         * @note To prevent this, do not exceed the maximum clock frequency of the slave to prevent unwanted system behavior.
         */
        #define SPI_CLOCK 0x00
    #endif

	#ifndef SPI_DDR
        /**
         * @def SPI_PORT
         * @brief Specifies the direction register for the SPI interface.
         *
         * @details
         * This macro defines the default direction register used by the SPI bus It determines which hardware port is used for SPI pin direction settings.
         *
         * By default, `SPI_DDR` is set to `DDRB`. Override this macro before including the SPI module if an alternate configuration is needed.
         *
         * @note Always verify and configure the SPI port to match the selected pin routing.
         */
        #define SPI_DDR DDRB
    #endif

    #ifndef SPI_PORT
        /**
         * @def SPI_PORT
         * @brief Specifies the port register for the SPI interface.
         *
         * @details
         * This macro defines the default port register used by the SPI bus It determines which hardware port is used for SPI pin control.
         *
         * By default, `SPI_PORT` is set to `PORTB`. Override this macro before including the SPI module.
         *
         * @note Always verify and configure the SPI port to match the selected pin routing.
         */
        #define SPI_PORT PORTB
    #endif

    #ifndef SPI_MOSI
        /**
         * @def SPI_MOSI
         * @brief Specifies the bit mask for the SPI MOSI (Master Out Slave In) pin.
         *
         * @details
         * This macro defines the pin used to identify the `MOSI` signal in SPI operations. The `MOSI` pin transmits data from the master to the slave device in SPI communication. By default, `SPI_MOSI` is set to `PINB5`. Override this macro before including the SPI module if a different pin assignment for MOSI is needed.
         *
         * @note Ensure this value matches the physical pin layout for your hardware.
         */
        #define SPI_MOSI PINB5
    #endif
    
    #ifndef SPI_MISO
        /**
         * @def SPI_MISO
         * @brief Specifies the bit mask for the SPI MISO (Master In Slave Out) pin.
         *
         * @details
         * This macro defines the pin mask used to identify the `MISO` signal in SPI operations. The `MISO` pin is used for data transmission from the slave to the master device in SPI communication. By default, `SPI_MISO` is set to `PINB6`. Override this macro before including the SPI module if a different pin assignment for MISO is needed.
         *
         * @note Ensure this value matches your hardware configuration.
         */
         #define SPI_MISO PINB6
    #endif

    #ifndef SPI_SCK
        /**
         * @def SPI_SCK
         * @brief Specifies the bit mask for the SPI SCK (Serial Clock) pin.
         *
         * @details
         * This macro defines the pin mask used to identify the `SCK` (Serial Clock) signal in SPI operations. The `SCK` pin provides the clock signal used to synchronize data transfer between the master and slave devices. By default, `SPI_SCK` is set to `PINB7`. Override this macro before including the SPI module if a different pin assignment for SCK is required.
         *
         * @note Ensure this value matches your hardware configuration for reliable communication.
         */
        #define SPI_SCK PINB7
    #endif

    #ifndef SPI_SS
	
        /**
         * @def SPI_SS
         * @brief Specifies the SPI SS (Slave Select) pin.
         *
         * @details
         * This macro defines the `SS` signal pin for SPI communication. The `SS` pin is used to configure settings such as pull-up resistors, invertion of input and interrupt triggering. By default, `SPI_SS` is set to `PINB4`. Override this macro before including the SPI module if a different pin control configuration is needed.
         *
         * @note Ensure the pin control register matches the selected hardware SPI port and pin mapping for reliable communication.
         */
        #define SPI_SS PINB4

        #ifndef SPI_ENABLE_MASTER_ABORT
            /**
             * @def SPI_ENABLE_MASTER_ABORT
             * @brief Enables the SPI master abort feature.
             *
             * @details
             * This macro activates the SPI master abort functionality, which provides a mechanism to safely initialize the master mode. If not defined elsewhere, this macro is automatically defined, along with the `SPI_SS` macro, which specifies the pin control register for the SPI Slave Select (SS) pin. The `SPI_SS` macro defines settings such as pull-up resistors, input inversion, and interrupt triggering on the SS pin, ensuring reliable communication and proper SPI master abort behavior on initialization.
             *
             * @note Override `SPI_ENABLE_MASTER_ABORT` and `SPI_SS` in project configuration if custom behavior or pin settings are required.
             *
             * @see SPI_SS for SS pin control configuration.
             */
            #define SPI_ENABLE_MASTER_ABORT
        #endif
    #endif

    #ifndef SPI_SPIE
        /**
         * @def SPI_SPIE
         * @brief Enable or disable SPI interrupt-based processing.
         *
         * @details
         * This macro controls whether the SPI processing uses interrupts or polling. When `SPI_SPIE` is defined, SPI operation is expected to use interrupt-driven processing. If it is undefined or commented out, the SPI module uses polling for data transfer completion. The current library implementation does **not** include interrupt handling. If interrupts are enabled by defining this macro, polling should be disabled by commenting out or undefining this macro to avoid conflicts.
         *
         * @note Interrupts and polling should not be used simultaneously. Uncomment or define this macro to enable interrupt-based SPI handling,or comment it out to use polling-based SPI processing.
         */
        // #define SPI_SPIE
    #endif

    #include <avr/io.h>
    #include "../../common/enums/SPI_enums.h"

    SPI_Status spi_init(SPI_Direction direction, SPI_Polarity setup, SPI_Phase sample);
    void spi_disable(void);
    void spi_select(SPI_Select mode);

    #ifndef SPI_SPIE
	    unsigned char spi_transfer(unsigned char data);
    #endif

#endif /* SPI_H_ */