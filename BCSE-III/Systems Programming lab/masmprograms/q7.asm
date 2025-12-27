name Perform substring deletion from a given string
buflen equ 255
.model small, os_dos
.stack 100h
.data
  newline db 13,10,"$"
  prompt db "Enter your string: $"
  prLen db "Length of your string: $"
  prIndex db "Enter substring start index: $"
  errNumber db "Invalid number!$"
  prSubLen db "Enter substring length: $"

  errBounds db "Incorrect bounds!$"

  subStart dw ? ;substring start index
  subLength dw ? ;substring length

  sourceStart db buflen
  sourceLen db ?
  source db buflen dup (?)

  ;General buffer for entering and printing numbers
  buffer db buflen
  bufLength db ?
  bufloc db buflen dup (?)

.code
include <common.inc>

printnum macro Value ;Print the value
  push ax
  push di
  mov ax, Value
  lea di, bufloc
  call utoa
  put09H bufloc
  pop di
  pop ax
  endm printnum

parseError: ;called when parsing error
  put09H errNumber
  .exit 1

boundsIncorrect: ;called on incorrect bounds
  put09H errBounds
  .exit 1

readnum macro Destination ;Read a integer to Destination
  read0AH buffer
  push si
  push ax
  lea si, bufloc
  call atou
  jc parseError
  mov Destination, ax
  put09H newline
  pop ax
  pop si
  endm readnum

.startup
  mov bp, sp ;Store stack pointer.
  put09H prompt ;Read input string
  read0AH sourceStart
  put09H prLen ;Print length of string.
  xor dh, dh
  mov dl, sourceLen
  push dx ;store length of string at ss:[bp-2]
  printnum dx
  put09H newline

  put09H prIndex ;read start index of substring
  readnum Start ;read unsigned so cannot be below 0.
  mov ax, ss:[bp-2] ;load sourceLen
  cmp ax, sourceStart 
  jbe boundsIncorrect ;error if sourceLen <= sourceStart

  put09H prSubLen
  readnum 

  .exit
end
