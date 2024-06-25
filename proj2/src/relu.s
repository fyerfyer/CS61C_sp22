.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
#   a0 (int*) is the pointer to the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   None
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# ==============================================================================
relu:
	# Prologue
	bge x0 a1 loop_error

	addi sp sp -12
	sw s0 0(sp)
	sw s1 4(sp)
	sw ra 8(sp)

	add s0 a0 x0
	add s1 a1 x0


loop_start:
    beqz s1, loop_end        
    addi s1, s1, -1           
    slli t0, s1, 2            
    add t1, s0, t0            
    lw t2, 0(t1)              
    blt t2 x0 loop_zero   
    j loop_start           

loop_zero:
    add t2 x0 x0              
    sw t2, 0(t1)              
    j loop_start              

loop_error:
    li a0, 36                
    j exit                  

loop_end:
    # Epilogue
    lw s0, 0(sp)            
    lw s1, 4(sp)            
    lw ra, 8(sp)             
    addi sp sp 12           
    jr ra                

	ret
