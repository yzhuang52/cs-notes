.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 77.
# =================================================================
argmax:
    li t0 1
    blt a1 t0 exit
    # Prologue
    addi sp, sp, -4
    sw ra, 0(sp)
    addi t0, x0, 0 # t0 hold the index, initialize as 0
    lw t1, 0(a0) # t1 hold the max value, initialize as array[0]
    addi t3, t0, 0 # t3 temporary hold the first index of the largest element, initialize as 0
loop_start:
    beq t0, a1, loop_end
    slli t2, t0, 2 # t2 hold the stride
    add t4, a0, t2 # t4 hold the address of array[i]
    lw t5, 0(t4) # t5 hold the value of array[i]
    ble t5, t1, loop_continue # if array[i] > max, update max and index, else loop
    mv t1, t5 
    mv t3, t0
loop_continue:
    addi t0, t0, 1
    jal loop_start

loop_end:
    

    # Epilogue
    mv a0, t3
    lw ra, 0(sp)
    addi sp, sp, 4
    ret

exit:
    addi a1, x0, 77
    j exit2