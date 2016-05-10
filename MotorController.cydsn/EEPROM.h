
#ifndef EEPROM_H
#define EEPROM_H

#include <project.h>

char EEPROM_read(uint8 id, uint8 * data);
char EEPROM_write(uint8 id, const uint8 * data);
char EEPROM_init(const size_t * types, uint8 count);

#endif

