# Name - Ben Isenberg
# Email - bji6@pitt.edu
	        .data
	        buf1: .space 64		#stores user input
	        buf2: .space 64		#stores correct answer
	        
	        newline: .asciiz "\n"
	        answer: .asciiz "Correct Answer: "
	 
	fontset:	.half
		##########################
		0xF00F, #--XXXX--   0000 1111 1111 0000    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0xF00F, #--XXXX--   0000 1111 1111 0000   
		##########################
		0xC000, #----X---   0000 0000 1100 0000    
		0xC003, #---XX---   0000 0011 1100 0000   
		0xC00C, #--X-X---   0000 1100 1100 0000    
		0xC000, #----X---   0000 0000 1100 0000    
		0xC000, #----X---   0000 0000 1100 0000    
		0xC000, #----X---   0000 0000 1100 0000    
		0xC000, #----X---   0000 0000 1100 0000    
		0xFC3f, #-XXXXXX-   0011 1111 1111 1100   
		##########################
		0xF00F, #--XXXX--   0000 1111 1111 0000    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x3000, #-----X--   0000 0000 0011 0000   
		0xC000, #----X---   0000 0000 1100 0000    
		0x0003, #---X----   0000 0011 0000 0000    
		0x000C, #--X-----   0000 1100 0000 0000    
		0x0030, #-X------   0011 0000 0000 0000    
		0xFC3F, #-XXXXXX-   0011 1111 1111 1100    
		##########################
		0xF03F, #-XXXXX--   0011 1111 1111 0000    
		0x0C00, #------X-   0000 0000 0000 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0xF03F, #-XXXXX--   0011 1111 1111 0000    
		0x0C00, #------X-   0000 0000 0000 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0xFC3F, #-XXXXXX-   0011 1111 1111 1100    
		##########################
		0x0C00, #------X-   0000 0000 0000 1100    
		0x3C00, #-----XX-   0000 0000 0011 1100    
		0xCC00, #----X-X-   0000 0000 1100 1100    
		0x0C03, #---X--X-   0000 0011 0000 1100    
		0xFC0F, #--XXXXX-   0000 1111 1111 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		##########################
		0xFC3F, #-XXXXXX-   0011 1111 1111 1100    
		0x0030, #-X------   0011 0000 0000 0000    
		0x0030, #-X------   0011 0000 0000 0000    
		0xF03F, #-XXXXX--   0011 1111 1111 0000    
		0x0C00, #------X-   0000 0000 0000 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0xF03F, #-XXXXX--   0011 1111 1111 0000    
		##########################
		0x3000, #-----X--   0000 0000 0011 0000    
		0xC000, #----X---   0000 0000 1100 0000    
		0x0003, #---X----   0000 0011 0000 0000    
		0xF00F, #--XXXX--   0000 1111 1111 0000
		0x0C30, #-X----X-   0011 0000 0000 1100    	
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0xF00F, #--XXXX--   0000 1111 1111 0000    
		##########################
		0xFC3F, #-XXXXXX-   0011 1111 1111 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0x3000, #-----X--   0000 0000 0011 0000    
		0xC000, #----X---   0000 0000 1100 0000    
		0x0003, #---X----   0000 0011 0000 0000    
		0x000C, #--X-----   0000 1100 0000 0000    
		0x0030, #-X------   0011 0000 0000 0000    
		0x0030, #-X------   0011 0000 0000 0000    
		##########################
		0xF00F, #--XXXX--   0000 1111 1111 0000    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0xF00F, #--XXXX--   0000 1111 1111 0000    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x0C30, #-X----X-   0011 0000 0000 1100   
		0xF00F, #--XXXX--   0000 1111 1111 0000    
		##########################
		0xF00F, #--XXXX--   0000 1111 1111 0000    
		0x0C30, #-X----X-   0011 0000 0000 1100    
		0x0C30, #-X----X-   0011 0000 0000 1100   
		0xFC0F, #--XXXXX-   0000 1111 1111 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0x0C00, #------X-   0000 0000 0000 1100    
		0xFC3F, #-XXXXXX-   0011 1111 1111 1100
		########################## (CARD 10)
		0xFC3F, #-XXXXXX-   0011 1111 1111 1100
		0xFC3F,	#-XXXXXX-   0011 1111 1111 1100
		0xFC3F,	#-XXXXXX-   0011 1111 1111 1100
		0xFC3F,	#-XXXXXX-   0011 1111 1111 1100
		0xFC3F,	#-XXXXXX-   0011 1111 1111 1100
		0xFC3F,	#-XXXXXX-   0011 1111 1111 1100
		0xFC3F,	#-XXXXXX-   0011 1111 1111 1100
		0xFC3F,	#-XXXXXX-   0011 1111 1111 1100
		########################## (CARD 11)
		0x0000, #--------   0000 0000 0000 0000
		0x2828, #-XX--XX-   0010 1000 0010 1000
		0x0000, #--------   0000 0000 0000 0000
	     	0x0820, #-X----X-   0010 0000 0000 1000
		0x2008, #--X--X--   0000 1000 0010 0000
		0x8002, #---XX---   0000 0010 1000 0000
		0x0000, #--------   0000 0000 0000 0000
		0x0000, #--------   0000 0000 0000 0000
		########################## (CARD 12)
		0x0000, #--------   0000 0000 0000 0000
		0x1414, #-XX--XX-   0001 0100 0001 0100
		0x0000, #--------   0000 0000 0000 0000
		0x4001, #---XX---   0000 0001 0100 0000
		0x1004, #--x--X--   0000 0100 0001 0000
		0x0410, #-X----X-   0001 0000 0000 0100
		0x0000, #--------   0000 0000 0000 0000
		0x0000, #--------   0000 0000 0000 0000
		########################## (CARD 13)
		0x0000, #--------   0000 0000 0000 0000
		0x0000, #--------   0000 0000 0000 0000
		0x0000, #--------   0000 0000 0000 0000
		0x0000, #--------   0000 0000 0000 0000
		0x0000, #--------   0000 0000 0000 0000
		0x0000, #--------   0000 0000 0000 0000
		0x0000, #--------   0000 0000 0000 0000
		0x0000  #--------   0000 0000 0000 0000	

	.text
	
	addi $k1, $zero, 1	#Counter to end game after 8 rounds
	
	la $t9, buf2		#buffer keeps track of random #'s
	
	li $t8, 0xD		#checks for user pressing return
	
	li $v0, 4
	la $a0, answer
	syscall
	
	j Start
	
