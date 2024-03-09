lui x20 1 
U
33
slli x20 x20 16 
I
33
lw x10 0(x20) 
I
33
jal x1 fact 
UJ
33
jal x0 full_exit 
UJ
33
addi sp sp -8 
I
33
sw x1 4(sp) 
S
33
sw x10 0(sp) 
S
33
addi x5 x10 -1 
I
33
addi x7 x0 1 
I
33
bge x5 x7 L1 
SB
33
addi x10 x0 1 
I
33
addi sp sp 8 
I
33
jalr x0 x1 0 
I
33
addi x10 x10 -1 
I
33
jal x1 fact 
UJ
33
addi x6 x10 0 
I
33
lw x10 0(sp) 
I
33
lw x1 4(sp) 
I
33
addi sp sp 8 
I
33
mul x10 x10 x6 
R
33
jalr x0 x1 0 
I
33
