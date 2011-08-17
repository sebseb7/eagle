#ifndef RADIO_H_
#define RADIO_H_

#define CE_DDR  DDRA
#define CE_PORT PORTA
#define CE_PINN PINA6

#define CSN_DDR  DDRA
#define CSN_PORT PORTA
#define CSN_PINN PINA5

#define CSN_HIGH CSN_PORT |=  (1<<CSN_PINN);
#define CSN_LOW  CSN_PORT &= ~(1<<CSN_PINN);
#define CE_HIGH  CE_PORT |=  (1<<CE_PINN);
#define CE_LOW   CE_PORT &= ~(1<<CE_PINN);


void RadioInit(void);
void RadioSend(uint8_t keyState, uint8_t bat);


#endif /* RADIO_H_ */
