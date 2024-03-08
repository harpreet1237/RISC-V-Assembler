0 lui x20, 1
4 slli x20, x20, 16
8 lw x10, 0(x20)
12 jal x1, fact
16 jal x0, full_exit
20 addi sp,sp,-8
24 sw x1,4(sp)
28 sw x10,0(sp)
32 addi x5,x10,-1
36 addi x7, x0, 1
40 bge x5,x7,L1
44 addi x10,x0,1
48 addi sp,sp,8
52 jalr x0, x1, 0
56 addi x10,x10,-1
60 jal x1,fact
64 addi x6,x10,0
68 lw x10,0(sp)
72 lw x1,4(sp)
76 addi sp,sp,8
80 mul x10,x10,x6
84 jalr x0, x1, 0
