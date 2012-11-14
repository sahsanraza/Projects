#Ben Isenberg
# bji6@pitt.edu




.data

.align 2

array: .space 128	#128 bytes for array of drops

g_array: .space 128	#pieces of gold

delay: .word 50000

delay2: .word 1000000	#approx. 30 seconds

End_string: .asciiz "Game Over! You is Wet ;)\n"

Win_string: .asciiz "You Win! You collected this many gold pieces: "


um_off: .asciiz "You left umbrella mode\n"

um_on: .asciiz "You entered umbrella mode\n"



.text

initialize_array:

	la $a0, array
	
	li $a1, 0	#counter
	
	li $a2, -1	#value to be entered into each spot indicating no rain drop
	
array_loop:

	sb $a2, 0($a0)
	
	addi $a1, $a1, 1	#increment counter
	
	beq $a1, 128, main
	
	addi $a0, $a0, 1	#offset to next byte in array
	
	j array_loop		#repeat

main:		

	li $k1, 0xC		#Green in LED
	li $s4, 0x40 		# RED in LED

	la $t1, 0xFFFF0008 	# at top left of LED display
	sb $s4, 0($t1)

	la $t7, 0xFFFF0008	#starting value for previous address

	li $t8, 0		#used to find byte offset

	li $t9, 0		#used to find byte offset

	lw $a2, delay

	j spinLoop
	
spinLoop:
	
	beqz $a2, add_Drop
	
	la $t2, 0xFFFF0000	#load address to find if a key was pressed
	
	lb $t3, 0($t2)
	
	beq $t3, 1, next	#if $t3 = 1 then a key was pressed
	
	subi $a2, $a2, 1
	
	j spinLoop
	
add_Drop:
	
	addi $a0, $s0, 10	#different random number everytime
	
	li $a1, 127		#upper bound = 127
	
	li $v0 42
	
	syscall			#get random number
	
	la $a1, array
	
	add $a0, $a0, $s0	#add x coord to random number
	
	bgt $a0, 127, add_Drop
	
	add $a1, $a1, $a0	#add number to array address for offset
	
	lb $v0, 0($a1)
	
	bgt $v0, -1, add_Drop	#if theres already rain there get a new random number
	
	sb $zero, 0($a1)	#store 0 in array
	
	j draw_rain
	
draw_rain:

	la $t0, 0xFFFF0008 	#start at top left of LED display

	la $a0, array
	
	move $t4, $zero		#clear $t4
	
	li $a1, 0		#counter
	
draw_loop:

	lb $t4, 0($a0)
	
	blt $t4, $zero, draw_skip	#dont draw if rain drop isnt present
	
	beq $t4, 7, Y_checker	#reset y coord if greater than 7	
	
	sll $s6, $t4, 5		#multiply y coordinate by 32
	
	srl $s7, $a1, 2		#divide x coordinate by 4
	
	add $s7, $s7, $s6	#add offsets together
	
	add $t0, $t0, $s7	#add offset to address
	
	and $s3, $a1, 0x3	#puts remainder of x/4 into $s3
	
	beq $s3, 0, rain0
	
	beq $s3, 1, rain1
	
	beq $s3, 2, rain2
	
	beq $s3, 3, rain3
	
	j draw_loop
	

death_check:

	lb $v1 32($t0)
	
	bgt $v1, 0, Furthercheck
	
	sb $zero, 0($t0)	#clear previous drop
	
	sb $k1, 32($t0)		#draw rain drop

	addi $a0, $a0, 1	#offset to next byte in array			
	
	addi $a1, $a1, 1	#increment counter
	
	la $t0, 0xFFFF0008 	#reset memory address so offset works
	
	beq $a1, 128, update_rain
	
	j draw_loop
	
Furthercheck:			#make sure its actually the player in the byte below the rain drop and not another drop

	beq $s3, 0, check0
	
	beq $s3, 1, check1
	
	beq $s3, 2, check2
	
	beq $s3, 3, check3
	
	j draw_loop

