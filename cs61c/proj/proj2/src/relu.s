.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 78.
# ==============================================================================
relu:
    # Prologue
    addi sp, sp, -12                 # allocate space on stack
    sw   ra, 0(sp)
    sw   s0, 4(sp) 
    sw   s2, 8(sp)
    addi s0, x0, 1                   # s0 hold 1 to check the length of array    
    blt  a1, s0, error   
    mv   t0, x0                      # t0 hold current index, loop t0 from 0 to a1

loop_start:
    slli s2, t0, 2                   # s2 hold the stride of array 
    beq  t0, a1, loop_end            # t0 = a1, finish loop. else enter loop
    add  t1, a0, s2                  # t1 hold the address of array[i]
    lw   t2, 0(t1)                   # t2 hold the value of array[i]
    bgt  t2, x0, loop_continue       # if array[i] > 0, value not change 
    sw   x0, 0(t1)                   # else array[i] = 0



loop_continue:
    addi t0, t0, 1                   # t0 += 1
    jal  loop_start


loop_end:


    # Epilogue

    lw   ra, 0(sp)
    lw   s0, 4(sp)  
    lw   s2, 8(sp)
    addi sp, sp, 12
	ret

error:
    addi a1, x0, 78                  # terminate program with error code 78
    j exit2 

