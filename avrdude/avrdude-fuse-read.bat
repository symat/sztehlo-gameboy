:: http://eleccelerator.com/fusecalc/fusecalc.php?chip=attiny85
::
:: - 8 MHz inner clock, no clock divide
:: - SPI enabled
:: - RESET enabled
:: - no memory lock


avrdude.exe -p t85 -c usbasp -P usb -B 1000 -U lfuse:r:-:h   -U hfuse:r:-:h   -U efuse:r:-:h   -U lock:r:-:h

@ECHO OFF

echo 
echo expected: lfuse: 0xE2
echo expected: hfuse: 0xDF
echo expected: efuse: 0xFF
echo expected: lock:  0xFF

