.globl argmax

.text
# =================================================================
# FUNCTION: Given a int array, return the index of the largest
#   element. If there are multiple, return the one
#   with the smallest index.
# Arguments:
#   a0 (int*) is the pointer to the start of the array
#   a1 (int)  is the # of elements in the array
# Returns:
#   a0 (int)  is the first index of the largest element
# Exceptions:
#   - If the length of the array is less than 1,
#     this function terminates the program with error code 36
# =================================================================
argmax:
	# Prologue
	bge x0 a1 loop_error

	addi sp sp -20
	sw s0 0(sp)  # array address
	sw s1 4(sp)  # index 
	sw s2 8(sp)  # maximun number
	sw s3 12(sp) # maximun index
	sw ra 16(sp)

	add s0 a0 x0
	add s1 a1 x0 

	# set the first element of the array to be maximum first
	add s3 x0 x0
	lw s2 0(s0)


loop_start:
    beqz s1 loop_end
	addi s1 s1 -1
	slli t0 s1 2
	add t1 s0 t0 
	lw t2 0(t1)  # get arr[s1]
	bgt t2 s2 loop_change
	j loop_start

loop_change:
	mv s2 t2
	mv s3 s1
	j loop_start

loop_error:
	li a0 36
	j exit

loop_end:
	# Epilogue
	mv a0 s3
	lw s0 0(sp)
	lw s1 4(sp)
	lw s2 8(sp)
	lw s3 12(sp)
	lw ra 16(sp)
	addi sp sp 20
	jr ra

	ret
