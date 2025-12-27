name take 2 decimal numbers, add and subtract
datalen equ 10 ;max 65535
.model small, os_dos
.stack 100h
.data
  buffer db datalen
  buflen db 0
  bufloc db datalen dup (0) ;actual data

  prompt1 db "Enter first value (A) in decimal: $"
  prompt2 db "Enter second value (B) in decimal: $"
  padd db "A + B = $"
  psub db "A - B = $"
  newline db 13,10,"$"

  errMsg db 13,10,"Invalid number!$"
  val1 dw 0 ;First value
  val2 dw 0 ;Second value
  result dw ? ;temporary results

.code
include <common.inc>

parseError:
  put09H errMsg
  .exit 1

readnum macro Prompt, Destination
  put09H Prompt
  read0AH buffer
  lea si, bufloc
  call atod
  jc parseError
  mov Destination, ax
  put09H newline
  endm readnum

printnum macro Number
  lea di, bufloc
  mov ax, Number
  call dtoa
  put09H bufloc
  endm printnum

.startup
  readnum prompt1, val1
  readnum prompt2, val2

  mov ax, val1
  add ax, val2
  put09H padd
  printnum ax
  put09H newline

  mov ax, val1
  sub ax, val2
  put09H psub
  printnum ax
  .exit
end
