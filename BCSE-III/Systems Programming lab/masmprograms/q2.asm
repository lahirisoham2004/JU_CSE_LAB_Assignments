name convert a binary number to decimal and vice versa.
datalen equ 20 ;17 binary digits
.model small, os_dos
.stack 100h
.data
  prompt1 db "Enter a binary string of 16 characters: $"
  prompt2 db "Enter a decimal number: $"

  ans1 db "Number in binary: $"
  ans2 db "Number in decimal: $"

  errMsg db 13,10,"Invalid input!$"
  newline db 13,10,"$"

  buffer db datalen ; length of buffer (max 4 characters, extra for null terminator)
  buflen db 0
  bufloc db datalen dup (0) ;actual data
.code
include <common.inc>

parseError:
  put09H errMsg
  .exit 1

.startup
  put09H prompt1
  read0AH buffer
  put09H newline

  lea si, bufloc
  call atobin
  jc parseError
  push ax ;store the value temporarily
  put09H ans2
  lea di, bufloc
  pop ax ;restore the value
  call utoa
  put09H bufloc
  put09H newline

  put09H prompt2
  read0AH buffer
  put09H newline

  lea si, bufloc
  call atou
  jc parseError
  push ax ;store the value temporarily
  put09H ans1
  lea di, bufloc
  pop ax
  call bintoa
  put09H bufloc
  .exit
end