check0:

	lb $v0, 32($t0)
	
	andi $v0, $v0, 0xC0
	
	bgtz $v0, Exit
	
	sb $zero, 0($t0)	#clear previous drop
	
	sb $k1, 32($t0)		#draw rain drop

	addi $a0, $a0, 1	#offset to next byte in array			
	
	addi $a1, $a1, 1	#increment counter
	
	la $t0, 0xFFFF0008 	#reset memory address so offset works
	
	beq $a1, 128, update_rain
	
	j draw_loop
	
check1:

	lb $v0, 32($t0)
	
	andi $v0, $v0, 0x30
	
	bgtz $v0, Exit
	
	sb $zero, 0($t0)	#clear previous drop
	
	sb $k1, 32($t0)		#draw rain drop

	addi $a0, $a0, 1	#offset to next byte in array			
	
	addi $a1, $a1, 1	#increment counter
	
	la $t0, 0xFFFF0008 	#reset memory address so offset works
	
	beq $a1, 128, update_rain
	
	j draw_loop
	
check2:

	lb $v0, 32($t0)
	
	andi $v0, $v0, 0xC
	
	bgtz $v0, Exit
	
	sb $zero, 0($t0)	#clear previous drop
	
	sb $k1, 32($t0)		#draw rain drop

	addi $a0, $a0, 1	#offset to next byte in array			
	
	addi $a1, $a1, 1	#increment counter
	
	la $t0, 0xFFFF0008 	#reset memory address so offset works
	
	beq $a1, 128, update_rain
	
	j draw_loop
	
check3:

	lb $v0, 32($t0)
	
	andi $v0, $v0, 0x3
	
	bgtz $v0, Exit
	
	sb $zero, 0($t0)	#clear previous drop
	
	sb $k1, 32($t0)		#draw rain drop

	addi $a0, $a0, 1	#offset to next byte in array			
	
	addi $a1, $a1, 1	#increment counter
	
	la $t0, 0xFFFF0008 	#reset memory address so offset works
	
	beq $a1, 128, update_rain
	
	j draw_loop
	
	
rain0:
	li $k1, 0xC0
	
	beq $a3, 0, death_check
	
	sb $zero, 0($t0)	#clear previous drop
	
	sb $k1, 32($t0)		#draw rain drop

	addi $a0, $a0, 1	#offset to next byte in array			
	
	addi $a1, $a1, 1	#increment counter
	
	la $t0, 0xFFFF0008 	#reset memory address so offset works
	
	beq $a1, 128, update_rain
	
	j draw_loop
	
rain1:
	li $k1, 0x30
	
	beq $a3, 0, death_check
	
	sb $zero, 0($t0)	#clear previous drop	
	
	sb $k1, 32($t0)		#draw rain drop

	addi $a0, $a0, 1	#offset to next byte in array			
	
	addi $a1, $a1, 1	#increment counter
	
	la $t0, 0xFFFF0008 	#reset memory address so offset works	
	
	beq $a1, 128, update_rain
	
	j draw_loop
	
rain2:
	li $k1, 0xC
	
	beq $a3, 0, death_check
	
	sb $zero, 0($t0)	#clear previous drop
		
	sb $k1, 32($t0)		#draw rain drop
	
	addi $a0, $a0, 1	#offset to next byte in array	
	
	addi $a1, $a1, 1	#increment counter
	
	la $t0, 0xFFFF0008 	#reset memory address so offset works	
	
	beq $a1, 128, update_rain
	
	j draw_loop
	
rain3:
	li $k1, 0x3
	
	beq $a3, 0, death_check	
	
	sb $zero, 0($t0)	#clear previous drop	
	
	sb $k1, 32($t0)		#draw rain drop
	
	addi $a0, $a0, 1	#offset to next byte in array	
	
	addi $a1, $a1, 1	#increment counter
	
	la $t0, 0xFFFF0008 	#reset memory address so offset works	
	
	beq $a1, 128, update_rain
	
	j draw_loop
	

draw_skip:

	addi $a0, $a0, 1	#offset to next byte in array	
	
	addi $a1, $a1, 1	#increment counter
	
	beq $a1, 128, update_rain
	
	j draw_loop
	

update_rain:

	lw $a2, delay		#reseting delay for next spinloop 	

	la $a0, array
	
	move $t4, $zero		#clear $t4
	
	li $a1, 0		#counter
	