Start:
	beq $k1, 9, ExitWin	#after 8 turns end game
	
	add $a0, $zero, $zero
	addi $a1, $zero, 9
	
	li $v0, 42		#random number
	syscall
	
	li $v0, 1		#print answer
	syscall
	
	move $t2, $a0		#number put in $t2
	
	sb $t2, 0($t9)		#place number in buffer
	
	addi $t9, $t9, 1	#go to next spot in memory
	
	beq $t2, 0, DrawZero
	beq $t2, 1, DrawOne
	beq $t2, 2, DrawTwo
	beq $t2, 3, DrawThree
	beq $t2, 4, DrawFour
	beq $t2, 5, DrawFive
	beq $t2, 6, DrawSix
	beq $t2, 7, DrawSeven
	beq $t2, 8, DrawEight
	beq $t2, 9, DrawNine
	
	
	
	
	
DrawBack:
	li $s0, 0xFC3F
	
	sh $s0, 0($t0)		#Display back of card
	sh $s0, 32($t0)
	sh $s0, 64($t0)
	sh $s0, 96($t0)
	sh $s0, 128($t0)
	sh $s0, 160($t0)
	sh $s0, 192($t0)
	sh $s0, 224($t0)
	
	li $a0, 250		# pause program for 1/4 sec in between cards being delt
	li $v0, 32
	syscall
	
	beq $k0, $k1, User
	
	j Start
	
	
