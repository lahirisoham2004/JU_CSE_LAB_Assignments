name Divide a 16-bit number by an 8-bit number
datalen equ 7
.model small, os_dos
.stack 100h
.data
  buffer db datalen
  buflen db 0
  bufloc db datalen dup(0)

  dividend dw ?
  divisor dw ?

  prDividend db "Enter dividend (16-bit integer): $"
  prDivisor db "Enter divisor (8-bit integer): $"
  prQuotient db "Quotient: $"
  prRemainder db "Remainder: $"
  errMsg db 13,10,"Invalid input! $"
  divMsg db 13,10,"Division by zero!$"
  newline db 13,10,"$"
.code
include <utils.inc>

putnum macro Number ;put the number in ax as output
  lea di, bufloc
  mov ax, Number
  call dtoa
  put09H bufloc
  endm putnum

parseError:
  put09H errMsg
  .exit 1

divByZero:
  put09H divMsg
  .exit 1

readnum macro Destination;read string and put into destination
  read0AH buffer
  lea si, bufloc
  call atod
  jc parseError
  mov Destination, ax
  put09H newline
  endm readnum

.startup
  put09H prDividend
  readnum dividend
  put09H prDivisor
  readnum divisor
  cmp divisor, 0
  je divByZero
  cmp divisor, -128                                                                                                                                                                                                                                                                                                                     
  jl parseError
  cmp divisor, 127
  jg parseError

  mov ax, dividend
  idiv16 divisor
  push dx ;store remainder
  push ax ;store quotient

  put09H prQuotient
  pop ax ;Get quotient
  putnum ax
  put09H newline
  put09H prRemainder
  pop ax ;Get remainder
  putnum ax
  .exit
end
