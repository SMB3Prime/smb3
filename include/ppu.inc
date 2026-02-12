.ifndef __PPU_INC__
__PPU_INC__ = 1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; PPU I/O regs (CPU side)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;   PPU_CTL1:
;   0-1: Name table address, changes between the four name tables at $2000 (0), $2400 (1), $2800 (2) and $2C00 (3).
;   2: Clear, PPU incs by 1 ("horizontal"); Set, PPU incs by 32 ("vertical")
;   3: Which pattern table holds for sprites; 0 for PT1 ($0000) or 1 for PT2 ($1000)
;   4: Which pattern table holds for BG; 0 for PT1 ($0000) or 1 for PT2 ($1000)
;   5: Set to use 8x16 sprites instead of 8x8
;   7: Set to generate VBlank NMIs
PPU_CTL1    = $2000     ; Write only

;   PPU_CTL2:
;   0: Clear for color, set for mono
;   1: Clear to clip 8 left pixels of BG
;   2: Clear to clip 8 left pixels of sprites
;   3: If clear, BG hidden
;   4: If clear, sprites hidden
;   5-7: BG color in mono mode, "color intensity" in color mode (??)
PPU_CTL2    = $2001     ; Write only

;   PPU_STAT:
;   4: if set, can write to VRAM, else writes ignored
;   5: if set, sprite overflow occurred on scanline
;   6: Set if any non-transparent pixel of sprite 0 is overlapping a non-transparent pixel of BG
;   7: VBlank is occurring (cleared after read)
PPU_STAT    = $2002

; Sprites: 256 bytes, each sprite takes 4, so 64 sprites total
; Only 8 sprites per scanline, sprite 0 is drawn on top (thus highest priority)
; PPU_SPR_ADDR / PPU_SPR_DATA
; * Byte 0 - Stores the y-coordinate of the top left of the sprite minus 1.
; * Byte 1 - Index number of the sprite in the pattern tables.
; * Byte 2 - Stores the attributes of the sprite.
;   * Bits 0-1 - Most significant two bits of the colour.  (Or "palette" 0-3)
;   * Bit 5 - Indicates whether this sprite has priority over the background.
;   * Bit 6 - Indicates whether to flip the sprite horizontally.
;   * Bit 7 - Indicates whether to flip the sprite vertically.
; * Byte 3 - X coordinate
PPU_SPR_ADDR    = $2003     ; Set address sprite data
PPU_SPR_DATA    = $2004     ; Read or write this sprite byte

PPU_SCROLL  = $2005     ; Scroll register; read PPU_STAT, then write horiz/vert scroll
PPU_VRAM_ADDR   = $2006     ; VRAM address (first write is high, next write is low)
PPU_VRAM_DATA   = $2007     ; Data to read/write at this address

; Note that all transparent colors ($3F04, $3F08, $3F0C, $3F10, $3F14, $3F18 and $3F1C) are mirrored from 3F00
PPU_BG_PAL  = $3F00     ; 3F00-3F0F
PPU_SPR_PAL = $3F10     ; 3F10-3F1F

.endif; include guard
