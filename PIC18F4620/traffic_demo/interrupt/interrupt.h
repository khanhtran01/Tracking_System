#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <p18f4620.h>

#define PORT_ENB	PORTA
#define TRIS_ENB	TRISA
void init_interrupt(void);
void init_ext_int0(void);
void low_isr(void);
void high_isr(void);


#endif