      sum:
          add bx,cx                                 
          jc pc1  ; Jump to overflow handler if carry is set

          jmp output ; If no overflow, jump to the output section

      pc1:                                          
          ; Handle the overflow here
          mov dl, '1'  ; Output the extra 1 for overflow
          mov ah, 2
          int 21h

      output:
          ; Output the high nibble of BH
          mov ch, bh
          shr ch, cl
          and ch, 0fh
          cmp ch, 10
          add ch, '0'
          cmp ch, ':'
          jl tag
          add ch, 7
      tag:
          mov dl, ch
          mov ah, 2
          int 21h

          ; Output the low nibble of BH
          mov ch, bh
          and ch, 0fh
          cmp ch, 10
          add ch, '0'
          cmp ch, ':'
          jl tag1
          add ch, 7
      tag1:
          mov dl, ch
          mov ah, 2
          int 21h

          ; Output the high nibble of BL
          mov ch, bl
          shr ch, cl
          and ch, 0fh
          cmp ch, 10
          add ch, '0'
          cmp ch, ':'
          jl tag2
          add ch, 7
      tag2:
          mov dl, ch
          mov ah, 2
          int 21h

          ; Output the low nibble of BL
          mov ch, bl
          and ch, 0fh
          cmp ch, 10
          add ch, '0'
          cmp ch, ':'
          jl tag3
          add ch, 7
      tag3:
          mov dl, ch
          mov ah, 2
          int 21h

      exit:
          mov ah, 4ch                               
          int 21h

