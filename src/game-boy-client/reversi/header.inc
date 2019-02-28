; $0100 - $0103: Entry point
nop
jp main

; $0104 - $0133: Nintendo logo
DS 48

; $0134 - $013E: Upper-case title followed by zeroes. Maximum 11 characters.
DB "REVERSI"
DS 4  ; Padding

; $013F - $0142: Manufacturer code
DS 4

; $0143: Game Boy Color compatibility flag
DB GBC_UNSUPPORTED

; $0144 - $0145: New licensee code
DB "AW"

; $0146: Super Game Boy compatibility flag
DB SGB_UNSUPPORTED

; $0147: Cartridge type. Either no ROM or MBC5 is recommended.
DB CART_ROM_ONLY

; $0148: ROM size
DB ROM_32K

; $0149: RAM size
DB RAM_NONE

; $014A: Destination code
DB DEST_INTERNATIONAL

; $014B: Old licensee code
DB $33  ; Indicates new license code will be used. Must be used for SGB games.

; $014C: ROM version number
DB $00

; $014D: Header checksum to be patched with rgbfix
DB $FF

; $014E- $014F: Global checksum to be patched with rgbfix
DW $0000
