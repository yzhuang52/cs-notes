.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 72.
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 73.
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 74.
# =======================================================
matmul:

    # Error checks
    li t0, 1
    # Checks the size of m0
    blt a1, t0, exit_m0
    blt a2, t0, exit_m0
    # Checks the size of m1
    blt a4, t0, exit_m1
    blt a5, t0, exit_m1
    # Checks if m0 and m1 size matches
    bne a2, a4, exit_nomatch
    # Prologue
    addi sp, sp -4
    sw ra, 0(sp)
    li t0, 0 # t0 hold the outer loop index, initialize as 0
    mv t2, a6 # t2 hold the address of result[i][j]
outer_loop_start:
    beq t0, a1, outer_loop_end 
    li t1, 0 # t1 hold the inner loop index, initialize as 0
inner_loop_start:
    # t4 = col, t5 = rowi * colm1
    # t6 hold the index of result
    beq t1, a5, inner_loop_end
    slli t3, t0, 2
    slli t4, t1, 2
    mul t5, t3, a5
    add t6, t5, t4
    addi sp, sp, -40
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    sw a3, 12(sp)
    sw a4, 16(sp)
    sw t0, 20(sp)
    sw t1, 24(sp)
    sw t2, 28(sp)
    sw a5, 32(sp)
    sw s0, 36(sp)
    # adjust the arguments for dot function
    mul s0, t3, a2
    add a0, a0, s0
    mv a1, a3
    add a1, a1, t4 
    li a3, 1
    mv a4, a5
    jal dot
    lw t0, 20(sp)
    lw t1, 24(sp)
    lw t2, 28(sp)
    sw a0, 0(t2)
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    lw a3, 12(sp)
    lw a4, 16(sp)
    lw a5, 32(sp)
    lw s0, 36(sp)
    addi sp, sp, 40
    addi t1, t1, 1
    addi t2, t2, 4
    jal inner_loop_start
inner_loop_end:
    addi t0, t0, 1
    jal outer_loop_start
outer_loop_end:
    # Epilogue
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

exit_m0:
    addi a1, x0, 72
    j exit2 

exit_m1:
    addi a1, x0, 73
    j exit2 

exit_nomatch:
    addi a1, x0, 74
    j exit2