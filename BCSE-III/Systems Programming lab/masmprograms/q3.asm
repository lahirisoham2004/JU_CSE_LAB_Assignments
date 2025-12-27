name print pairs of even numbers whose sum is 100
limit equ 100
.model small, os_dos
.stack 100h
.data
  prompt db "Pairs of even integers with sum = 100:",13,10,"$"
  string db 4 dup (0) ;to store the converted numbers
  newline db 13,10,"$"
.code
include <utils.inc>

putnum macro Number ;put the number in ax as output
  lea di, string
  mov ax, Number
  call utoa
  put09H string
  endm putnum

.startup
  put09H prompt

  xor cx, cx
mainLoop:
  mov ax, limit
  sub ax, cx
  putnum ax
  putchar02H ' '
  putnum cx
  put09H newline
  add cx, 2
  cmp cx, limit
  jbe mainLoop

  .exit
end
