memcpy:
    ; DE = bytes to copy
    ; BC = source address
    ; HL = destination address

.copyByte:
    ld A, [BC]
    ld [HL], A
    inc BC
    inc HL
    dec DE

    ld A, E
    cp 0
    jr nz, .copyByte
    ld A, D
    cp 0
    jr nz, .copyByte

    ret

memset:
    ; DE = bytes to clear
    ; B = value to set
    ; HL = address

.copyByte:
    ld [HL], B
    inc HL
    dec DE

    ld A, E
    cp $00
    jr nz, .copyByte
    ld A, D
    cp $00
    jr nz, .copyByte

    ret
