;Common macros and functions

;NOTE: pushreg and popreg don't touch ax and flags. Use them for return values.
;Push bx,cx,dx,si,di,bp; mov bp, sp
pushreg macro
  push bx
  push cx
  push dx
  push si
  push di
  push bp
  mov bp, sp
  endm pushreg

;mov sp, bp; pop bp,di,si,dx,cx,bx.
popreg macro
  mov sp, bp
  pop bp
  pop di
  pop si
  pop dx
  pop cx
  pop bx
  endm popreg

;16-bit unsigned dx|ax = ax * Value
umul16 macro Value
  xor dx, dx
  push cx
  mov cx, Value
  mul cx
  pop cx
  endm umul16

;16-bit signed dx|ax = ax * Value
imul16 macro Value
  xor dx, dx
  push cx
  mov cx, Value
  imul cx
  pop cx
  endm imul16

;16-bit unsigned ax = ax / Value; ax = quotient; dx = remainder
udiv16 macro Value
  xor dx, dx
  push cx
  mov cx, Value
  div cx
  pop cx
  endm udiv16

;16-bit signed ax = ax / Value; ax = quotient; dx = remainder
idiv16 macro Value
  cwd
  push cx
  mov cx, Value
  idiv cx
  pop cx
  endm idiv16

;8-bit unsigned ax = al * Value
umul8 macro Value
  push cx
  xor ah, ah
  mov cl, Value
  mul cl
  pop cx
  endm umul8

;8-bit signed ax = al * Value
imul8 macro Value
  push cx
  xor ah, ah
  mov cl, Value
  imul cl
  pop cx
  endm imul8

;8-bit unsigned al = al / Value; al = quotient; ah = remainder
udiv8 macro Value
  push cx
  xor ah, ah
  mov cl, Value
  div cl
  pop cx
  endm udiv8

;8-bit signed al = al / Value; al = quotient; ah = remainder
idiv8 macro Value
  push cx
  cbw
  mov cl, Value
  idiv cl
  pop cl
  endm idiv8

;Put a dollar-terminated string from ds:PrAddr.
put09H macro PrAddr
  push ax
  push dx
  mov ah, 09h
  lea dx, PrAddr
  int 21h
  pop dx
  pop ax
  endm putline

;put a single character on console.
putchar02H macro Character
  push ax
  push dx
  mov dl, Character
  mov ah, 02h
  int 21h
  pop dx
  pop ax
  endm putchar02H

;Remember to put a newline after this.
;Use function 0AH using ds:buffer; put a '$' automatically at the end
read0AH macro buffer
  push ax
  push dx
  push bx
  mov ah, 0ah ;read buffered line
  lea dx, buffer
  int 21h

  lea bx, buffer
  inc bx
  xor ah, ah
  mov al, byte ptr [bx]
  inc bx
  add bx, ax
  mov byte ptr [bx], '$'

  pop bx
  pop dx
  pop ax
  endm read0AH

;Same as above but get a ASCIIZ or C-style string.
readCstr macro buffer
  push ax
  push dx
  push bx
  mov ah, 0ah ;read buffered line
  lea dx, buffer
  int 21h

  lea bx, buffer
  inc bx
  xor ah, ah
  mov al, byte ptr [bx]
  inc bx
  add bx, ax
  mov byte ptr [bx], 0

  pop bx
  pop dx
  pop ax
  endm readCstr

;destination = array[itr]; array is ds:array
arrget macro destination, array, itr
  push si
  push bx
  lea bx, array
  mov si, itr
  mov destination, [bx][si]
  pop bx
  pop si
  endm arrget

;array[itr] = source; array is ds:array
arrput macro array, itr, source
  push si
  push bx
  lea bx, array
  mov si, itr
  mov [bx][si], source
  pop bx
  pop si
  endm arrput

;Find string length; after finish Terminator == ds:SourceStr[ax]; return value in ax
strlen macro SourceStr, Terminator
  pushreg
  push es
  mov ax, ds
  mov es, ax ;scasb expects strings in es
  lea di, SourceStr
  push di ;store temporarily
  mov al, Terminator
  mov cx, 0ffffh
  pushf ;Store flags
  cld ;Change a flag
  repne scasb ;repeat while not equal scan(compare) [di] to al.
  popf ;Restore the flags
  pop ax ;load start of string in ax
  pop es ;reload es
  sub di, ax
  dec di ;now di contains correct required length.
  mov ax, di
  popreg
  endm strlen

;NOTE: 8086 stack allocates while moving BACKWARD, while words are read forward, so first variable is at bp-2 (the bytes are ss:[bp-2] and ss:[bp-1]), next one is at bp-4, so on and so forth. I know it looks and sounds so stupid...

;Read dollar-terminated ASCII string from [si] and store as 16-bit unsigned integer in ax. Carry flag is cleared if no error, set if error. AX is undefined on error.
atou:
  pushreg
  xor ax, ax
  push ax ;now word ptr ss:[bp-2] is my integer.

atouLoop:
  mov al, [si]
  cmp al, '$'
  je atouLoopEnd
  cmp al, '0'
  jb atouError
  cmp al, '9'
  ja atouError
  sub al, '0' ;get digit
  xor ah, ah ;convert al to word ax; unsigned
  push ax ;store the digit
  mov ax, word ptr ss:[bp-2] ;load sum
  umul16 10 ;Multiply by 10
  mov word ptr ss:[bp-2], ax ;store sum*10
  pop ax ;get digit back
  add word ptr ss:[bp-2], ax ;store sum*10+d

  inc si
  jmp atouLoop
