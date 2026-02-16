
#ifndef PORT_H
#define PORT_H

#if defined (__AVR_ATmega32__)

/* For absolute pin numbering see mega32/port_mega32.h */
#include "mega32/port_mega32.h"

#elif defined (__AVR_ATmega328P__)

/* For absolute pin numbering see mega328p/port_mega328p.h */
#include "mega328p/port_mega328p.h"

#endif

#endif /* PORT_H */
