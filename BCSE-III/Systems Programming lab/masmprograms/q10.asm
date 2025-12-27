name Print prime numbers uptil 100
limit equ 100
.model small, os_dos
.stack 100h
.data
  bufloc db 7 dup (0) ;to print integers
  prompt db "Prime numbers till 100:",13,10,"$"
  newline db 13,10,"$"

  ;We will use the sieve algorithm
  sieve db limit+1 dup (0)
.code
include <common.inc>

printax macro ;print ax
  lea di, bufloc
  push ax ;utoa modifies ax
  call utoa
  pop ax
  put09H bufloc
  endm printax

.startup
  put09H prompt

  mov sieve, 1
  lea bx, sieve
  mov si, 2

sieveOuter:
  cmp byte ptr [bx][si-1], 0
  jnz sieveInnerEnd
  mov ax, si
  printax
  put09H newline
  mov di, si
sieveInner:
  mov byte ptr [bx][di-1], 1
  add di, si
  cmp di, limit
  jb sieveInner
sieveInnerEnd:
  inc si
  cmp si, limit
  jb sieveOuter

  .exit
end
