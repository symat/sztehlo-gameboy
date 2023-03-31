:: http://eleccelerator.com/fusecalc/fusecalc.php?chip=attiny85
::
:: - 8 MHz inner clock, no clock divide
:: - SPI enabled
:: - RESET enabled
:: - no memory lock

avrdude.exe -p t85 -c usbasp -P usb -B 1000 -U lfuse:w:0xE2:m -U hfuse:w:0xDF:m -U efuse:w:0xFF:m -U lock:w:0xFF:m