User:	
	li $v0, 4		#print newline after answer is printed
	la $a0, newline
	syscall 

	li $v0, 8
	la $a0, buf1 
	addi $a1, $k0, 1	#reads n-1 characters because of null character at end
	syscall
	
	la $t9, buf2		#reset where u are in buffer
	
Check:

	lb $t7, 0($a0)
	lb $s7, 0($t9)
	
	beq $t7, $t8, Lose	#ends game if user presses enter
	
	beq $t7, $zero, Prepare   #start next round when null byte is reached
	
	sub $t7, $t7, 48	#convert hex ascii code to decimal number
	
	bne $t7, $s7, Lose	#end game if user types in wrong number
	
	addi $a0, $a0, 1	#go to next number in memory
	addi $t9, $t9, 1	#go to next number in memory
	
	j Check
	
	
Prepare:
	
	addi $k0, $zero, 0	#clear card position tracker if user guessed number correctly
	
	addi $k1, $k1, 1	#increment round counter
	
	la $t9, buf2		#reset where u are in buffer
	
	li $v0, 4		#print newline before answer is printed
	la $a0, newline
	syscall 
	
	li $v0, 4
	la $a0, answer
	syscall
	
	j Reset		#go to beginning
	
	
DrawZero:
	li $t3, 0xF00F		#Load fonts for 0
	li $t4, 0x0C30
	
	addi $k0, $k0, 1 	#increment card position tracker
	
	jal Jump
	
	
	sh $t3, 0($t0)		#Display 0
	sh $t4, 32($t0)
	sh $t4, 64($t0)
	sh $t4, 96($t0)
	sh $t4, 128($t0)
	sh $t4, 160($t0)
	sh $t4, 192($t0)
	sh $t3, 224($t0)
	
	
	li $a0, 1000		# pause program for 1 sec
	li $v0, 32
	syscall
	
	j DrawBack
	
Jump:
	beq $k0, 1, FirstPosition	#Check what position to draw card
	beq $k0, 2, SecondPosition
	beq $k0, 3, ThirdPosition
	beq $k0, 4, FourthPosition
	beq $k0, 5, FifthPosition
	beq $k0, 6, SixthPosition
	beq $k0, 7, SeventhPosition
	beq $k0, 8, EighthPosition


DrawOne:
	li $t3, 0xC000		#Load fonts for 1
	li $t4, 0xC003
	li $t5, 0xC00C
	li $t6, 0xFC3f
	
	addi $k0, $k0, 1 	#increment card position tracker

	jal Jump
	
	sh $t3, 0($t0)		#Display 1
	sh $t4, 32($t0)
	sh $t5, 64($t0)
	sh $t3, 96($t0)
	sh $t3, 128($t0)
	sh $t3, 160($t0)
	sh $t3, 192($t0)
	sh $t6, 224($t0)
	
	li $a0, 1000		# pause program for 1 sec
	li $v0, 32
	syscall	
	
	j DrawBack
	

DrawTwo:

	li $s0, 0xF00F

	addi $k0, $k0, 1 	#increment card position tracker

	jal Jump
	
	sh $s0, 0($t0)		#Display 2
	li $s0, 0x0C30
	sh $s0, 32($t0)
	li $s0, 0x3000
	sh $s0, 64($t0)
	li $s0, 0xC000
	sh $s0, 96($t0)
	li $s0, 0x0003
	sh $s0, 128($t0)
	li $s0, 0x000C
	sh $s0, 160($t0)
	li $s0, 0x0030
	sh $s0, 192($t0)
	li $s0, 0xFC3F
	sh $s0, 224($t0)
	
	li $a0, 1000		# pause program for 1 sec
	li $v0, 32
	syscall	
	
	j DrawBack


DrawThree:
	li $t3, 0xF03F		#Load fonts for 3
	li $t4, 0x0C00
	li $t5, 0xFC3F

	addi $k0, $k0, 1 	#increment card position tracker
	
	jal Jump
	
	sh $t3, 0($t0)		#Display 3
	sh $t4, 32($t0)
	sh $t4, 64($t0)
	sh $t3, 96($t0)
	sh $t4, 128($t0)
	sh $t4, 160($t0)
	sh $t4, 192($t0)
	sh $t5, 224($t0)
	
	li $a0, 1000		# pause program for 1 sec
	li $v0, 32
	syscall	
	
	j DrawBack	


