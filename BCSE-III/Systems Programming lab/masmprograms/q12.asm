name Rename a file
.model small, os_dos
.stack 100h
.data
  newline db 13,10,"$"
  prname1 db "Enter source filename: $";
  name1start db 255
  name1len db ?
  name1 db 255 dup(?)

  prname2 db "Enter target filename: $";
  name2start db 255
  name2len db ?
  name2 db 255 dup(?)

  okMsg db "Rename successful.$"
  errMsg db "Rename failed!$"
.code
include <common.inc>

.startup
  put09H prname1
  readCstr name1start
  put09H newline

  put09H prname2
  readCstr name2start
  put09H newline

  mov ax, ds
  mov es, ax
  lea di, name2
  lea dx, name1
  mov ah, 56h
  int 21h
  jc Failure

  put09H okMsg
  .exit
Failure:
  push ax
  put09H errMsg
  pop ax
  .exit al
end
