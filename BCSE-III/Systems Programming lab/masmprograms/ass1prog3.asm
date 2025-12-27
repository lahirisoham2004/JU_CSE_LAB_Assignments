.model small
.stack 100h

.data
    num1 dw 1A3Fh    ; First hexadecimal number
    num2 dw 2B5Dh    ; Second hexadecimal number
    sum  dw ?        ; To store the result of addition
    hexStr db '0000$' ; To store the hexadecimal string of the sum, terminated by '$'

.code
main proc
    ; Initialize data segment
    mov ax, @data
    mov ds, ax

    ; Load the first number into ax
    mov ax, num1

    ; Add the second number to ax
    add ax, num2

    ; Store the result in the sum variable
    mov sum, ax

    ; Convert the sum to a hexadecimal string
    call HexToStr

    ; Print the sum
    mov dx, offset hexStr
    mov ah, 09h        ; DOS interrupt to print string
    int 21h

    ; Exit program
    mov ah, 4Ch
    int 21h

main endp

;-----------------------------------------------------------
; HexToStr: Convert the value in AX to a 4-digit hexadecimal string
; The string is stored in hexStr and is terminated with '$' for printing
;-----------------------------------------------------------
HexToStr proc
    ; Convert the first nibble (highest 4 bits)
    mov cx, 4         ; Loop counter for 4 hex digits
    mov di, offset hexStr  ; Point to the string

ConvertLoop:
    rol ax, 4         ; Rotate left by 4 bits to get the next hex digit in the lowest nibble
    mov bl, al        ; Move the lower byte of AX to BL
    and bl, 0Fh       ; Mask out the upper 4 bits to isolate the hex digit
    cmp bl, 09h
    jbe DigitIsNum
    add bl, 07h       ; If the digit is 'A'-'F', add 7 to adjust ASCII value

DigitIsNum:
    add bl, 30h       ; Convert to ASCII ('0'-'9' or 'A'-'F')
    mov [di], bl      ; Store the ASCII character in the string
    inc di            ; Move to the next character in the string
    loop ConvertLoop  ; Repeat for all 4 digits

    ret
HexToStr endp

end main
