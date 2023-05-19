.globl factorial

.data
n: .word 8

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

factorial: # arg n in a0, return n! in a0
    addi t0 x0 1 # n = 1 return n
    beq a0, t0, ret_one
    addi sp sp -8 # allocate space for return address 
    sw ra 0(sp) # store return address
    sw a0, 4(sp) # save n
    addi a0 a0 -1 # n = n - 1
    jal ra, factorial # call factorial(n-1)
    lw t0 4(sp) # restore a0 to to
    mul a0, t0, a0  
    lw ra 0(sp) # restore return address
    addi sp sp 8 # free stack frame
ret_one:
    ret
