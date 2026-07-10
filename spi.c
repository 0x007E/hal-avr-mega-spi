/**
 * @file spi.c
 * @brief Source file with implementation of hardware SPI functions and macros.
 *
 * This file contains the definitions of function implementations and macros for hardware-based SPI communication on AVR-0/1/2-Series microcontrollers.
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
 * @important The SS pin must be HIGH during initialization of the bus to avoid unintended slave mode configuration.
 * 
 * @see spi.h for declarations and related information.
 * @see https://github.com/0x007e/hal-avr-mega "AVR ATmega GitHub Repository"
 */

#include "spi.h"

/**
 * @brief Initialize the SPI hardware interface in master mode with specified configuration.
 *
 * @param direction Specifies the bit order for SPI data transmission (MSB or LSB first).
 * @param setup Specifies the clock polarity (SPI_Polarity) to configure the clock idle state.
 * @param sample Specifies the clock phase (SPI_Phase) to configure the clock sampling edge.
 *
 * @return Returns an SPI_Status code indicating the result of initialization. Possible return values:
 * - SPI_None: Initialization completed successfully.
 * - SPI_Master_Abort: Initialization was aborted due to the Slave Select (SS) pin being low when `SPI_ENABLE_MASTER_ABORT` is enabled.
 *
 * @details
 * This function configures the SPI peripheral registers on the AVR ATTiny series device to initialize the SPI bus in master mode. It sets the SPI port multiplexer, configures the data direction for SPI pins, and sets up the SPI clock phase and polarity according to the specified parameters.
 * 
 * @important If the macro `SPI_ENABLE_MASTER_ABORT` is defined, special handling is enabled: if during initialization the SS pin is detected low, 
 * the SPI master initialization is aborted by disabling the SPI peripheral and returning the status `SPI_Master_Abort`.
 *
 * The function also sets pull-up resistors on the MISO and SS pins according to configuration, and configures SPI interrupts if the `SPI_SPIE` macro is defined.
 *
 * @note Ensure that the SPI pins and port multiplexer settings correspond to your hardware configuration. The function assumes a default SPI clock prescaler; customize `SPI_CLOCK` and `SPI2X_ENABLE` macros for different clock rates.
 *
 * @see SPI_Direction, SPI_Polarity, SPI_Phase for parameter options and configuration.
 * @see spi_disable() for disabling the SPI peripheral in case of master abort.
 */
SPI_Status spi_init(SPI_Direction direction, SPI_Polarity setup, SPI_Phase sample)
{
	#ifdef SPI_ENABLE_MASTER_ABORT
		SPI_DDR  &= ~((1<<SPI_MISO) | (1<<SPI_SS)); // Setup MISO and SS as input
		SPI_PORT |=   (1<<SPI_MISO) | (1<<SPI_SS);  // Activate pull up resistor at MISO and SS
	#else
		SPI_DDR  &= ~(1<<SPI_MISO);					// Setup MISO as input
		SPI_DDR  |=  (1<<SPI_SS);					// Setup SS as output
		SPI_PORT |=  (1<<SPI_MISO) | (1<<SPI_SS);	// Activate pull up resistor at MISO and drive SS pin HIGH
	#endif
	
	// Double speed setup
	#ifdef SPI2X_ENABLE
		SPSR = (1<<SPI2X);                      // Double speed mode activated
	#else
		SPSR = 0x00;                            // Double speed mode deactivated
	#endif
	
	#if SPI_CLOCK > 0
		SPCR = (0x03 & SPI_CLOCK);
	#else
		SPCR = 0x00;
	#endif
	
	// Master/Slave setup (direction setup)
	SPCR |= (1<<MSTR);
	
	// MSB/LSB first
	SPCR |= ((0x01 & direction)<<DORD);
	
	// Polarity and Phase of SCK and DATA
	SPCR |= ((0x01 & setup)<<CPOL) | ((0x01 & sample)<<CPHA);
	
	// SPI interrupt setup
	#ifdef SPI_SPIE
		SPCR  |= (1<<SPIE);
	#endif
	
	SPCR |= (1<<SPE);   // Activate the SPI Controller
	
	#ifdef SPI_ENABLE_MASTER_ABORT
		// Check if master abort has occurred
		if(!(SPCR & (1<<MSTR)))
		{
			spi_disable();
			return SPI_Master_Abort;
		}
	#endif
	
	// Setup SCK, MOSI and SS as output
	// PORT configuration gets overwritten from SPI controller
	SPI_DDR  |= (1<<SPI_SCK) | (1<<SPI_MISO) | (1<<SPI_MOSI) | (1<<SPI_SS);
	
	return SPI_None;
}

