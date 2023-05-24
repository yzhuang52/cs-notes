.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 93.
# - If you receive an fwrite error or eof,
#   this function terminates the program with error code 94.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 95.
# ==============================================================================

# C source code:
# FILE* fp = fopen(filepath, w);
# int* p = malloc(sizeof(int)*2)
# *p = row
# *(p+1) = col
# fwrite(fp, p, 2, 4);
# fwrite(fp, matrix, rows * cols, 4);
# fclose(fp)

write_matrix:

    # Prologue
    addi sp, sp, -12
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    # 1. Open file
    addi sp, sp, -12
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    mv a1, a0
    li a2, 1
    jal fopen
    li t0, -1
    beq a0, t0, exit_93
    mv s0, a0 # s0 hold the file descriptor
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    # 1. Ends

    # 2. Write rows and cols to file
    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 8
    jal malloc 
    li t0, 0
    beq a0, t0, exit_88
    mv s1, a0 # s1 hold the pointer for rows and cols  
    lw a0, 0(sp)
    lw a1, 4(sp)
    addi sp, sp, 8
    sw a2, 0(s1)
    sw a3, 4(s1)
    addi sp, sp, -16
    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    sw a4, 12(sp)
    mv a1, s0
    mv a2, s1 
    li a3, 2
    li a4, 4
    jal fwrite 
    li t0, -1
    beq a0, t0, exit_94
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    lw a4, 12(sp)
    addi sp, sp, 16

    # 2. Ends

    # 3. Write matrix to file

    addi sp, sp, -16
    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    sw a4, 12(sp)
    mul t1, a2, a3 # t1 holds rows * cols
    mv a2, a1 
    mv a1, s0 
    mv a3, t1
    li a4, 4
    jal fwrite 
    li t0, -1
    beq a0, t0, exit_94
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    lw a4, 12(sp)
    addi sp, sp, 16

    # 3. Ends

    # 4. Close file
    mv a1, s0 
    jal fclose
    li t0, -1
    beq a0, t0, exit_95
    # Epilogue
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    addi sp, sp, 12

    ret

exit_88:
    li a1, 88
    j exit2 

exit_93:
    li a1, 93
    j exit2

exit_94:
    li a1, 94
    j exit2 

exit_95:
    li a1, 95
    j exit2