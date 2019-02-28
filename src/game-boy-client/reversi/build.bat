set filename=reversi
c:\rgbds\rgbgfx -o graphics.bin graphics.png
c:\rgbds\rgbasm -o%filename%.obj %filename%.a
c:\rgbds\rgblink -o%filename%.gb %filename%.obj
c:\rgbds\rgbfix -p0 -v %filename%.gb
pause
