/*
 * nRF24L01.h
 *
 *  Created on: 26-Jan-2009
 *      Author: Neil MacMillan
 *
 *  Constants for numerical values used to interact with the nRF24L01 radio.
 *
 *  Modified from here: http://www.tinkerer.eu/AVRLib/nRF24L01
 *
 *  See nRF24L01 Product Specification 2.0 section 9 (p 53) for detailed descriptions of registers.
 *  http://www.nordicsemi.no/files/Product/data_sheet/nRF24L01_Product_Specification_v2_0.pdf
 *
 *  The DIYEmbedded tutorial 0 reproduces the product spec tables and adds useful commentary on the registers.
 *  http://www.diyembedded.com/
 */

#ifndef NRF24L01_H_
#define NRF24L01_H_

/// This should be &ed with the register values defined below when they are masked into the R_REGISTER and W_REGISTER
/// instructions, to make sure that they do not overwrite the 3 MSBits in those instructions.
#define REGISTER_MASK 0x1F

typedef uint8_t radio_register_t;


/*****											Registers										*****/

/// Configure interrupts, CRC, power, and Tx/Rx status
#define CONFIG      0x00
/// Enhanced Shockburst (auto-ack) on the Rx pipes.
#define EN_AA       0x01
/// Enable or disable the Rx pipes
#define EN_RXADDR   0x02
/// Set address width (must be the same on all radios)
#define SETUP_AW    0x03
/// Set retry delay and number of retries for pipes using Enhanced Shockburst
#define SETUP_RETR  0x04
/// Set the channel to use, from the 2.4 GHz ISM band.
#define RF_CH       0x05
/// Setup radio data rate, output power, and LNA gain
#define RF_SETUP    0x06
/// Interrupt status, Tx FIFO full, and number of the Rx pipe that received a packet.
#define STATUS      0x07
/// Count lost and resent packets
#define OBSERVE_TX  0x08
/// Carrier detect (LSB is 1 if the receiver detects an in-band signal for at least 128 us)
#define CD          0x09
/// Receive address bytes (see documentation for explanation of how they fit 6 5-byte addresses
/// into 40 bits).  P0 is also used for auto-ack handling.
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
/// Transmit destination address.
#define TX_ADDR     0x10
/// Payload data width for each of the Rx pipes (0x01 bytes to 0x20 bytes, or 0x00 if pipe is not used)
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
/// Auto-retransmit status (cf. REUSE_TX_PL instruction), Tx FIFO full/empty, Rx FIFO full/empty
/// (The Rx FIFO is a 3-packet queue shared by all six pipes)
#define FIFO_STATUS 0x17


/*****							Register Bit Mask Shift Values (use with _BV())							*****/

/// CONFIG Register
/// 0 - Rx data-ready interrupt is sent to IRQ pin.  1 - Interrupt is not sent to IRQ pin.
#define MASK_RX_DR  6
/// 0 - Tx data-sent interrupt is sent to IRQ pin.  1 - Interrupt is not sent to IRQ pin.
#define MASK_TX_DS  5
/// 0 - Max-retries-reached interrupt is sent to IRQ pin.  1 - Interrupt is not sent to IRQ pin.
#define MASK_MAX_RT 4
/// 0 - Disable automatic CRC.  1 - Enable automatic CRC.
#define EN_CRC      3
/// 0 - Use 1-byte CRC.  1 - Use 2-byte CRC.
#define CRCO        2
/// 0 - Power down the radio.  1 - Power up the radio
#define PWR_UP      1
/// 0 - Radio is a transmitter.  1 - Radio is a receiver.
#define PRIM_RX     0

/// EN_AA Register
/// 0 - Disable Enhanced Shockburst on Rx pipe 5.  1 - Enable Enhanced Shockburst on Rx pipe 5.
#define ENAA_P5     5
/// 0 - Disable Enhanced Shockburst on Rx pipe 4.  1 - Enable Enhanced Shockburst on Rx pipe 4.
#define ENAA_P4     4
/// 0 - Disable Enhanced Shockburst on Rx pipe 3.  1 - Enable Enhanced Shockburst on Rx pipe 3.
#define ENAA_P3     3
/// 0 - Disable Enhanced Shockburst on Rx pipe 2.  1 - Enable Enhanced Shockburst on Rx pipe 2.
#define ENAA_P2     2
/// 0 - Disable Enhanced Shockburst on Rx pipe 1.  1 - Enable Enhanced Shockburst on Rx pipe 1.
#define ENAA_P1     1
/// 0 - Disable Enhanced Shockburst on Rx pipe 0.  1 - Enable Enhanced Shockburst on Rx pipe 0.
#define ENAA_P0     0

/// EN_RXADDR Register
/// 0 - Disable Rx pipe 5.  1 - Enable Rx pipe 5.
#define ERX_P5      5
/// 0 - Disable Rx pipe 4.  1 - Enable Rx pipe 4.
#define ERX_P4      4
/// 0 - Disable Rx pipe 3.  1 - Enable Rx pipe 3.
#define ERX_P3      3
/// 0 - Disable Rx pipe 2.  1 - Enable Rx pipe 2.
#define ERX_P2      2
/// 0 - Disable Rx pipe 1.  1 - Enable Rx pipe 1.
#define ERX_P1      1
/// 0 - Disable Rx pipe 0.  1 - Enable Rx pipe 0.
#define ERX_P0      0

