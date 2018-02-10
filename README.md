# Toy OS project
C++ program to simulate a simple 16-bit CPU or Virtual Machine (VM) consisting of 4 general purpose registers (r[0]-r[3]) a program counter (pc), an instruction register (ir), a status register (sr), a stack pointer (sp), a clock, an arithmetic and logic unit (ALU), a 256 word memory (mem) with base and limit registers, and a disk. 
The general purpose registers are represented by a vector of 4 integers, mem is represented by a vector of 256 integers, pc is represented by an integer, ir is represented by an integer, etc.
# Running 
1. *make run*
2. *./run example.s*
3. *make clean* (removes object files)

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
![alt text](https://github.com/Ivan-Rdrgz/OS/blob/master/instruction_sheet.png =250x)
