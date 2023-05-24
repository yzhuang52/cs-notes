.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
#
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 88.
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 90.
# - If you receive an fread error or eof,
#   this function terminates the program with error code 91.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 92.
# ==============================================================================

# C source code
# FILE* fp = fopen("path", r);
# fread(fp, rows, 4);
# fread(fp, cols, 4);
# int* matrix = malloc(sizeof(int) * (*rows) * (*cols));
# int* matrix_start = matrix;
# for (int i = 0; i < *rows)
#    for (int j = 0; j < *cols)
#       {
#           fread(fp, matrix, 4);
#           matrix++;
#        }
# fclose(fp);
# return matrix_start;
read_matrix:

    # Prologue
	addi sp, sp, -20
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    # 1. get the file descriptor by calling fopen
    # fopen require a1 to be the pointer to path, a2 to be permissions
    # we need save a1 and a2 before calling fopen, and adjust arguments
    
    addi sp, sp, -8
    sw a1, 0(sp)
    sw a2, 4(sp)
    mv a1, a0 
    li a2, 0
    jal fopen 
    li t0, -1
    beq a0, t0, exit_90
    mv s0, a0 # the file descriptor is stored at s0
    lw a1, 0(sp)
    lw a2, 4(sp)
    addi sp, sp, 8
    # 1. ends

    # 2. read the size of matrix and stored them at *rows and *cols
    # read rows
    addi sp, sp, -12
    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    mv a2, a1
    li a3, 4
    mv a1, s0 
    jal fread 
    li t0, -1
    beq a0, t0, exit_91 
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    addi sp, sp, 12

    # read cols
    addi sp, sp, -12
    sw a1, 0(sp)
    sw a2, 4(sp)
    sw a3, 8(sp)
    li a3, 4
    mv a1, s0 
    jal fread 
    li t0, -1
    beq a0, t0, exit_91 
    lw a1, 0(sp)
    lw a2, 4(sp)
    lw a3, 8(sp)
    addi sp, sp, 12
    # 2. ends

    # 3. allocate space for matrix with rows and cols
    lw t1, 0(a1)
    lw t2, 0(a2)
    addi sp, sp, -8
    sw a1, 0(sp)
    sw a2, 4(sp)
    mul t3, t1, t2 
    slli t3, t3, 2
    mv a0, t3 
    jal malloc
    li t0, 0
    beq a0, t0, exit_88
    lw a1, 0(sp)
    lw a2, 4(sp)
    addi sp, sp, 8
    # Now a0 has the matrix pointer 
    # 3. ends 

    # 4. Use for loop to read the matrix one by one
    li t1, 0 # i = 0
    lw s1, 0(a1)
    lw s2, 0(a2)
    mv s3, a0
outer_loop_start:
    beq t1, s1, outer_loop_end
    li t2, 0 # j = 0
inner_loop_start:
    beq t2, s2, inner_loop_end
    addi sp, sp, -24
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    sw a3, 12(sp)
    sw t1, 16(sp)
    sw t2, 20(sp)
    mv a2, a0
    li a3, 4
    mv a1, s0 
    jal fread
    li t0, -1 
    beq a0, t0, exit_91 
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    lw a3, 12(sp)
    lw t1, 16(sp)
    lw t2, 20(sp)
    addi sp, sp, 24
    addi a0, a0, 4
    addi t2, t2, 1
    jal inner_loop_start
inner_loop_end:
    addi t1, t1, 1
    jal outer_loop_start
outer_loop_end:
    # 4. ends
    mv a0, s3

    # 5. close file
    addi sp, sp, -4
    sw a0, 0(sp)
    mv a1, s0
    jal fclose 
    li t0, -1
    beq a0, t0, exit_92
    lw a0, 0(sp)
    addi sp, sp, 4
    # Epilogue

    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    addi sp, sp, 20
    ret

exit_88:
    li a1, 88
    j exit2 

exit_90:
    li a1, 90
    j exit2

exit_91:
    li a1, 91
    j exit2 

exit_92:
    li a1, 92
    j exit2