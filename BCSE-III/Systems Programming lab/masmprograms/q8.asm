name GCD and LCM of 3 integers.
datalen equ 7
.model small, os_dos
.stack 100h
.data
  buffer db datalen
  buflen db 0
  bufloc db datalen dup(0)

  prFirst db "Enter first integer: $"
  prSecond db "Enter second integer: $"
  prThird db "Enter third integer: $"
  prGCD db "Greatest common divisor: $"
  prLCM db "Least common multiple: $"
  errMsg db 13,10,"Invalid input!$"
  divMsg db 13,10,"Division by zero!$"
  newline db 13,10,"$"

  first dw ?
  second dw ?
  third dw ? ;Three integers
.code
include <utils.inc>

printax macro ;print ax
  lea di, bufloc
  push ax ;utoa modifies ax
  call utoa
  pop ax
  put09H bufloc
  endm printax

divByZero:
  put09H divMsg
  .exit 1

;GCD function to find GCD of ax and bx. Result in ax. ASSUME bx is not 0.
gcd:
  pushreg
  cmp ax, 0
  jz divByZero
  cmp bx, 0
  jz divByZero
  cmp ax, bx
  jae gcdLoop
  xchg ax, bx ;ax must be bigger for this to work.
gcdLoop:
  udiv16 bx
  mov ax, bx
  mov bx, dx
  cmp bx, 0
  jnz gcdLoop
  popreg
  ret

;LCM function to find LCM of ax and bx, result in ax. Assume bx is not 0.
lcm macro
  pushreg
  push ax ;ss:[bp-2] is first number
  push bx ;ss:[bp-4] is second number
  call gcd ;Find their gcd
  push ax ;ss:[bp-6] is GCD
  mov ax, ss:[bp-2]
  udiv16 ss:[bp-6]
  umul16 ss:[bp-4]
  popreg
  endm lcm

parseError:
  put09H errMsg
  .exit 1

readnum macro Prompt, Destination
  put09H Prompt
  read0AH buffer
  lea si, bufloc
  call atou
  jc parseError
  mov Destination, ax
  put09H newline
  endm readnum

.startup
  readnum prFirst, first
  readnum prSecond, second
  readnum prThird, third

  mov ax, first
  mov bx, second
  call gcd
  mov bx, third
  call gcd

  put09H prGCD
  printax
  put09H newline

  mov ax, first
  mov bx, second
  lcm
  mov bx, third
  lcm

  put09H prLCM
  printax
  .exit
end
