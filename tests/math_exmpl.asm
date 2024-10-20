push 0
pop ax
5:
pushr ax
pushr ax
mul
out
pushr ax
push 1
add
pop ax
pushr ax
push 10
ja 5:
hlt