DrawFour:
	li $s0, 0x0C00
	
	addi $k0, $k0, 1 	#increment card position tracker

	jal Jump
	
	sh $s0, 0($t0)		#Display 4
	li $s0, 0x3C00
	sh $s0, 32($t0)
	li $s0, 0xCC00
	sh $s0, 64($t0)
	li $s0, 0x0C03
	sh $s0, 96($t0)
	li $s0, 0xFC0F
	sh $s0, 128($t0)
	li $s0, 0x0C00
	sh $s0, 160($t0)
	sh $s0, 192($t0)
	sh $s0, 224($t0)
	
	li $a0, 1000		# pause program for 1 sec
	li $v0, 32
	syscall	
	
	j DrawBack


DrawFive:
	li $s0, 0xFC3F
	
	addi $k0, $k0, 1 	#increment card position tracker

	jal Jump
	
	sh $s0, 0($t0)		#Display 5
	li $s0, 0x0030
	sh $s0, 32($t0)
	sh $s0, 64($t0)
	li $s0, 0xF03F
	sh $s0, 96($t0)
	li $s0, 0x0C00
	sh $s0, 128($t0)
	sh $s0, 160($t0)
	sh $s0, 192($t0)
	li $s0, 0xF03F
	sh $s0, 224($t0)
	
	li $a0, 1000		# pause program for 1 sec
	li $v0, 32
	syscall	
	
	j DrawBack

DrawSix:
	li $s0, 0x3000
	
	addi $k0, $k0, 1 	#increment card position tracker

	jal Jump
	
	sh $s0, 0($t0)		#Display 6
	li $s0, 0xC000
	sh $s0, 32($t0)
	li $s0, 0x0003
	sh $s0, 64($t0)
	li $s0, 0xF00F
	sh $s0, 96($t0)
	li $s0, 0x0C30
	sh $s0, 128($t0)
	sh $s0, 160($t0)
	sh $s0, 192($t0)
	li $s0, 0xF00F
	sh $s0, 224($t0)
	
	li $a0, 1000		# pause program for 1 sec
	li $v0, 32
	syscall	
	
	j DrawBack	

DrawSeven:
	li $s0, 0xFC3F
	
	addi $k0, $k0, 1 	#increment card position tracker

	jal Jump
	
	sh $s0, 0($t0)		#Display 7
	li $s0, 0x0C00
	sh $s0, 32($t0)
	li $s0, 0x3000
	sh $s0, 64($t0)
	li $s0, 0xC000
	sh $s0, 96($t0)
	li $s0, 0x0003
	sh $s0, 128($t0)
	li $s0, 0x000C
	sh $s0, 160($t0)
	li $s0, 0x0030
	sh $s0, 192($t0)
	sh $s0, 224($t0)
	
	li $a0, 1000		# pause program for 1 sec
	li $v0, 32
	syscall	
	
	j DrawBack	

DrawEight:
	li $s0, 0xF00F
	
	addi $k0, $k0, 1 	#increment card position tracker
	
	jal Jump

	sh $s0, 0($t0)		#Display 8
	li $s0, 0x0C30
	sh $s0, 32($t0)
	sh $s0, 64($t0)
	li $s0, 0xF00F
	sh $s0, 96($t0)
	li $s0, 0x0C30
	sh $s0, 128($t0)
	sh $s0, 160($t0)
	sh $s0, 192($t0)
	li $s0, 0xF00F
	sh $s0, 224($t0)
	
	li $a0, 1000		# pause program for 1 sec
	li $v0, 32
	syscall	
	
	j DrawBack
	
