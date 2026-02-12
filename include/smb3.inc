.ifndef __SMB3_INC__
__SMB3_INC__ = 1

.include "constants.inc"
.include "ram.inc"
.include "ppu.inc"
.include "apu.inc"
.include "mmc3.inc"
.include "io.inc"


.macro BYTE_INDEX_TO target, base
    .assert ((target)-(base)-1) >= -1, error, "Index underflow"
    .assert ((target)-(base)-1) <= $FF, error, "Index overflow"
    .byte <((target)-(base)-1)
.endmacro

; Handy pseudo instructions... only make sense in the context of CMPing a number...
.macro BLT _1
    BCC _1  ; A < CMP (unsigned)
    .endmacro

.macro BGE _1
    BCS _1  ; A >= CMP (unsigned)
    .endmacro

.macro BLS _1
    BMI _1  ; A < CMP (signed)
    .endmacro

.macro BGS _1
    BPL _1  ; A >= CMP (signed)
    .endmacro

.macro NEG  ; RegEx S&R "EOR #\$ff.*\n.*ADD #\$01" -> "NEG"
    EOR #$ff
    CLC
    ADC #$01
    .endmacro

.macro ABS_LDA _1
    .byte $AD, <_1, $00
    .endmacro

.macro ABS_LDA_X _1
    .byte $BD, <_1, $00
    .endmacro

.macro ABS_STA _1
    .byte $8D, <_1, $00
    .endmacro

.macro ABS_STA_X _1
    .byte $9D, <_1, $00
    .endmacro

.macro ABS_LDY _1
    .byte $AC, <_1, $00
    .endmacro

.macro ABS_LDY_X _1
    .byte $BC, <_1, $00
    .endmacro

.macro ABS_STY _1
    .byte $8C, <_1, $00
    .endmacro

.macro ABS_LDX _1
    .byte $AE, <_1, $00
    .endmacro

.macro ABS_STX _1
    .byte $8E, <_1, $00
    .endmacro

.macro ABS_SBC _1
    .byte $ED, <_1, $00
    .endmacro

.macro ABS_SBC_X _1
    .byte $FD, <_1, $00
    .endmacro

.macro ABS_ADC _1
    .byte $6D, <_1, $00
    .endmacro

.macro ABS_ADC_X _1
    .byte $7D, <_1, $00
    .endmacro

.macro ABS_INC _1
    .byte $EE, <_1, $00
    .endmacro

.macro ABS_INC_X _1
    .byte $FE, <_1, $00
    .endmacro

.macro ABS_ORA _1
    .byte $0D, <_1, $00
    .endmacro

; This is used in video update streams; since the video address register
; takes the address high-then-low (contrary to 6502's normal low-then-high),
; this allows a 16-bit value but "corrects" it to the proper endianness.
.macro DBYT _1
    .byte >_1, <_1
    .endmacro

; Pads bytes to align to nearest 64 byte boundary for DMC samples
; SB: This would be useful for your own works, but I can't use
; it in the natively disassembly since the assembler pads zeroes
; instead of $FF values... just FYI!
;
; Usage example:
;
; DMCAlign
.macro DMCAlign
    .res (((* + $3F) & $FFC0) - *)
    .endmacro

.endif; include guard
