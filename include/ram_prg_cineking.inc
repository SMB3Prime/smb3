.ifndef __RAM_PRG_CINEKING_INC__
__RAM_PRG_CINEKING_INC__ = 1

__PRGRAM_CINEKING_OFFSET__ .set $1A01
.macro PRGRAM_CINEKING_NOINC name
    name = PRGRAM_BASE + __PRGRAM_CINEKING_OFFSET__
.endmacro
.macro PRGRAM_CINEKING name, size
    .if (__PRGRAM_CINEKING_OFFSET__ + (size)) > $2000
        .error "PRGRAM overflow: name"
    .endif
    name = PRGRAM_BASE + __PRGRAM_CINEKING_OFFSET__
    __PRGRAM_CINEKING_OFFSET__ .set __PRGRAM_CINEKING_OFFSET__ + (size)
.endmacro

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; $7A01-$7A11 MMC3 SRAM as Cinematic for Wand Return (Post-Airship)
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; This uses the same space as most of the Auto Scroll data, I'm annoyed that I have to make a section for this

; After the wand is returned ONLY
    PRGRAM_CINEKING CineKing_WandState, 1   ; Wand state; 0 = falling, 1 = spinning, 2 = held
    PRGRAM_CINEKING CineKing_WandFrame, 1   ; Wand frame; 0 to 7
    PRGRAM_CINEKING CineKing_ToadFrame, 1   ; Toad's frame
    PRGRAM_CINEKING CineKing_DiagHi, 1   ; Text high address value

__PRGRAM_CINEKING_OFFSET__ .set __PRGRAM_CINEKING_OFFSET__ + 3

    PRGRAM_CINEKING CineKing_TimerT, 1   ; Cheering Toad animation Timer
    PRGRAM_CINEKING CineKing_Timer3, 1   ; Timer decremented every 4 ticks (does not appear to be used!)

__PRGRAM_CINEKING_OFFSET__ .set __PRGRAM_CINEKING_OFFSET__ + 2

    PRGRAM_CINEKING CineKing_WandX, 1   ; Wand X position
    PRGRAM_CINEKING CineKing_WandY, 1   ; Wand Y position
    PRGRAM_CINEKING CineKing_WandXVel, 1   ; Wand X velocity (4.4FP)
    PRGRAM_CINEKING CineKing_WandYVel, 1   ; Wand Y velocity (4.4FP)
    PRGRAM_CINEKING CineKing_WandXVel_Frac, 1   ; Wand X velocity fractional accumulator
    PRGRAM_CINEKING CineKing_WandYVel_Frac, 1   ; Wand Y velocity fractional accumulator

.endif; include guard