update_loop:

	la $t0, 0xFFFF0008 	#load LED memory address 	

	lb $t4, 0($a0)
	
	blt $t4, $zero, update_skip	#dont draw if rain drop isnt present
	
	addi $t4, $t4, 1
	
	sb $t4, 0($a0)		#add 1 to y coord
	
	addi $a1, $a1, 1	#increment counter
	
	addi $a0, $a0, 1	#offset to next byte in array
	
	beq $a1, 128, spinLoop	
	
	j update_loop
	
update_skip:
	
	addi $a0, $a0, 1	#offset to next byte in array	
	
	addi $a1, $a1, 1	#increment counter
	
	beq $a1, 128, spinLoop
	
	j update_loop
	
Y_checker:

	sll $s6, $t4, 5		#multiply y coordinate by 32
	
	srl $s7, $a1, 2		#divide x coordinate by 4
	
	add $s7, $s7, $s6	#add offsets together
	
	add $t0, $t0, $s7	#add offset to address
	
	sb $zero, 0($t0)

	move $t4, $zero		#set y to 0
	
	sb $t4, 0($a0)
	
	beq $a1, 128, update_rain
	
	addi $a1, $a1, 1
	
	addi $a0, $a0, 1
	
	la $t0, 0xFFFF0008 	#reset memory address so offset works
	
	j draw_loop
	
	
next:
	la $t4, 0xFFFF0004	#load address to find out which key was pressed
	
	lb $t5, 0($t4)
	
	beq $t5, 0xFFFFFFE0, moveUp
	
	beq $t5, 0xFFFFFFE1, moveDown
	
	beq $t5, 0xFFFFFFE2, moveLeft
	
	beq $t5, 0xFFFFFFE3, moveRight
	
	beq $t5, 0x00000042, umbrella_mode
	
	j next
	
umbrella_mode:

	addi $a3, $a3, 1	#umbrella mode on
	
	li $v0, 1
	
	seq $a3, $a3, $v0	# $a3 = 1 if not in mode previously, otherwise a3 = 0
	
	beq $a3, 0, umbrella_off
	
	beq $a3, 1, umbrella_on
	
	j spinLoop
	
	   
umbrella_off:

            move $a3, $zero
            
            la $a0, um_off
            li $v0, 4
            syscall
            
            j spinLoop
            
umbrella_on:

           la $a0, um_on
           li $v0, 4
           syscall
           
           j spinLoop                        
	
moveUp:

	subi $s1, $s1, 1	#subtract 1 from y coordinate of player
	
	blt $s1, 0, bad_Move1	#cant move off grid

	sb $zero, 0($t7)	#clear previous spot
	
	sll $t8, $s1, 5		#multiply y coordinate by 32
	
	srl $t9, $s0, 2		#divide x coordinate by 4
	
	add $s2, $t8, $t9	#add offsets together
	
	add $t1, $t1, $s2	#add offset to address
	
	move $t7, $t1		#save previous address to turn off correct LED
	
	sb $s4, 0($t1)
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop
	
moveDown:

	addi $s1, $s1, 1	#add 1 to y coordinate of player
	
	bgt $s1, 7, bad_Move2	#cant move off grid

	sb $zero, 0($t7)	#clear previous spot
	
	sll $t8, $s1, 5		#multiply y coordinate by 32
	
	srl $t9, $s0, 2		#divide x coordinate by 4
	
	add $s2, $t8, $t9	#add offsets together
	
	add $t1, $t1, $s2	#add offset to address
	
	move $t7, $t1		#save previous address to turn off correct LED
	
	sb $s4, 0($t1)		#light up correct LED
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop
	
moveLeft:

	subi $s0, $s0, 1	#subtract 1 from x coordinate of player
	
	blt $s0, 0, bad_Move3	#cant move off grid
	
	sb $zero, 0($t7)	#clear previous spot
	
	srl $t9, $s0, 2		#divide x coordinate by 4
	
	sll $t8, $s1, 5		#multiply y coordinate by 32
	
	add $s2, $t8, $t9	#add offsets together
	
	add $t1, $t1, $s2	#add offset to address

	move $t7, $t1		#save previous address to turn off correct LED
	
	and $s3, $s0, 0x3	#puts remainder of x/4 into $s3
	
	beq $s3, 0, remainder0
	
	beq $s3, 1, remainder1
	
	beq $s3, 2, remainder2
	
	beq $s3, 3, remainder3
	

