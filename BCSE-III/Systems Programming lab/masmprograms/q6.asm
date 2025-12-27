name print Fibonacci series upto 10 terms
terms equ 10
.model small, os_dos
.stack 100h
.data
  prompt db "Fibonacci series upto 10 terms:",13,10,"$"
  string db 7 dup (0) ;to store the converted numbers
  newline db 13,10,"$"

  a dw 0
  b dw 1
.code
include <utils.inc>

printax macro ;print ax
  lea di, string
  push ax ;utoa modifies ax
  call utoa
  pop ax
  put09H string
  endm printax

.startup
  put09H prompt
  mov ax, a
  printax
  put09H newline
  mov ax, b
  printax
  put09H newline
  mov cx, 2
mainLoop:
  mov ax, a
  add ax, b
  printax
  put09H newline
  xchg b, ax ;b = current and ax = b
  mov a, ax ;a = ax = b and b = current.
  inc cx
  cmp cx, terms
  jb mainLoop

  .exit
end
