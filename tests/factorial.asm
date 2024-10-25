in
call 5:
push ax
mul
out
hlt

5:
    pop ax
    push ax

    push ax
    push 1
    jne 6:

    push 1
    pop ax
    ret

6:
    push ax
    push 1
    sub

    call 5:

    push ax
    mul
    pop ax

    ret
