#define FALSE	0
#define TRUE	!FALSE

#ifdef __AVR__
#include <avr/pgmspace.h>
#define	CODE    __ATTR_PROGMEM__
#endif

typedef unsigned char bool;
