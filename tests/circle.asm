push 0
pop bx

6:
    push 0
    pop ax
    push 0
    pop cx

    5:

    push 10
    push ax
    sub
    push 10
    push ax
    sub
    mul
    push 7
    push bx
    sub
    push 7
    push bx
    sub
    mul
    add
    pop cx
    push 40
    push cx

    ja 7:

    push ax
    push bx
    push 20
    mul
    add
    pop dx
    push 35
    pop [dx]

    7:

    push 1
    push ax
    add
    pop ax
    push ax
    push 20

    ja 5:

push bx
push 1
add
pop bx

push bx
push 14

ja 6:

draw
hlt
