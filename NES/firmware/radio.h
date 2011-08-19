/*
 * radio.h
 *
 *  Created on: 24-Jan-2009
 *      Author: Neil MacMillan
 *
 *  References:
 *    Ball, Brennen.  DIYEmbedded Tutorials.  http://www.diyembedded.com/
 *    Engelke, Stefan.  AVR-Lib/nRF24L01.  http://www.tinkerer.eu/AVRLib/nRF24L01
 *    Nordic Semiconductor.  nRF24L01 Product Specification 2.0.  http://www.nordicsemi.no/files/Product/data_sheet/nRF24L01_Product_Specification_v2_0.pdf
 *
 *    Most of the hard work for this was done by [Engelke].
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <stddef.h>
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"
#include "nRF24L01.h"
#include "packet.h"
#include "spi.h"

#define RADIO_ADDRESS_LENGTH 5

typedef enum _radio_pipe {
	RADIO_PIPE_0 = 0,
	RADIO_PIPE_1 = 1,
	RADIO_PIPE_2 = 2,
	RADIO_PIPE_3 = 3,
	RADIO_PIPE_4 = 4,
	RADIO_PIPE_5 = 5,
	RADIO_PIPE_EMPTY = 7,	// FIFO is empty when pipe number bits in status register are 0b111.
} RADIO_PIPE;

typedef enum _radio_tx_power {
	RADIO_LOWEST_POWER = 0,		// -18 dBm (about 16 uW)
	RADIO_LOW_POWER = 1,		// -12 dBm (about 63 uW)
	RADIO_HIGH_POWER = 2,		// -6 dBm (about 251 uW)
	RADIO_HIGHEST_POWER = 3,	// 0 dBm (1 mW)
} RADIO_TX_POWER;

typedef enum _radio_dr {
	RADIO_1MBPS = 0,		// that's Mbps, not MBps.
	RADIO_2MBPS = 1,
} RADIO_DATA_RATE;

typedef enum _radio_receive {
	RADIO_RX_INVALID_ARGS,		// one of the arguments to Radio_Receive was invalid
	RADIO_RX_TRANSMITTING,		// the radio was transmitting
	RADIO_RX_FIFO_EMPTY,		// there aren't any packets in the Rx FIFO to receive (Radio_Receive does not receive data)
	RADIO_RX_MORE_PACKETS,		// after copying out the head of the Rx FIFO, there is still another packet in the FIFO.
	RADIO_RX_SUCCESS,			// there was a packet to receive, it was successfully received, and the Rx FIFO is now empty.
} RADIO_RX_STATUS;

typedef enum _radio_transmit {
	RADIO_TX_MAX_RT,
	RADIO_TX_SUCCESS,
} RADIO_TX_STATUS;

typedef enum _radio_tx_wait {
	RADIO_WAIT_FOR_TX,
	RADIO_RETURN_ON_TX,
} RADIO_TX_WAIT;

typedef enum _radio_ack {
	RADIO_ACK,
	RADIO_NO_ACK,
} RADIO_USE_ACK;

typedef enum _ed {
	DISABLE=0,
	ENABLE=1,
} ON_OFF;		// there's got to be a better name for this.

void Radio_Init(void);


/**
 * Configure one of the radio's six Rx pipes.
 * This configures a pipe's address and enables or disables the pipe.  Pipes 0 and 1 are enabled by default, and pipes 2-5 are
 * 		disabled by default.  The configuration for pipe 0 will be changed while the radio is transmitting, to facilitate auto-
 * 		ack.  It will be changed back when the transmission is completed.
 * \param pipe The pipe to configure.
 * \param address The 1- or 5-byte address to give the pipe.  For pipes 0 and 1 all five bytes can be different, but
 * 		pipes 2-5 share the four most significant bytes of pipe 1's address.  The LSB of each pipe's address must be unique.
 * 		For example:
 * 				Pipe 0: 0x0123456789
 * 				Pipe 1:	0x9876543210
 * 				Pipe 2: 0x98765432AB
 * 				Pipe 3: 0x98765432BC
 * 				Pipe 4: 0x98765432CD
 * 				Pipe 5: 0x98765432DE
 * 		If pipe 0 or 1 is being configured, then address must be a 5-byte array.  If the other four pipes are being configured,
 * 		then the first byte of address is used as the LSB of the pipe's address (i.e. you only pass a 1-byte address, with the
 * 		four MSBytes of the pipe's address left implied).  For example, this will set the first four pipe addresses above:
 * 				uint8_t address[5] = {0x01, 0x23, 0x45, 0x67, 0x89};
 * 				Radio_Configure_Rx(RADIO_PIPE_0, address, ENABLE);
 * 				address = {0x98, 0x76, 0x54, 0x32, 0x10};
 * 				Radio_Configure_Rx(RADIO_PIPE_1, address, ENABLE);
 * 				address[0] = 0xAB;
 * 				Radio_Configure_Rx(RADIO_PIPE_2, address, ENABLE);
 * 				address[0] = 0xBC;
 * 				Radio_Configure_Rx(RADIO_PIPE_3, address, ENABLE);
 * 				...
 * \param enable Enable or disable the pipe.
 */
void Radio_Configure_Rx(RADIO_PIPE pipe, uint8_t* address, ON_OFF enable);

/**
 * Configure the radio transceiver.
 * \param dr The data rate at which the radio will transmit and receive data (1 Mbps or 2 Mbps).
 * \param power The transmitter's power output.
 */
void Radio_Configure(RADIO_DATA_RATE dr, RADIO_TX_POWER power);

/**
 * Set the radio transmitter's address.
 * \param The 5-byte address that packets will be sent to.
 */
void Radio_Set_Tx_Addr(uint8_t* address);

/**
 * Transmit some data to another station.
 * \param payload The data packet to transmit.
 * \param wait If this is RADIO_WAIT, then Radio_Transmit will not return until the transmission is completed
 * 		successfully or fails.  If this is RADIO_DO_NOT_WAIT then Radio_Transmit will return as soon as the
 * 		payload is loaded onto the radio and the transmission is started (in this case, Radio_Is_Transmitting
 * 		can be used to determine if the radio is busy).
 * \return If wait was RADIO_DO_NOT_WAIT, then Radio_Transmit always returns RADIO_TX_SUCCESS.  If wait was
 * 		RADIO_WAIT and the transmission was successful (TX_DS interrupt asserted, i.e. the ack packet was
 * 		received), Radio_Transmit returns RADIO_TX_SUCCESS.  If wait was RADIO_WAIT and the transmission failed
 * 		(MAX_RT interrupt asserted, i.e. no ack was received and the maximum number of retries were sent), then
 * 		Radio_Transmit returns RADIO_TX_MAX_RT.
 */
uint8_t Radio_Transmit(radiopacket_t* payload, RADIO_TX_WAIT wait);

/**
 * Get the next packet from the Rx FIFO.
 * \param payload If there is a packet to copy out of the Rx FIFO, then its payload will be placed in this structure.
 * 		If the FIFO is empty, then this structure will be left alone.
 * \return See enum RADIO_RX_STATUS for values.
 */
RADIO_RX_STATUS Radio_Receive(radiopacket_t* buffer);

/**
 * Calculate the radio's transmit success rate over the last 16 packets.  The return value is the percentage of packets
 * that were transmitted successfully, ranging from 0 to 100.
 */
uint8_t Radio_Success_Rate(void);

/**
 * Flush the radio's Rx and Tx FIFOs.
 */
void Radio_Flush(void);

void inthandl(void);

#endif /* RADIO_H_ */
