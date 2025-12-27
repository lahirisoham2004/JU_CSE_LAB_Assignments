name take 2 decimal numbers, add and subtract
datalen equ 7 ;max 65535
.model small, os_dos
.stack 100h
.data
  buffer db datalen ; length of buffer (max 4 characters, extra for null terminator )
  buflen db 0
  bufloc db datalen dup (0) ;actual data

  prompt1 db "Enter first value (A) in decimal: $"
  prompt2 db "Enter second value (B) in decimal: $"
  padd db "A + B = $"
  psub db "A - B = $"
  newline db 13,10,"$"

  errMsg db "Invalid number!$"
  val1 dw 0 ;First value
  val2 dw 0 ;Second value
  result dw ? ;temporary results

.code
pushreg macro
  push ax
  push bx
  push cx
  push dx
  push si
  push di
  push bp
  mov bp, sp
  endm pushreg

popreg macro
  mov sp, bp
  pop bp
  pop di
  pop si
  pop dx
  pop cx
  pop bx
  pop ax
  endm popreg

mulimm macro Immediate
  xor dx, dx
  push cx
  mov cx, Immediate
  mul cx
  pop cx
  endm mulimm

divimm macro Immediate
  xor dx, dx
  push cx
  mov cx, Immediate
  div cx
  pop cx
  endm divimm

putline macro PrAddr
  pushreg
  mov ah, 09h
  lea dx, PrAddr
  int 21h
  popreg
  endm putline

readline macro
  pushreg
  mov ah, 0ah ;read buffered line
  lea dx, buffer
  int 21h
  putline newline
  popreg
  endm readline

parseError: ;parsing error
  putline errMsg
  .exit 1
  ;parseError ends here

parseDecimal: ;Function to read to a destination from bufloc using buflen; take destination address at di
  pushreg
  mov word ptr [di], 0; initialize destination to zero
  xor cx, cx
  cmp byte ptr bufloc, '-' ;Is minus sign present at first byte?
  jne psloop ;skip if not
  inc cl ;else check from bufloc + 1

psloop:
  lea bx, bufloc
  add bx, cx
  mov al, [bx] ;move into al

  cmp al, '0'
  jb parseError
  cmp al, '9'
  ja parseError
  sub al, '0'
  xor ah, ah
  push ax ;Stored digit
  mov ax, word ptr [di]
  mulimm 10 ;Multiply number by 10
  mov word ptr [di], ax
  pop ax ;Got digit
  add word ptr [di], ax

  inc cl
  cmp cl, buflen
  jb psloop

  cmp byte ptr bufloc, '-' ;Was a negative integer?
  jne negParseSkip
  neg word ptr [di] ;else negate the result

negParseSkip:
  popreg
  ret

itoa: ;Function to convert a number in word ptr [si] to string, stored in bufloc.
  pushreg
  lea bx, bufloc
  mov ax, word ptr [si]
  push ax ;store our number temporarily

  cmp ax, 0
  je itoaZero
  ;is negative?
  jg itoaPositive ;if positive, skip

  neg word ptr [si] ;negate before conversion
  mov [bx], '-' ;put first character as minus
  inc bx

itoaPositive:
  mov di, bx ;ds:bx at which to store
  mov ax, word ptr [si]
dcount:
  divimm 10 ;Quotient in ax
  inc di
  cmp ax, 0
  jnz dcount ;Continue till quotient is zero

  mov [di], '$'
  mov ax, word ptr [si]
dconvert:
  dec di
  divimm 10 ;Get remainder
  add dl, '0' ;Convert to ascii digit
  mov [di], dl
  cmp ax, 0
  jnz dconvert

  pop ax; restore original number
  cmp ax, 0 ;was original number negative?
  jg itoaSkip ;if positive, do nothing
  neg word ptr [si] ;Make original number negative back again

itoaSkip:
  popreg
  ret

itoaZero: ;When zero
  pop ax
  lea di, bufloc
  mov [di], '0'
  mov [di+1], '$'
  popreg
  ret

parse macro destination
  lea di, destination
  call parseDecimal
  endm parse

print macro source
  lea si, source
  call itoa
  putline bufloc
  endm print

.startup
  putline prompt1
  readline
  parse val1
  putline prompt2
  readline
  parse val2

  mov ax, val1
  add ax, val2
  mov result, ax

  putline padd
  print result
  putline newline

  mov ax, val1
  sub ax, val2
  mov result, ax

  putline psub
  print result
  putline newline
  .exit

end
