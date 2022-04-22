#ifndef DEBUG_H_
#define DEBUG_H_

#if DEBUG	/* to be defined in Makefile */
#include <stdio.h>

#define BAUD 9600UL      // Baudrate

// Berechnungen
#define UBRR_VAL 		((F_CPU+BAUD*8)/(BAUD*16)-1)   	// clever runden
#define UBRR_VAL_U2X 	((F_CPU+BAUD*4)/(BAUD*8)-1)   	// clever runden
#define BAUD_REAL 		(F_CPU/(16*(UBRR_VAL+1)))     	// Reale Baudrate
#define BAUD_REAL_U2X 	(F_CPU/(8*(UBRR_VAL_U2X+1)))   // Reale Baudrate
#define BAUD_ERROR 		((BAUD_REAL*1000)/BAUD) 		// Fehler in Promille, 1000 = kein Fehler.
#define BAUD_ERROR_U2X 	((BAUD_REAL_U2X*1000)/BAUD) 	// Fehler in Promille, 1000 = kein Fehler.

#if ((BAUD_ERROR > 1000) && (BAUD_ERROR < BAUD_ERROR_U2X)) || ((BAUD_ERROR < 1000) && (BAUD_ERROR > BAUD_ERROR_U2X))
#define USED_UBRR UBRR_VAL
#define USED_ERROR BAUD_ERROR
#define USE_2X 0
#else
#define USED_UBRR UBRR_VAL_U2X
#define USED_ERROR BAUD_ERROR_U2X
#define USE_2X 1
#endif


#if ((USED_ERROR<975) || (USED_ERROR>1025))
  #error Systematischer Fehler der Baudrate grösser 1% und damit zu hoch!
#endif

int uart_putchar(char c, FILE *stream) {
    while (!(UCSR0A & (1<<UDRE0))); /* warten bis Senden moeglich */
    UDR0 = c;                      	/* sende Zeichen */
    return 0;
}

void init_uart(void)
{
  UBRR0 = USED_UBRR;
  UCSR0B |= (1<<TXEN0);
  // Frame Format: Asynchron 8N1
  UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
#if USE_2X
   /* U2X-Modus erforderlich */
   UCSR0A |= (1 << U2X0);
#else
   /* U2X-Modus nicht erforderlich */
   UCSR0A &= ~(1 << U2X0);
#endif

   static FILE str_uart = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
   stdout = &str_uart;

   printf("init_uart done!\n");
}



#else

#define printf(...)

void init_uart(void){}

#endif

#endif
