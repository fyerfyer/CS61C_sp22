.globl matmul

.text
matmul:
    # Step 1: Error checking
    bge x0, a1, loop_error
    bge x0, a2, loop_error  
    bge x0, a4, loop_error 
    bge x0, a5, loop_error 
    bne a2, a4, loop_error  

    # Prologue
    addi sp sp -16    
    sw s0 0(sp)           
    sw s1 4(sp)               # s1 stores the current col of Matrix B, it's added by 1
    sw s2 8(sp)               # s2 stores the current row of Matrix A, it's added by a2
	sw ra 12(sp)              

    li s0 0                
    li t0 0              
    li t3 0              
    slli t1 a2 2              # a row of Matrix A takes 4*a2 bytes

outer_loop_start:
    bge s0 a1 outer_loop_end  
    li s1 0                    

inner_loop_start:
    bge s1 a5 inner_loop_end 

    addi sp sp -44   
    sw a0 0(sp)
    sw a1 4(sp)
    sw a2 8(sp)
    sw a3 12(sp)
    sw a4 16(sp)
    sw a5 20(sp)
    sw a6 24(sp)
    sw t0 28(sp)
    sw t1 32(sp)
    sw t3 36(sp)
	sw ra 40(sp)

    # Prepare to jump to dot
    slli t2 s1 2           
    add a0 a0 t0           
    add a1 a3 t2           
    mv a2 a2                
    li a3 1                 
    mv a4 a5                

    jal ra dot             

    lw t3 36(sp)            
    lw a6 24(sp)            
    slli t4 t3, 2           
    add t4 t4, a6           
    sw a0 0(t4)          
    addi t3 t3, 1          

    lw ra 40(sp)        
    lw a0 0(sp)
    lw a1 4(sp)
    lw a2 8(sp)
    lw a3 12(sp)
    lw a4 16(sp)
    lw a5 20(sp)
    lw t0 28(sp)
    lw t1 32(sp)
    addi sp sp, 44       

    addi s1 s1 1         
    j inner_loop_start     

inner_loop_end:
    addi s0 s0 1           
    add t0 t0 t1           
    j outer_loop_start       


loop_error:
    li a0, 38                
    j exit   

outer_loop_end:
    # Epilogue

    lw s0 0(sp)             
    lw s1 4(sp)             
    lw s2 8(sp)         
	lw ra 12(sp)    
    addi sp sp 16    
    jr ra                    
                
