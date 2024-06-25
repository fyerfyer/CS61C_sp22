.globl dot

.text
# =======================================================
# FUNCTION: Dot product of 2 int arrays
# Arguments:
#   a0 (int*) is the pointer to the start of arr0
#   a1 (int*) is the pointer to the start of arr1
#   a2 (int)  is the number of elements to use
#   a3 (int)  is the stride of arr0
#   a4 (int)  is the stride of arr1
# Returns:
#   a0 (int)  is the dot product of arr0 and arr1
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
#   - If the stride of either array is less than 1,
#     this function terminates the program with error code 37
# =======================================================
dot:

	# Prologue
	ble a2 x0 loop_error1
	ble a3 x0 loop_error2
	ble a4 x0 loop_error2

	addi sp sp -28
	sw s0 0(sp)    # arr1
	sw s1 4(sp)    # arr2
	sw s2 8(sp)    # counter
	sw s3 12(sp)   # index1
	sw s4 16(sp)   # index2
	sw s5 20(sp)   # sum
	sw ra 24(sp)

	add s0 a0 x0
	add s1 a1 x0
	add s2 x0 x0
	add s3 x0 x0
	add s4 x0 x0
	add s5 x0 x0

loop_start:
	beq s2 a2 loop_end

	# arr1
	slli t0 s3 2
	add t1 s0 t0
	lw t2 0(t1)

	#arr2
	slli t3 s4 2
	add t4 s1 t3
	lw t5 0(t4)

	mul t6 t2 t5
	add s5 s5 t6

	addi s2 s2 1
	add s3 s3 a3
	add s4 s4 a4
	j loop_start 

loop_error1:
	li a0 36
	j exit

loop_error2:
	li a0 37
	j exit

loop_end:
	# Epilogue
	mv a0 s5
	lw s0 0(sp)    
	lw s1 4(sp)    
	lw s2 8(sp)    
	lw s3 12(sp)   
	lw s4 16(sp)   
	lw s5 20(sp)   
	lw ra 24(sp)
	addi sp sp 28

	jr ra

	ret
