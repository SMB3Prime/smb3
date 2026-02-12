.segment "INESHDR"
    .byte "NES", $1a ;identification of the iNES header
    .byte 16 ;number of 16KB PRG-ROM pages
    .byte 16 ;number of 8KB CHR-ROM pages
    .byte $40;mapper 4 and mirroring
    .res 9   ;clear the remaining bytes
