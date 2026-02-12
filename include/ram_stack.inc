.ifndef __RAM_STACK_INC__
__RAM_STACK_INC__ = 1

.segment "STACK"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $1xx LOW STACK VARIABLES
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; These are actually "bottom" of the stack; I don't think this is normally recommended.
; but this is probably to keep them "safe" from other RAM clearing routines, since they directly effect the IRQ.
; The debug flag in particular is pretty precariously placed, and under some kind of heavy call stack, seems like
; there's risk it could be set by accident... but I guess this never happens... ?

__STACK_OFFSET__ .set $0
.macro STK name, size
    name = STACK_BASE + __STACK_OFFSET__
    __STACK_OFFSET__ .set __STACK_OFFSET__ + (size)
.endmacro

    STK Update_Select, 1       ; Changes which path of "update routines" are selected; $00 = ??, $20 = Title Screen, $40 = Spade Game, $80 = Vertical level, $A0 = 32 pixel partition, $C0 = Normal

    STK Raster_Effect, 1       ; $00 is standard status bar, $20 is title/ending, $40 = 32 pixel partition, $60 = Spade Bonus Game (3 sliding rows), $80 is nothing (e.g. as in 2P versus), $A0 = ???

    __STACK_OFFSET__ .set $60
    STK Debug_Flag, 1       ; Set to $80 by the debug menu, enables debug functionality like power level cycling and not dying from time over

.endif; include guard