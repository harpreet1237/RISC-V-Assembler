.data
n: .byte 2
.text
lui x20, 1
slli x20, x20, 16 # x20 contains the address of 'n'
lw x10, 0(x20) # loading n in x10
jal x1, fact # Jump to Factorial for first time
jal x0, full_exit

fact: # called fact with x10 = n
	 addi sp,sp,-8 # making space in stack for 2 entries (8 Bytes)
	 sw   x1,4(sp) # storing return address
	 sw   x10,0(sp) # storing the value of 'n'
	 addi x5,x10,-1 # x5 <= n-1
     addi x7, x0, 1 # x7 <= 1
     
	 bge  x5,x7,L1  # compare n-1 with 1;
	 addi x10,x0,1  # if (n-1 == 1), return 1 as fact(n-1)
	 addi sp,sp,8   #
	 jalr x0, x1, 0
L1: addi x10,x10,-1
	 jal  x1,fact # called fact with x10 = n-1
	 addi x6,x10,0
	 lw   x10,0(sp)
	 lw   x1,4(sp)
	 addi sp,sp,8
	 mul  x10,x10,x6
	 jalr x0, x1, 0
full_exit: # All good, check the stack pointer and register x10