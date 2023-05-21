.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 75.
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 76.
# =======================================================
dot:
    li t0, 1
    blt a2, t0, exit_length
    blt a3, t0, exit_stride
    blt a4, t0, exit_stride
    # Prologue
    addi sp, sp, -16
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    li t0, 0 # t0 hold the index
    li t1, 0 # t1 hold the sum of dot(a1, a2)
loop_start:
    beq t0, a2, loop_end 
    slli t2, t0, 2
    mul t3, t2, a3 # t3 hold stride*index of a1
    mul t4, t2, a4 # t4 hold stride*index of a2
    add t5, a0, t3 # t5 hold the address of a1[stride*index]
    add t6, a1, t4 # t6 hold the address of a2[stride*index]
    lw s0, 0(t5) # s0 hold the value of a1[stride*index]
    lw s1, 0(t6) # s1 hold the value of a2[stride*index]
    mul s2, s0, s1
    add t1, t1, s2 
    addi t0, t0, 1
    jal loop_start
loop_end:
    mv a0, t1

    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    addi sp, sp, 16
    ret

exit_length:
    addi a1, x0, 75
    j exit2 

exit_stride:
    addi a1, x0, 76
    j exit2 