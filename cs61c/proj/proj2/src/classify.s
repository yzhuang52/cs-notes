.globl classify

.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 89.
    # - If malloc fails, this function terminats the program with exit code 88.
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>
    # C source code
    # int* m0_rows = malloc(sizeof(int));
    # int* m0_cols = malloc(sizeof(int));
    # int* m1_rows = malloc(sizeof(int));
    # int* m1_cols = malloc(sizeof(int));
    # int* input_rows = malloc(sizeof(int));
    # int* input_cols = malloc(sizeof(int));
    # int* m0 = read_matrix(M0_PATH, m0_rows, m0_cols)
    # int* m1 = read_matrix(M1_PATH, m1_rows, m1_cols)
    # int* input = read_matrix(INPUT_PATH, input_rows, input_cols)
    # int* layer1 = malloc(sizeof(int) * (*m0_rows) * (*input_cols));
    # int* layer2 = malloc(sizeof(int) * (*m1_rows) * (*m0_cols));
    # matmul(m0, (*m0_rows), (*m1_cols), input, (*input_rows), (*input_cols), layer1);
    # ReLU(layer1)
    # matmul(m1, (*m1_rows), (*m1_cols), layer1, (*m0_rows), (*input_cols), layer2);
    # return argmax(layer2, (*m1_rows) * (*m0_cols));
    li t0, 5
    bne a0, t0, exit_89
    addi sp, sp, -52
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    sw s3, 16(sp)
    sw s4, 20(sp)
    sw s5, 24(sp)
    sw s6, 28(sp)
    sw s7, 32(sp)
    sw s8, 36(sp)
    sw s9, 40(sp)
    sw s10, 44(sp)
    sw s11, 48(sp)
	# =====================================
    # LOAD MATRICES
    # =====================================
    # Load pretrained m0
    # m0_rows -> s0, m0_cols -> s1, m1_rows -> s2, m1_cols -> s3
    # input_rows -> s4, input_cols -> s5 
    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 4
    jal malloc 
    beq a0, x0, exit_88
    mv s0, a0 
    lw a0, 0(sp)
    lw a1, 4(sp)
    addi sp, sp, 8

    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 4
    jal malloc 
    beq a0, x0, exit_88
    mv s1, a0
    lw a0, 0(sp)
    lw a1, 4(sp)
    addi sp, sp, 8

    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 4
    jal malloc 
    beq a0, x0, exit_88
    mv s2, a0
    lw a0, 0(sp)
    lw a1, 4(sp)
    addi sp, sp, 8

    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 4
    jal malloc 
    beq a0, x0, exit_88
    mv s3, a0
    lw a0, 0(sp)
    lw a1, 4(sp)
    addi sp, sp, 8

    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 4
    jal malloc 
    beq a0, x0, exit_88
    mv s4, a0
    lw a0, 0(sp)
    lw a1, 4(sp)
    addi sp, sp, 8

    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    li a0, 4
    jal malloc 
    beq a0, x0, exit_88
    mv s5, a0
    lw a0, 0(sp)
    lw a1, 4(sp)
    addi sp, sp, 8

    # Load pretrained m0
    addi sp, sp, -12
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    lw a0, 4(a1)
    mv a1, s0
    mv a2, s1
    jal read_matrix 
    mv s6, a0 # s6 hold pointer to m0
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

    # Load pretrained m1
    addi sp, sp, -12
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    lw a0, 8(a1)
    mv a1, s2
    mv a2, s3
    jal read_matrix 
    mv s7, a0 # s7 hold pointer to m1
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

    # Load input matrix
    addi sp, sp, -12
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    lw a0, 12(a1)
    mv a1, s4
    mv a2, s5
    jal read_matrix 
    mv s8, a0 # s8 hold pointer to input
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)

    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    lw t0, 0(s0)
    lw t1, 0(s5)
    li t2, 4
    mul t2, t2, t0
    mul t2, t2, t1
    mv t2, a0
    jal malloc 
    beq a0, x0, exit_88
    mv s9, a0 # s9 holds pointer to layer1
    lw a0, 0(sp)
    lw a1, 4(sp)

    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    lw t0, 0(s2)
    lw t1, 0(s1)
    li t2, 4
    mul t2, t2, t0
    mul t2, t2, t1
    mv t2, a0
    jal malloc 
    beq a0, x0, exit_88
    mv s10, a0 # s10 holds pointer to layer2
    lw a0, 0(sp)
    lw a1, 4(sp)

    # layer1 = m0 * m1
    addi sp, sp, -12
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    mv a0, s6 
    lw a1, 0(s1)
    lw a2, 0(s2)
    mv a3, s8
    lw a4, 0(s4)
    lw a5, 0(s5)
    mv a6, s9
    jal matmul 
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp) 
    
    # call relu
    addi sp, sp, -8
    sw a0, 0(sp)
    sw a1, 4(sp)
    mv a0, s9
    lw t0, 0(s0)
    lw t1, 0(s5)
    mul t1, t1, t0
    mv a1, t1 
    jal relu
    lw a0, 0(sp)
    lw a1, 4(sp)
    addi sp, sp, 8

    # layer2 = m1 * layer1
    # matmul(m1, (*m1_rows), (*m1_cols), layer1, (*m0_rows), (*input_cols), layer2);
    
    addi sp, sp, -12
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    mv a0, s7 
    lw a1, 0(s2)
    lw a2, 0(s3)
    mv a3, s9
    lw a4, 0(s0)
    lw a5, 0(s5)
    mv a6, s10
    jal matmul 
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp) 

     
    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix

    addi sp, sp, 12
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    lw a0, 16(a1)
    mv a1, s10 
    lw a2, 0(s0)
    lw a3, 0(s5)
    jal write_matrix
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax

    # argmax(layer2, (*m1_rows) * (*m0_cols));
    addi sp, sp, -12
    sw a0, 0(sp)
    sw a1, 4(sp)
    sw a2, 8(sp)
    lw t0, 0(s0)
    lw t1, 0(s5)
    mul t1, t1, t0
    mv a1, t1 
    mv a0, s10
    jal argmax 
    mv s11, a0 # s11 store the classification result
    lw a0, 0(sp)
    lw a1, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12
    # Print classification
    beq a2, x0, exit
    mv a1, s11
    jal print_int


    # Print newline afterwards for clarity
    li a1, '\n'
    li a0, 11
    ecall 
exit: 

    mv a0, s11
    lw ra, 0(sp)
    lw s0, 4(sp)
    lw s1, 8(sp)
    lw s2, 12(sp)
    lw s3, 16(sp)
    lw s4, 20(sp)
    lw s5, 24(sp)
    lw s6, 28(sp)
    lw s7, 32(sp)
    lw s8, 36(sp)
    lw s9, 40(sp)
    lw s10, 44(sp)
    lw s11, 48(sp)
    addi, sp, sp, 52
    ret

exit_88:
    li a1, 88
    j exit2 

exit_89:
    li a1, 89
    j exit2

