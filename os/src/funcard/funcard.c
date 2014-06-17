#include <compat/deprecated.h>
#include <avr/eeprom.h>
#include <avr/io.h>

void hwinit( void )
{
	outb(ACSR ,0x80);
	outb(DDRA ,0xFF);
	outb(DDRB ,0xFF);
	outb(DDRC ,0xFF);
	outb(DDRD ,0xFF);
	outb(PORTA,0xFF);
	outb(PORTB,0xFF);
	outb(PORTC,0xFF);
	outb(PORTD,0xFF);
	return;
}

uint8_t readinternalmem(const uint8_t * addr)
{
  while(!eeprom_is_ready());

  return eeprom_read_byte(addr);
}

void writeinternalmem(uint8_t * addr, uint8_t byte)
{
  while(!eeprom_is_ready());

  return eeprom_write_byte(addr, byte);
}