DrawNine:
	li $s0, 0xF00F
	
	addi $k0, $k0, 1 	#increment card position tracker

	jal Jump
				
	sh $s0, 0($t0)		#Display 9
	li $s0, 0x0C30
	sh $s0, 32($t0)
	sh $s0, 64($t0)
	li $s0, 0xFC0F
	sh $s0, 96($t0)
	li $s0, 0x0C00
	sh $s0, 128($t0)
	sh $s0, 160($t0)
	sh $s0, 192($t0)
	li $s0, 0xFC3F
	sh $s0, 224($t0)
	
	li $a0, 1000		# pause program for 1 sec
	li $v0, 32
	syscall	
	
	j DrawBack
	
	
FirstPosition:

	la $t0,0xFFFF0010	#address of first position
	
	jr $ra			#Go back to where you came from
	
SecondPosition:

	la $t0, 0xFFFF0012	#address of second position
	
	jr $ra
	
ThirdPosition:

	la $t0, 0xFFFF0014	#address of third position
	
	jr $ra
	
FourthPosition:

	la $t0, 0xFFFF0016	#address of fourth position
	
	jr $ra
	
FifthPosition:

	la $t0, 0xFFFF0018	#address of fifth position
	
	jr $ra
	
SixthPosition:

	la $t0, 0xFFFF001a	#address of sixth position
	
	jr $ra
	
SeventhPosition:

	la $t0, 0xFFFF001c	#address of seventh position
	
	jr $ra
	
EighthPosition:

	la $t0, 0xFFFF001e	#address of eighth position 
	
	jr $ra
		
	
Reset:
	
	li $t4, 0x0000
	la $t0, 0xFFFF0010
	
	sh $t4, 0($t0)		#reset card
	sh $t4, 32($t0)
	sh $t4, 64($t0)
	sh $t4, 96($t0)
	sh $t4, 128($t0)
	sh $t4, 160($t0)
	sh $t4, 192($t0)
	sh $t4, 224($t0)
	
	la $t0, 0xFFFF0012
	
	sh $t4, 0($t0)		#reset card
	sh $t4, 32($t0)
	sh $t4, 64($t0)
	sh $t4, 96($t0)
	sh $t4, 128($t0)
	sh $t4, 160($t0)
	sh $t4, 192($t0)
	sh $t4, 224($t0)
	
	la $t0, 0xFFFF0014	
	
	sh $t4, 0($t0)		#reset card
	sh $t4, 32($t0)
	sh $t4, 64($t0)
	sh $t4, 96($t0)
	sh $t4, 128($t0)
	sh $t4, 160($t0)
	sh $t4, 192($t0)
	sh $t4, 224($t0)
	
	la $t0, 0xFFFF0016	
	
	sh $t4, 0($t0)		#reset card
	sh $t4, 32($t0)
	sh $t4, 64($t0)
	sh $t4, 96($t0)
	sh $t4, 128($t0)
	sh $t4, 160($t0)
	sh $t4, 192($t0)
	sh $t4, 224($t0)
	
	la $t0, 0xFFFF0018	
	
	sh $t4, 0($t0)		#reset card
	sh $t4, 32($t0)
	sh $t4, 64($t0)
	sh $t4, 96($t0)
	sh $t4, 128($t0)
	sh $t4, 160($t0)
	sh $t4, 192($t0)
	sh $t4, 224($t0)
	
	la $t0, 0xFFFF001a	
	
	sh $t4, 0($t0)		#reset card
	sh $t4, 32($t0)
	sh $t4, 64($t0)
	sh $t4, 96($t0)
	sh $t4, 128($t0)
	sh $t4, 160($t0)
	sh $t4, 192($t0)
	sh $t4, 224($t0)
	
	la $t0, 0xFFFF001c	
	
	sh $t4, 0($t0)		#reset card
	sh $t4, 32($t0)
	sh $t4, 64($t0)
	sh $t4, 96($t0)
	sh $t4, 128($t0)
	sh $t4, 160($t0)
	sh $t4, 192($t0)
	sh $t4, 224($t0)
	
	la $t0, 0xFFFF001e	
	
	sh $t4, 0($t0)		#reset card
	sh $t4, 32($t0)
	sh $t4, 64($t0)
	sh $t4, 96($t0)
	sh $t4, 128($t0)
	sh $t4, 160($t0)
	sh $t4, 192($t0)
	sh $t4, 224($t0)
	
	j Start
	
