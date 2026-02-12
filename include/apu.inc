.ifndef __APU_INC__
__APU_INC__ = 1

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; SOUND I/O regs
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; $4000(rct1)/$4004(rct2)/$400C(noise) bits
; ---------------------------------------
; 0-3   volume / envelope decay rate
; 4 envelope decay disable
; 5 length counter clock disable / envelope decay looping enable
; 6-7   duty cycle type (unused on noise channel)

; Duty cycles:
; 00 = a weak, grainy tone.  (12.5% Duty), 01 = a solid mid-strength tone. (25% Duty),
; 10 = a strong, full tone (50% Duty), 11 = sounds a lot like 01 (25% Duty negated)

PAPU_CTL1   = $4000 ; pAPU Pulse 1 Control Register.
PAPU_CTL2   = $4004 ; pAPU Pulse 2 Control Register.
PAPU_NCTL1  = $400C ; pAPU Noise Control Register 1.


; $4008(tri) bits
; ---------------
; 0-6   linear counter load register
; 7 length counter clock disable / linear counter start
PAPU_TCR1   = $4008 ; pAPU Triangle Control Register 1.


; $4001(rct1)/$4005(rct2) bits
; --------------------------
; 0-2   right shift amount
; 3 decrease / increase (1/0) wavelength
; 4-6   sweep update rate
; 7 sweep enable
PAPU_RAMP1  = $4001 ; pAPU Pulse 1 Ramp Control Register.
PAPU_RAMP2  = $4005 ; pAPU Pulse 2 Ramp Control Register.


; $4002(rct1)/$4006(rct2)/$400A(Tri) bits
; -------------------------------------
; 0-7   8 LSB of wavelength
PAPU_FT1    = $4002 ; pAPU Pulse 1 Fine Tune (FT) Register.
PAPU_FT2    = $4006 ; pAPU Pulse 2 Fine Tune (FT) Register.
PAPU_TFREQ1 = $400A ; pAPU Triangle Frequency Register 1.


; $400E(noise) bits
; -----------------
; 0-3   playback sample rate
; 4-6   unused
; 7 random number type generation
PAPU_NFREQ1 = $400E ; pAPU Noise Frequency Register 1.

; $4003(rct1)/$4007(rct2)/$400B(tri)/$400F(noise) bits
; --------------------------------------------------
; 0-2   3 MS bits of wavelength (unused on noise channel) (the "high" frequency)
; 3-7   length of tone
PAPU_CT1    = $4003 ; pAPU Pulse 1 Coarse Tune (CT) Register.
PAPU_CT2    = $4007 ; pAPU Pulse 2 Coarse Tune (CT) Register.
PAPU_TFREQ2 = $400B ; pAPU Triangle Frequency Register 2.
PAPU_NFREQ2 = $400F ; pAPU Noise Frequency Register 2.


; $4010 - DMC Play mode and DMA frequency

; Bits 0-3:
;    f   period
;    ----------
;    0   $1AC
;    1   $17C
;    2   $154
;    3   $140
;    4   $11E
;    5   $0FE
;    6   $0E2
;    7   $0D6
;    8   $0BE
;    9   $0A0
;    A   $08E
;    B   $080
;    C   $06A
;    D   $054
;    E   $048
;    F   $036
; Bits 6-7: this is the playback mode.
;   00 - play DMC sample until length counter reaches 0 (see $4013)
;   x1 - loop the DMC sample (x = immaterial)
;   10 - play DMC sample until length counter reaches 0, then generate a CPU
PAPU_MODCTL = $4010 ; pAPU Delta Modulation Control Register.

PAPU_MODDA  = $4011 ; pAPU Delta Modulation D/A Register.
PAPU_MODADDR    = $4012 ; pAPU Delta Modulation Address Register.
PAPU_MODLEN = $4013 ; pAPU Delta Modulation Data Length Register.

; read
; ----
; 0 rectangle wave channel 1 length counter status
; 1 rectangle wave channel 2 length counter status
; 2 triangle wave channel length counter status
; 3 noise channel length counter status
; 4 DMC is currently enabled (playing a stream of samples)
; 5 unknown
; 6 frame IRQ status (active when set)
; 7 DMC's IRQ status (active when set)
;
; write
; -----
; 0 rectangle wave channel 1 enable
; 1 rectangle wave channel 2 enable
; 2 triangle wave channel enable
; 3 noise channel enable
; 4 enable/disable DMC (1=start/continue playing a sample;0=stop playing)
; 5-7   unknown
PAPU_EN     = $4015 ; R/W pAPU Sound Enable

.endif; include guard