/// SETUP_AW Register
/// Address width (bits 1:0)
/// 01 - 3 bytes.  10 - 4 bytes.  11 - 5 bytes.
#define AW          0

/// SETUP_RETR Register
/// Autoretransmit delay (bits 7:4):
///		0000 - Wait 250+86 us.
///		0001 - Wait 500+86 us.
///		0010 - Wait 750+86 us.
///		...
///		1111 - Wait 4000+86 us.
#define ARD         4
/// Autoretransmit count (bits 3:0)
///		0000 - Retransmit disabled.
///		0001 - Up to 1 retransmit on fail of auto-ack.
///		0010 - Up to 2 retransmits.
///		...
///		1111 - Up to 15 retransmits.
#define ARC         0

/// We skip the RF_CH register, because it's pretty straightforward.  Load the channel number into bits 6:0.

/// RF_SETUP Register
/// Force PLL lock signal.  See http://en.wikipedia.org/wiki/Phase-locked_loop.  This shouldn't be set.
#define PLL_LOCK    4
/// 0 - On-air data rate is 1 Mbps.  1 - On-air data rate is 2 Mbps.
#define RF_DR       3
/// Radio Tx Power (bits 2:1)
/// 00 - -18 dBm.  01 - -12 dBm.  10 - -6 dBm.  11 - 0 dBm.  dBm is decibels relative to 1 mW.
#define RF_PWR      1
/// Low-noise amplifier gain.  This shouldn't be cleared.
#define LNA_HCURR   0

/// STATUS Register
/// 0 - Rx data ready interrupt was not triggered.  1 - Rx data ready interrupt was triggered.  (write 1 to clear after interrupt)
#define RX_DR       6
/// 0 - Tx data sent interrupt was not triggered.  1 - Tx data sent interrupt was triggered.  (write 1 to clear after interrupt)
/// If Enhanced Shockburst is enabled, the interrupt is triggered once the transmitter receives the ack.
#define TX_DS       5
/// 0 - Max retries sent interrupt was not triggered.  1 - Max retries sent interrupt was triggered.  (write 1 to clear after interrupt)
/// If the MAX_RT interrupt is triggered, this needs to be cleared before the radio can be used again.
#define MAX_RT      4
/// Number of the data pipe that just received data (bits 3:1)
/// 000 to 101 - pipe number 0-5.  110 not used.  111 all FIFOs are empty.
#define RX_P_NO     1
/// 0 - There are available locations in the Tx FIFO.  1 - The Tx FIFO is full.
#define TX_FULL     0

/// OBSERVE_TX Register
/// Lost packet count (bits 7:4)
/// Counts up to 15 lost packets (does not overflow).  Reset by writing to RF_CH.
#define PLOS_CNT    4
/// Resent packet count (bits 3:0)
/// Counts the packets that were re-sent in Enhanced Shockburst mode.  Reset by sending a new packet.
#define ARC_CNT     0

/// We skip the CD register, because it just has 1 bit.  0 - No carrier detected.  1 - Carrier detected.

/// We also skip the RX_ADDR, TX_ADDR, and RX_PW registers because they don't use bit shifting.

/// FIFO_STATUS Register
/// 0 - The Tx payload is not being re-sent.  1 - The Tx payload is being resent (see REUSE_TX_PL instruction below).
#define TX_REUSE    6
/// 0 - The Tx FIFO is not full.  1 - The Tx FIFO is full.  Note that the FIFOs are three packets deep, so "not full" != "empty"
#define TX_FIFO_FULL   5
/// 0 - The Tx FIFO is not empty.  1 - The Tx FIFO is empty.
#define TX_FIFO_EMPTY    4
/// 0 - The Rx FIFO is not full.  1 - The Rx FIFO is full.
#define RX_FIFO_FULL     1
/// 0 - The Rx FIFO is not empty.  1 - The Rx FIFO is empty.
#define RX_FIFO_EMPTY    0

/*****										Instructions										*****/
/// Read Register
#define R_REGISTER    0x00
/// Write Register
#define W_REGISTER    0x20
/// Read receive payload (clears FIFO; LSByte first)
#define R_RX_PAYLOAD  0x61
/// Write transmit payload
#define W_TX_PAYLOAD  0xA0
/// Flush transmit FIFO
#define FLUSH_TX      0xE1
/// Flush receive FIFO (should not be used while an ack is being transmitted)
#define FLUSH_RX      0xE2
/// Reuse transmit payload.  Use this to continuously retransmit the payload (in Tx mode) as long as CE is held high, until
/// the Tx FIFO is flushed or the payload is overwritten.  This should not be changed while transmitting.  This is different
/// from Enhanced Shockburst.
#define REUSE_TX_PL   0xE3
/// No operation.
#define NOP           0xFF

#endif /* NRF24L01_H_ */