atouLoopEnd:
  mov ax, word ptr ss:[bp-2]
  popreg ;doesn't modify ax
  clc
  ret
atouError:
  popreg
  stc
  ret
  ;endp atou

;Read dollar-terminated ASCII string from [si] which may start with a '+' or '-' and returns a 16-bit SIGNED integer in ax. Carry flag is 0 if no error, 1 if error.
atod:
  pushreg
  xor ax, ax
  push ax ;Now our result variable is in word ptr ss:[bp-2], initialized to 0
  push ax ;Allocate a flag variable in [bp-4], 0 if +ve (default) and -1 if negative
  xor cx, cx
  cmp byte ptr [si], '-'
  je atodNegative
  cmp byte ptr [si], '+'
  je atodPositive
  jmp atodLoop

atodNegative: ;If negative
  mov byte ptr ss:[bp-4], 0ffh
  inc si
  jmp atodLoop
atodPositive:
  inc si

atodLoop:
  mov al, [si]
  cmp al, '$'
  je atodLoopEnd
  cmp al, '0'
  jb atodError
  cmp al, '9'
  ja atodError
  sub al, '0'
  xor ah, ah
  push ax ;store the digit
  mov ax, word ptr ss:[bp-2] ;reload integer
  umul16 10 ;get sum = sum*10
  mov word ptr ss:[bp-2], ax
  pop ax ;get digit back
  add word ptr ss:[bp-2], ax

  inc si
  jmp atodLoop
atodLoopEnd:

  cmp byte ptr ss:[bp-4], 0
  jz atodEnd
  neg word ptr ss:[bp-2] ;execute if original was negative

atodEnd:
  mov ax, word ptr ss:[bp-2]
  popreg
  clc
  ret
atodError:
  popreg
  stc
  ret

;Create dollar-terminated decimal string from SIGNED integer at ax to word ptr [di]. Length of generated string (excluding '$') is in ax.
dtoa:
  pushreg
  push ax; store our input at word ptr ss:[bp-2]
  xor cx, cx ;count string length

  cmp ax, 0
  je dtoaZero
  jg dtoaPositive ;if positive, continue

  ;Negative integer.
  neg word ptr ss:[bp-2] ;we are negative
  mov byte ptr [di], '-'
  inc di
  inc cx

dtoaPositive:
  mov ax, word ptr ss:[bp-2]
dtoaCount:
  udiv16 10 ;divide the number by 10
  inc di
  inc cx
  cmp ax, 0
  jnz dtoaCount ;loop till quotient is zero, counting the digits

  mov byte ptr [di], '$' ;Terminator
  mov ax, word ptr ss:[bp-2]
dtoaConv:
  dec di
  udiv16 10 ;Get remainder
  add dl, '0'
  mov byte ptr [di], dl
  cmp ax, 0
  jnz dtoaConv ;Loop till ax is zero

  mov ax, cx ;Initialize count return value
  popreg
  ret

dtoaZero:
  mov byte ptr [di], '0'
  mov byte ptr [di+1], '$'
  mov ax, 1
  popreg
  ret

;Create dollar-terminated decimal string from unsigned integer at ax at word ptr [di]. Length of generated string (excluding '$') in ax.
utoa:
  pushreg
  cmp ax, 0
  jz utoaZero
  push ax; store our number temporarily at word ptr ss:[bp-2]

  xor cx, cx ;cx = count of the number of digits = length
utoaCount:
  udiv16 10 ;divide the number by 10
  inc di
  inc cx
  cmp ax, 0
  jnz utoaCount ;loop till quotient is zero, counting the digits

  mov byte ptr [di], '$' ;Terminator
  mov ax, word ptr ss:[bp-2] ;reload input integer
utoaConv:
  dec di
  udiv16 10 ;divide ax by 10; dl contains remainder
  add dl, '0'
  mov byte ptr [di], dl
  cmp ax, 0
  jnz utoaConv

  mov ax, cx ;Initialize count return value
  popreg
  ret

utoaZero:
  mov byte ptr [di], '0'
  mov byte ptr [di+1], '$'
  mov ax, 1
  popreg
  ret

;Parse a dollar-terminated string from byte ptr [si] as a binary string and return a 16-bit unsigned integer in ax.
atobin:
  pushreg
  xor ax, ax
  xor dx, dx

atobinLoop:
  mov dl, byte ptr [si]
  cmp dl, '$'
  je atobinLoopEnd
  sub dl, '0'
  cmp dl, 1
  ja atobinError
  shl ax, 1
  or ax, dx
  inc si
  jmp atobinLoop
atobinLoopEnd:

  popreg
  clc
  ret

atobinError:
  popreg
  stc
  ret

;Convert a 16-bit unsigned integer from ax to a binary string in byte ptr [di] of 16 characters; plus a terminator at 17th place. ax is unchanged.
bintoa:
  pushreg
  mov dx, 8000h

bintoaLoop:
  mov byte ptr [di], '0'
  test ax, dx
  jz bintoaSkip
  inc byte ptr [di]
bintoaSkip:
  inc di
  shr dx, 1
  cmp dx, 0
  jnz bintoaLoop

  mov byte ptr [di], '$'
  popreg
  ret
