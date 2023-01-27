# Assumed to be used on Ubuntu with wine for asm6.exe compatibility.

all: smb3.nes

smb3.nes: smb3.asm PRG/* CHR/*
	wine asm6.exe smb3.asm
	mv smb3.bin smb3.nes


clean:
	rm smb3.nes 
