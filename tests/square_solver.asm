in
in
in
pop cx
pop bx
pop ax

push 0
push ax

jne 1:

    push 0
    push bx

    jne 2:
        push 0
        push cx

        jne 3:
            push 8
            out
            hlt
        3:
        push -666
        out
        hlt
    2:
    push -1
    push cx
    mul
    push bx
    div
    push 1
    out
    out
    hlt
1:
push bx
push bx
mul
push 4
push cx
push ax
mul
mul
sub
pop dx

push 0
push dx
jne 4:
    push -1
    push bx
    mul
    push 2
    push ax
    mul
    div
    push 1
    out
    out

    hlt
4:
jb 5:
    push -1
    push bx
    mul
    push dx
    sqrt
    sub
    push 2
    push ax
    mul
    div
    push -1
    push bx
    mul
    push dx
    sqrt
    add
    push 2
    push ax
    mul
    div
    push 2
    out
    out
    out

    hlt
5:
push -666
out

hlt
