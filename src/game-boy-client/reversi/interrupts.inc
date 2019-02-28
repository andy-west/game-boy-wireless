; $0040 - $0067: Interrupt handlers
jp draw

REPT 5
    nop
ENDR

; $0048
jp stat

REPT 5
    nop
ENDR

; $0050
jp timer

REPT 5
    nop
ENDR

; $0058
jp serial

REPT 5
    nop
ENDR

; $0060
jp joypad

REPT 5
    nop
ENDR