moveRight:

	addi $s0, $s0, 1	#add 1 to x coordinate of player
	
	bge $s0, 127, Gold	#gold if player makes it across

	sb $zero, 0($t7)	#clear previous spot
	
	srl $t9, $s0, 2		#divide x coordinate by 4
	
	sll $t8, $s1, 5		#multiply y coordinate by 32
	
	add $s2, $t8, $t9	#add offsets together
	
	add $t1, $t1, $s2	#add offset to address

	move $t7, $t1		#save previous address to turn off correct LED
	
	and $s3, $s0, 0x3	#puts remainder of x/4 into $s3
	
	beq $s3, 0, remainder0
	
	beq $s3, 1, remainder1
	
	beq $s3, 2, remainder2
	
	beq $s3, 3, remainder3
	

	
remainder0:

	lb $s4, 0($t1)		#load byte that you want to change from memory
	
	and $s4, $s4, 0x3F	#clear first two bits in byte for placing 01
	
	or $s4, $s4, 0x40	#light LED in first position
	
	sb $s4, 0($t1)
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop
	
remainder1:

	lb $s4, 0($t1)		#load byte that you want to change from memory
	
	and $s4, $s4, 0xCF	#clear second two bits for placing 01
	
	or $s4, $s4, 0x10	#light LED in second position
	
	sb $s4, 0($t1)
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop
	
remainder2:

	lb $s4, 0($t1)		#load byte that you want to change from memory
	
	and $s4, $s4, 0xF3	#clear third two bits for placing 01
	
	or $s4, $s4, 0x4	#light LED in third position
	
	sb $s4, 0($t1)
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop
	
remainder3:

	lb $s4, 0($t1)		#load byte that you want to change from memory
	
	and $s4, $s4, 0xFC	#clear last two bits for placing 01
	
	or $s4, $s4, 0x1	#light LED in last position

	sb $s4, 0($t1)
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop
	

bad_Move1:

	addi $s1, $s1, 1		#add 1 back to y
	
	j spinLoop			#return to main
	
bad_Move2:

	subi $s1, $s1, 1		#subtract 1 from y
	
	j spinLoop			#return to main	
	
bad_Move3:

	addi $s0, $s0, 1		#add 1 back to x
	
	j spinLoop			#return to main
	

Gold:				#determines amount of gold that should appear

	la $t0, array
	
	move $a0, $zero
	
Gold_loop:

	beq $a0, 128, Draw_gold

	lb $k1, 0($t0)
	
	addi $t0, $t0, 1	#moves to next byte
	
	addi $a0, $a0, 1	#counts array
	
	blt $k1, 0, Gold_loop
	
	addi $v0, $v0, 1	#counts rain
	
	j Gold_loop
	
Draw_gold:

	li $v1, 127
	
	sub $t5, $v1, $v0	# $t5 = amount of gold
	
	la $t0, 0xFFFF0008
	
Clear:				#clears all LEDs
	
	sb $zero, 0($t0)
	
	addi $t0, $t0, 1
	
	beq $t0, 0xFFFF0108, More_draw
	
	j Clear
	

More_draw:		#finds random x and y coordinates for all the gold pieces
	
	li $a1, 126	#upper bound = 126
	
	li $v0 42
	
	syscall		#get random number
	
	la $v1, g_array
	
	add $v1, $v1, $a0	#add num to array address for offset
	
	lb $v0, 0($v1)	
	
	bgt $v0, 0, More_draw
	
	li $a1, 8	#upper bound = 8
	
	li $v0 42
	
	syscall		#get random number
	
	sb $a0, 0($v1)
	
	addi $t5, $t5, -1	#places correct amount of gold in array
	
	beqz $t5, drawing
	
	j More_draw
	
	
drawing:

	la $a1, g_array
	
	la $a0, 0xFFFF0008
	
	li $k0, -1		#counts for x
	
