/*
 * twiSlave.h
 *
 *  Created on: 02.01.2022
 *      Author: ManCloud
 */

#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

/* === INCLUDES ============================================================= */
#include <util/twi.h>
#include <avr/interrupt.h>
#include <global.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>  //<- for memmove


/* === TYPES ================================================================ */

/* === MACROS =============================================================== */
#define TWI_ADDR 0x29

#define TWCR_ACK TWCR = _BV(TWEN)|_BV(TWIE)|_BV(TWINT)|_BV(TWEA);
#define TWCR_NACK TWCR = _BV(TWEN)|_BV(TWIE)|_BV(TWINT);
#define TWCR_RESET TWCR = _BV(TWEN)|_BV(TWIE)|_BV(TWINT)|_BV(TWEA)|_BV(TWSTO);

/* === GLOBALS ============================================================== */

/* === PROTOTYPES =========================================================== */
void twi_init(void);
void twi_stop(void);
bool twi_available(void);
uint16_t twi_read(void);
void twi_flush(void);

ISR(TWI_vect);

#endif
