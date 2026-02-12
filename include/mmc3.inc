.ifndef __MMC3_INC__
__MMC3_INC__ = 1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; MMC3 regs
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; MMC3_COMMAND:
;  Bits 0-2 - Command number:
;  * 0 - Swap two 1 KB VROM banks at PPU $0000.
;  * 1 - Swap two 1 KB VROM banks at PPU $0800.
;  * 2 - Swap one 1 KB VROM bank at PPU $1000.
;  * 3 - Swap one 1 KB VROM bank at PPU $1400.
;  * 4 - Swap one 1 KB VROM bank at PPU $1800.
;  * 5 - Swap one 1 KB VROM bank at PPU $1C00.
;  * 6 - Swap PRG-ROM bank at either $8000 or $C000 based on bit 6.
;  * 7 - Swap PRG-ROM bank at either $A000
;
;  Bit 6 - If 0, enables swapping at $8000 and $A000, otherwise enables
;  swapping at $C000 and $A000.  (NOTE: This is what SMB3 uses, so we only
;  have defs for this mode!)
;
;  Bit 7 - If 1, causes addresses for commands 0-5 to be the exclusive-or
;  of the address stated and $1000.

; Note that bit 6 is set on all of these consistently since SMB3 uses the PRG switch this way
MMC3_2K_TO_PPU_0000 = %01000000 ; 0
MMC3_2K_TO_PPU_0800 = %01000001 ; 1
MMC3_1K_TO_PPU_1000 = %01000010 ; 2
MMC3_1K_TO_PPU_1400 = %01000011 ; 3
MMC3_1K_TO_PPU_1800 = %01000100 ; 4
MMC3_1K_TO_PPU_1C00 = %01000101 ; 5
MMC3_8K_TO_PRG_C000 = %01000110 ; 6
MMC3_8K_TO_PRG_A000 = %01000111 ; 7
MMC3_PPU_XOR_1000   = %10000000


MMC3_COMMAND    = $8000 ; consult ref
MMC3_PAGE   = $8001 ; page number to MMC3_COMMAND
MMC3_MIRROR = $A000 ; bit 0 clear is horizontal mirroring, bit 0 set is vertical mirroring
MMC3_SRAM_EN    = $A001 ; bit 7 set to enable SRAM at $6000-$7FFF
MMC3_IRQCNT = $C000 ; Countdown to an IRQ
MMC3_IRQLATCH   = $C001 ; Store a temp val to be copied to MMC3_IRQCNT later
MMC3_IRQDISABLE = $E000 ; Disables IRQ generation and copies MMC3_IRQLATCH to MMC3_IRQCNT
MMC3_IRQENABLE  = $E001 ; Enables IRQ generation

.endif; include guard
