# Toy OS project
C++ program to simulate a simple 16-bit CPU or Virtual Machine (VM) consisting of 4 general purpose registers (r[0]-r[3]) a program counter (pc), an instruction register (ir), a status register (sr), a stack pointer (sp), a clock, an arithmetic and logic unit (ALU), a 256 word memory (mem) with base and limit registers, and a disk. 
The general purpose registers are represented by a vector of 4 integers, mem is represented by a vector of 256 integers, pc is represented by an integer, ir is represented by an integer, etc.

# Building
the three main folders are "programs", "inputs", and "outputs". The program files should be placed in the "programs" folder and have a ".s" extention, any inputs to the program should have a ".in" extension and be placed in the "inputs" folder, the output files will appear in the "outputs" folder. 


The least significant five bits of sr are reserved for OVERFLOW, LESS, EQUAL, GREATER, and CARRY status in that order, the rest are "don't-care" (d): 

  d  	          ...          	  d  	  V  	  L  	  E  	  G  	  C  
15		  5	  4	  3	  2	  1	  0

ALU is part of the logic of your program, disk is represented by a collection of files, and the rest of the components are represented by simple variables in your program:
	int pc, ir, sr, sp, base, limit, clock;
We only use the lower 16 bits of the variables. clock could be alternatively represented by a class.
The VM supports two instruction formats. 

Format 1:
      OP      	RD	I	RS	      UNUSED        
      15:11      	10:9	8	7:6	          5:0      

Format 2:
      OP      	RD	I	      ADDR/CONST      
      15:11      	10:9	8	              7:0      

where OP (bits 11 to 15 from right to left) stands for opcode, 
RD (bits 9 and 10) stands for register-destination, 
I (bit 8) stands for immediate, 
and RS (bits 6 and 7) stands for register-source. 

When I is 0, the next 2 bits specify the source register and the next 6 bits are unused. 
When I is 1, immediate address mode is in effect: depending on the instruction, the next 8 bits are treated as either an unsigned 8 bit address (ADDR), or an 8 bit two's complement constant (CONST). This implies 0 <= ADDR < 256 and -128 <= CONST < 128. 

load and loadi are special instructions, they both use format 2: when I = 0, we use ADDR, when I = 1, we use CONST. 

If a field is unused, it is considered don't-care, and it can be set to any bit pattern, but in this project we will set don't-cares to all zeros. 

To simplify writing programs for the VM, we need an assembly language and its corresponding assembler. The following table lists all instructions supported by the Assembler and in turn VM. 



# Instruction set
*  OP	I	Instruction	Semantic in Pseudo C++ Syntax	          Additional Action
*00000	0	load RD ADDR	r[RD] = mem[ADDR]	 
*00000	1	loadi RD CONST	r[RD] = CONST	 
*00001	1	store RD ADDR	mem[ADDR] = r[RD]	 
*00010	0	add RD RS	r[RD] = r[RD] + r[RS]	                        Set CARRY
*00010	1	addi RD CONST	r[RD] = r[RD] + CONST	                    Set CARRY
*00011	0	addc RD RS	r[RD] = r[RD] + r[RS] + CARRY	              Set CARRY
*00011	1	addci RD CONST	r[RD] = r[RD] + CONST + CARRY	          Set CARRY
*00100	0	sub RD RS	r[RD] = r[RD] - r[RS]	                        Set CARRY
*00100	1	subi RD CONST	r[RD] = r[RD] - CONST	                    Set CARRY
*00101	0	subc RD RS	r[RD] = r[RD] - r[RS] - CARRY	              Set CARRY
*00101	1	subci RD CONST	r[RD] = r[RD] - CONST - CARRY	          Set CARRY
*00110	0	and RD RS	r[RD] = r[RD] & r[RS]	 
*00110	1	andi RD CONST	r[RD] = r[RD] & CONST	 
*00111	0	xor RD RS	r[RD] = r[RD] ^ r[RS]	 
*00111	1	xori RD CONST	r[RD] = r[RD] ^ CONST	 
*01000	d	compl RD	r[RD] = ~ r[RD]	 
*01001	d	shl RD	r[RD] = r[RD] << 1, shift-in-bit = 0	          Set CARRY
*01010	d	shla RD	shl arithmetic	                                Set CARRY & Sign Extend
*01011	d	shr RD	r[RD] = r[RD] >> 1, shift-in-bit = 0	          Set CARRY
*01100	d	shra RD	shr arithmetic	                                Set CARRY & Sign Extend
*01101	0	compr RD RS	if r[RD] < r[RS] set LESS reset EQUAL and GREATER; 
        if r[RD] == r[RS] set EQUAL reset LESS and GREATER; 
        if r[RD] > r[RS] set GREATER reset EQUAL and LESS	 
*01101	1	compri RD CONST  	if r[RD] < CONST set LESS reset EQUAL and GREATER; 
        if r[RD] == CONST set EQUAL reset LESS and GREATER; 
        if r[RD] > CONST set GREATER reset EQUAL and LESS	 
*01110	d	getstat RD	r[RD] = SR	 
*01111	d	putstat RD	SR = r[RD]	 
*10000	1	jump ADDR	pc = ADDR	 
*10001	1	jumpl ADDR	if LESS == 1, pc = ADDR, else do nothing	 
*10010	1	jumpe ADDR	if EQUAL == 1, pc = ADDR, else do nothing	 
*10011	1	jumpg ADDR	if GREATER == 1, pc = ADDR, else do nothing	 
*10100	1	call ADDR	push VM status; pc = ADDR	 
*10101	d	return	pop and restore VM status	 
*10110	d	read RD	read new content of r[RD] from .in file	 
*10111	d	write RD	write r[RD] into .out file	 
*11000	d	halt	halt execution	 
*11001	d	noop	no operation	 
