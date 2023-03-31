:: jp2: self-programming - short this on the target usbasp programmer, you want to upgrade
:: jp3: slow-clock
:: set 5V programmer for the source programmer


:: test connection (should be successfully connet the target usbasp programmer, if the self-programming fuse is set)

avrdude.exe -p atmega8 -c usbasp -P usb || exit /b



:: fuse overwrite

avrdude.exe -p atmega8 -c usbasp -P usb  -B 200 -U hfuse:w:0xC9:m -U lfuse:w:0xEF:m   || exit /b



:: flashing

avrdude.exe -p atmega8 -c usbasp -P usb  -B 1 -U flash:w:usbasp.atmega8.2011-05-28.hex   || exit /b
