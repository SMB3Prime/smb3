.ifndef __IO_INC__
__IO_INC__ = 1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; OTHER I/O regs
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

SPR_DMA     = $4014 ; Sprite DMA Register -- DMA from CPU memory at $100 x n to SPR-RAM (256 bytes)


; Read / Write Joypad 1/2:
;                   *  Bit 0 - Reads data from joypad or causes joypad strobe
;                      when writing.
;                   *  Bit 3 - Indicates whether Zapper is pointing at a sprite.
;                   *  Bit 4 - Cleared when Zapper trigger is released.
;                   Only bit 0 is involved in writing.
JOYPAD      = $4016

; Frame counter control
; Changes the frame counter that changes updates on sound; any write resets
; the frame counter, good for synchronizing sound with VBlank etc.
; 0        4, 0,1,2,3, 0,1,2,3,..., etc.
; 1        0,1,2,3,4, 0,1,2,3,4,..., etc.
; bit 6 - enable frame IRQs (when zero)
; bit 7 - 0 = 60 IRQs a frame / 1 = 48 IRQs a frame (obviously need bit 6 clear to use)
; Interestingly, both of the above are clear on bootup, meaning IRQs are being generated,
; but the 6502 ignores NMIs on startup; also, need to read from $4015 (PAPU_EN) to acknowledge
; the interrupt, otherwise it holds the status on!
FRAMECTR_CTL    = $4017

.endif; include guard
