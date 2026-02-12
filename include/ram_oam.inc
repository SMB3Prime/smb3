.ifndef __RAM_OAM_INC__
__RAM_OAM_INC__ = 1

.segment "SHADOW_OAM"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $2xx SPRITE RAM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; \$02([0-9A-F][0-9A-F])
; Sprite_RAM+$\1

; Sprite memory is laid out in four bytes:
; Byte 0 - Stores the y-coordinate of the top left of the sprite minus 1.
; Byte 1 - Index number of the sprite in the pattern tables.
; Byte 2 - Stores the attributes of the sprite.
; * Bits 0-1 - Most significant two bits of the colour.
; * Bit 5 - Indicates whether this sprite has priority over the background.
; * Bit 6 - Indicates whether to flip the sprite horizontally.
; * Bit 7 - Indicates whether to flip the sprite vertically.
; Byte 3 - X coordinate
Sprite_RAM = OAM_BASE

.endif; include guard