/**
 * @brief Disable the SPI hardware interface and reset related pins.
 *
 * @details
 * This function disables the SPI peripheral on the AVR ATTiny series device by clearing the master and enable bits in the SPI control register. It also resets the SPI mode bits to their default state. The function configures the SPI-related pins (`MOSI`, `MISO`, `SCK`, `SS`) by clearing their direction bits, effectively disabling SPI pin control, and drives these pins low by clearing the output register. Additionally, the pull-up resistor on the SS pin is disabled. If SPI interrupts are enabled via the `SPI_SPIE` macro, this function disables SPI interrupt requests. The port multiplexer configuration for SPI is cleared and the SPI interrupt flags are reset.
 *
 * @note After calling this function, the SPI peripheral and associated pins are fully disabled,
 * which is necessary before changing SPI configurations or powering down the SPI module to reduce power consumption.
 */
void spi_disable(void)
{
	// Disable SPI
	SPCR &= ~(1<<SPE);
	SPI_DDR  &= ~((1<<SPI_SCK) | (1<<SPI_MOSI) | (1<<SPI_MISO) | (1<<SPI_SS));
	SPI_PORT &= ~((1<<SPI_SCK) | (1<<SPI_MOSI) | (1<<SPI_MISO) | (1<<SPI_SS));
	
	#ifdef SPI_SPIE
		SPCR &= ~(1<<SPIE);
		SPSR |= (1<<SPIF);
	#endif
}

#ifndef SPI_SPIE
	#ifndef SPI_MULTISLAVE
	/**
	 * @brief Control the SPI Slave Select (SS) pin to enable or disable the SPI slave device.
	 *
	 * @param mode Specifies the SPI select state, either SPI_Enable or SPI_Disable.
	 *
	 * @details
	 * This function manages the SPI Slave Select (SS) pin using the configured SPI port. When `SPI_Enable` is passed, the `SS` pin is driven low to select (activate) the SPI slave device. For any other value, the `SS` pin is driven high, deselecting (deactivating) the slave. This control is essential for ensuring that only one SPI slave device communicates with the master at any given time on a shared SPI bus.
	 *
	 * @note The SS pin behavior is active low; pulling it low selects the slave. Ensure that the pin configuration and port macros reflect your hardware's SPI setup.
	 */
    void spi_select(SPI_Select mode)
    {
	    switch(mode)
	    {
		    case SPI_Enable : SPI_PORT &= ~(1<<SPI_SS);   break;  // Slave Select On
		    default         : SPI_PORT |=  (1<<SPI_SS);   break;  // Slave Select Off
	    }
    }
	#endif

	/**
	 * @brief Transfer a single byte of data over the SPI bus.
	 *
	 * @param data The byte value to be sent via SPI.
	 *
	 * @return Returns the byte received simultaneously from the SPI slave device.
	 *
	 * @details
	 * This function writes the provided data byte to the SPI data register, initiating the SPI transmission. It then waits in a busy loop until the SPI interrupt flag indicates that the transfer is complete. Upon completion, the function reads and returns the received byte from the SPI data register. SPI communication is full-duplex, so while sending a byte, a byte is received simultaneously. This function therefore effectively performs a combined send/receive operation.
	 *
	 * @note This is a blocking call that waits until the SPI hardware signals transmission completion. Ensure SPI is properly initialized before calling this function.
	 */
    unsigned char spi_transfer(unsigned char data)
    {
	    SPDR = data;    // Write data into the SPI Data Register and initiate a transmission
	    
	    // Wait until transmission is Complete
	    while(!(SPSR & (1<<SPIF)))
		{
			asm volatile("NOP");
		}
	    return SPDR;
    }

#endif