Lose:
	la $t0,0xFFFF0010	
	addi $s6, $zero, 1
	
loseloop:

	li $t1, 0x0000		#load frowny face font
	li $t2, 0x1414
	li $t3, 0x4001
	li $t4, 0x1004
	li $t5, 0x0410
	
	sh $t1, 0($t0)		#Display frowns
	sh $t2, 32($t0)
	sh $t1, 64($t0)
	sh $t3, 96($t0)
	sh $t4, 128($t0)
	sh $t5, 160($t0)
	sh $t1, 192($t0)
	sh $t1, 224($t0)
	
	beq $s6, $k1, End
	
	addi, $t0, $t0, 2
	addi $s6, $s6, 1
	
	j loseloop
	
End:
	li $v0,10	#exit program
	syscall
	
	
	
	
ExitWin:
	
	la $t0,0xFFFF0010
	
	li $t1, 0x0000
	li $t2, 0x2828
	li $t3, 0x0820
	li $t4, 0x2008
	li $t5, 0x8002
	
	
	sh $t1, 0($t0)		#Display smiles
	sh $t2, 32($t0)
	sh $t1, 64($t0)
	sh $t3, 96($t0)
	sh $t4, 128($t0)
	sh $t5, 160($t0)
	sh $t1, 192($t0)
	sh $t1, 224($t0)
	
	la $t0,0xFFFF0012
	
	sh $t1, 0($t0)		#Display smiles
	sh $t2, 32($t0)
	sh $t1, 64($t0)
	sh $t3, 96($t0)
	sh $t4, 128($t0)
	sh $t5, 160($t0)
	sh $t1, 192($t0)
	sh $t1, 224($t0)
	
	la $t0,0xFFFF0014
	
	sh $t1, 0($t0)		#Display smiles
	sh $t2, 32($t0)
	sh $t1, 64($t0)
	sh $t3, 96($t0)
	sh $t4, 128($t0)
	sh $t5, 160($t0)
	sh $t1, 192($t0)
	sh $t1, 224($t0)
	
	la $t0,0xFFFF0016
	
	sh $t1, 0($t0)		#Display smiles
	sh $t2, 32($t0)
	sh $t1, 64($t0)
	sh $t3, 96($t0)
	sh $t4, 128($t0)
	sh $t5, 160($t0)
	sh $t1, 192($t0)
	sh $t1, 224($t0)
	
	la $t0,0xFFFF0018
	
	sh $t1, 0($t0)		#Display smiles
	sh $t2, 32($t0)
	sh $t1, 64($t0)
	sh $t3, 96($t0)
	sh $t4, 128($t0)
	sh $t5, 160($t0)
	sh $t1, 192($t0)
	sh $t1, 224($t0)

	la $t0,0xFFFF001a
	
	sh $t1, 0($t0)		#Display smiles
	sh $t2, 32($t0)
	sh $t1, 64($t0)
	sh $t3, 96($t0)
	sh $t4, 128($t0)
	sh $t5, 160($t0)
	sh $t1, 192($t0)
	sh $t1, 224($t0)
	
	la $t0,0xFFFF001c
	
	sh $t1, 0($t0)		#Display smiles
	sh $t2, 32($t0)
	sh $t1, 64($t0)
	sh $t3, 96($t0)
	sh $t4, 128($t0)
	sh $t5, 160($t0)
	sh $t1, 192($t0)
	sh $t1, 224($t0)
	
	la $t0,0xFFFF001e
	
	sh $t1, 0($t0)		#Display smiles
	sh $t2, 32($t0)
	sh $t1, 64($t0)
	sh $t3, 96($t0)
	sh $t4, 128($t0)
	sh $t5, 160($t0)
	sh $t1, 192($t0)
	sh $t1, 224($t0)
	
	li $v0,10	#exit program
	syscall
	
