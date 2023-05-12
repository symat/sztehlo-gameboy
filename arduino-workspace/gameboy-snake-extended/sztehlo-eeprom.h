// Egyszerű segéd függvények EEPROM írás / olvasáshoz

// Az ATTiny85-nek 512 bytes EEPROM-ja van, ezekkel a függvényekkel
// írhatjuk/olvashatjuk az első 256 byte-ot

// A 255. byte mindíg egy "program azonosító". Ha az első byte-on nem 
// a megadott azonosító szám szerepel, akkor az eepromInit() függvény 
// letörli az eeprom-ot. Ha minden programunknál más azonosító-t használunk,
// akkor így elkerülhetjük, hogy érvénytelen adatot olvassunk ki.

#ifndef __SZTEHLO_GAMEBOY_EEPROM__
#define __SZTEHLO_GAMEBOY_EEPROM__


uint8_t eepromRead(uint8_t address) {
  /* Wait for completion of previous write */
  while(EECR & (1<<EEPE))   ;
  /* Set up address register */
  EEAR = address;
  /* Start eeprom read by writing EERE */
  EECR |= (1<<EERE);
  /* Return data from data register */ 
  return EEDR;
}



void eepromUpdate(uint8_t address, uint8_t data) {
  if(eepromRead(address) == data) {
    return;
  }
  
  /* Wait for completion of previous write */ 
  while(EECR & (1<<EEPE));

  /* Set Programming mode */
  EECR = (0<<EEPM1)|(0<<EEPM0);

  /* Set up address and data registers */ 
  EEAR = address;
  EEDR = data;

  /* Write logical one to EEMPE */
  EECR |= (1<<EEMPE);
  
  /* Start eeprom write by setting EEPE */ 
  EECR |= (1<<EEPE);
}

uint8_t eepromInit(uint8_t eeprom_program_id) {
  if(eepromRead(255) != eeprom_program_id) {
    for(uint8_t i=0; i<255; i++) {
      eepromUpdate(i, 0x00);
    }
    eepromUpdate(255, eeprom_program_id);
  }
}


#endif // __SZTEHLO_GAMEBOY_EEPROM__