drawing_loop:			#draws gold pieces
	
	addi $k0, $k0, 1
	
	beq $k0, 128, initialize_gold
	
	lb $v0, 0($a1)
	
	addi $a1, $a1, 1
	
	beqz $v0, drawing_loop
	
	srl $k1, $k0, 2		#divide x coordinate by 4
	
	sll $v0, $v0, 5		#multiply y coordinate by 32
	
	add $v0, $k1, $v0	#add offsets together
	
	add $a0, $a0, $v0	#add offset to address
	
	and $k1, $k0, 0x3	#puts remainder of x/4 into $k1
	
	beq $k1, 0, g_remainder0
	
	beq $k1, 1, g_remainder1
	
	beq $k1, 2, g_remainder2
	
	beq $k1, 3, g_remainder3
	
g_remainder0:
	
	li $s4, 0x80	#light LED in first position
	
	sb $s4, 0($a0)
	
	la $a0, 0xFFFF0008
	
	j drawing_loop
	

g_remainder1:
	
	li $s4, 0x20	#light LED in second position
	
	sb $s4, 0($a0)
	
	la $a0, 0xFFFF0008
	
	j drawing_loop
	
	
g_remainder2:

	li $s4, 0x8	#light LED in third position
	
	sb $s4, 0($a0)
	
	la $a0, 0xFFFF0008
	
	j drawing_loop
	
g_remainder3:
	
	li $s4, 0x2	#light LED in last position
	
	sb $s4, 0($a0)
	
	la $a0, 0xFFFF0008
	
	j drawing_loop
	

initialize_gold:

	li $s4, 0x40 		# RED in LED

	la $t1, 0xFFFF0008 	# start at top left of LED display
	sb $s4, 0($t1)
	
	li $s4, -1		#counter
	
	lw $a2, delay2		
	
	li $s0, 0		#reset x coord of player
	li $s1, 0		#reset y coord of player	
	
																																														
spinLoop2:
	li $a3, 0	#counter
	
	beqz $a2, win
	
	la $t2, 0xFFFF0000	#load address to find if a key was pressed
	
	lb $t3, 0($t2)
	
	beq $t3, 1, next_gold	#if $t3 = 1 then a key was pressed
	
	subi $a2, $a2, 1
	
	j spinLoop2
	

#Gold_counter:		#THIS FUNCTION DOESNT WORK BUT IT WAS MEANT TO COUNT HOW MUCH GOLD THE PLAYER COLLECTED

#	beq $s4, 128, spinLoop2
#
#	addi $s4, $s4, 1	#counter
#
#	la $t0, g_array
#	
#	lb $v0, 0($t0)
#
#	addi $t0, $t0, 1
#	
#	bne $s4, $s0, Gold_counter
#	
#	bne $v0, $s1, Gold_counter
#	
#	addi $a3, $a3, 1
#	
#	j Gold_counter
	
	
	
win:		#prints that the player won and exits program

	la $a0 Win_string
	li $v0, 4
	syscall
	
	move $a0, $a3	# $a3 = amount of gold collected
	li $v0, 1
	syscall

	li $v0,10
	syscall	
		
	
next_gold:

	la $t4, 0xFFFF0004	#load address to find out which key was pressed
	
	lb $t5, 0($t4)
	
	beq $t5, 0xFFFFFFE0, moveUp_gold
	
	beq $t5, 0xFFFFFFE1, moveDown_gold
	
	beq $t5, 0xFFFFFFE2, moveLeft_gold
	
	beq $t5, 0xFFFFFFE3, moveRight_gold
	
moveUp_gold:	
	
	subi $s1, $s1, 1	#subtract 1 from y coordinate of player
	
	blt $s1, 0, bad_Move1_gold	#cant move off grid

	sb $zero, 0($t7)	#clear previous spot
	
	sll $t8, $s1, 5		#multiply y coordinate by 32
	
	srl $t9, $s0, 2		#divide x coordinate by 4
	
	add $s2, $t8, $t9	#add offsets together
	
	add $t1, $t1, $s2	#add offset to address
	
	move $t7, $t1		#save previous address to turn off correct LED
	
	sb $s4, 0($t1)		#light up correct LED
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop2
	
moveDown_gold:

	addi $s1, $s1, 1	#add 1 to y coordinate of player
	
	bgt $s1, 7, bad_Move2_gold	#cant move off grid

	sb $zero, 0($t7)	#clear previous spot
	
	sll $t8, $s1, 5		#multiply y coordinate by 32
	
	srl $t9, $s0, 2		#divide x coordinate by 4
	
	add $s2, $t8, $t9	#add offsets together
	
	add $t1, $t1, $s2	#add offset to address
	
	move $t7, $t1		#save previous address to turn off correct LED
	
	sb $s4, 0($t1)		#light up correct LED
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop2
	
moveLeft_gold:

	subi $s0, $s0, 1	#subtract 1 from x coordinate of player
	
	blt $s0, 0, bad_Move3_gold	#cant move off grid
	
	sb $zero, 0($t7)	#clear previous spot
	
	srl $t9, $s0, 2		#divide x coordinate by 4
	
	sll $t8, $s1, 5		#multiply y coordinate by 32
	
	add $s2, $t8, $t9	#add offsets together
	
	add $t1, $t1, $s2	#add offset to address

	move $t7, $t1		#save previous address to turn off correct LED
	
	and $s3, $s0, 0x3	#puts remainder of x/4 into $s3
	
	beq $s3, 0, remainder0_gold
	
	beq $s3, 1, remainder1_gold
	
	beq $s3, 2, remainder2_gold
	
	beq $s3, 3, remainder3_gold
	

moveRight_gold:

	addi $s0, $s0, 1	#add 1 to x coordinate of player
	
	bge $s0, 127, spinLoop2	#gold if player makes it across

	sb $zero, 0($t7)	#clear previous spot
	
	srl $t9, $s0, 2		#divide x coordinate by 4
	
	sll $t8, $s1, 5		#multiply y coordinate by 32
	
	add $s2, $t8, $t9	#add offsets together
	
	add $t1, $t1, $s2	#add offset to address

	move $t7, $t1		#save previous address to turn off correct LED
	
	and $s3, $s0, 0x3	#puts remainder of x/4 into $s3
	
	beq $s3, 0, remainder0_gold
	
	beq $s3, 1, remainder1_gold
	
	beq $s3, 2, remainder2_gold
	
	beq $s3, 3, remainder3_gold
	


remainder0_gold:
	lb $s4, 0($t1)		#load byte that you want to change from memory
	
	and $s4, $s4, 0x3F	#clear first location for placing 10
	
	or $s4, $s4, 0x40	#light LED in first position
	
	sb $s4, 0($t1)
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop2


remainder1_gold:

	lb $s4, 0($t1)		#load byte that you want to change from memory
	
	and $s4, $s4, 0xCF	#clear second location for placing 10
	
	or $s4, $s4, 0x10	#light LED in second position
	
	sb $s4, 0($t1)
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop2

remainder2_gold:

	lb $s4, 0($t1)		#load byte that you want to change from memory
	
	and $s4, $s4, 0xF3	#clear third location for placing 10
	
	or $s4, $s4, 0x4	#light LED in third position
	
	sb $s4, 0($t1)
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop2
	

remainder3_gold:
	lb $s4, 0($t1)		#load byte that you want to change from memory
	
	and $s4, $s4, 0xFC	#clear last location for placing 10
	
	or $s4, $s4, 0x1	#light LED in last position

	sb $s4, 0($t1)
	
	la $t1, 0xFFFF0008 	#reset memory address so offset works
	
	j spinLoop2


bad_Move1_gold:

	addi $s1, $s1, 1	#add 1 back to y
	
	j spinLoop2			#return to spinLoop2
	
bad_Move2_gold:

	subi $s1, $s1, 1	#subtract 1 from y
	
	j spinLoop2			#return to spinLoop2
	
bad_Move3_gold:

	addi $s0, $s0, 1	#add 1 back to x
	
	j spinLoop2			#return to spinLoop2


Exit:				#player is hit by rain so he loses and program exits

	la $a0 End_string
	li $v0, 4
	syscall

	li $v0,10
	syscall
