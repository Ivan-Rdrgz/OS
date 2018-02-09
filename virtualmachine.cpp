//Virtual Machine.cpp
//Javier Zarate
//Ivan Rodriguez 
//Dr. Zemoudeh
#include "virtualmachine.h"
#include <iostream> 
#include <fstream> 
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
 
using namespace std;
 
VirtualMachine::VirtualMachine(){
	pc = 0;
	ir = 0;
	limit = 0;  
	base = 0;
	sr = 0;
	i = 0;
	rs = 0;
	address = 0;
	constant = 0;
	mask = 0;
	sp = 256; 
	extension = 0u;
	r = vector <int>(REG_FILE_SIZE);
	mem = vector <int>(MEM_SIZE);
}

void VirtualMachine::setFile(string& file){
	VirtualMachine::file = file;
}

string VirtualMachine::getFile(){
	return file;
}

void VirtualMachine::startClock(){
	TIME = 0;
}
 
void VirtualMachine::writeClock(string& readOut){
	int storeTime = TIME;
	ofstream outfile;
	outfile.open(readOut.c_str(), ios::app); 
	if (!outfile.is_open()) {
		cout << "Failed to write clock.\n";
		exit(0);
	}
	outfile << "\n Clock ticks: " << storeTime;
}
 
void VirtualMachine::load(string oFile){
	setFile(oFile);
	//.o file 
	oFile = oFile.substr(0, oFile.length()-2);
	oFile = oFile + ".o";
	int limitcount = 0;
	vector<int> temp; //temp vector to hold read in values.
	int progInstruction;
	ifstream program;
	program.open(oFile.c_str());

	if(!program.is_open()){
		cout << "Error, Failed to load program.\n";
		exit(1); // exits program if file does not open. 
	}
	while(program.good()){
		while(program >> progInstruction){
			temp.push_back(progInstruction);
			limitcount += 1;
		}
	}
	for (int i = 0; i < temp.size(); i++){
		mem[i] = temp[i]; //setting instructions to memory
	}
	limit = limitcount; //set limit to file size
	startClock();
	while(pc <= limit){
		ir = mem[pc]; //initializes IR to mem local
		execute();
		pc++;
	}
}

void VirtualMachine::fetch(){
	//opcode
	op = ir >> 11;

	//destination reg 
	rd = ir >> 9;
	mask = 3;
	rd &= mask;

	//immediate
	i = ir >> 8;
	mask = 1;
	i &= mask;

	//register source
	rs = ir >> 6;
	mask = 3;
	rs &= mask;

	//address
	address = ir;
	mask = 127;
	address &= mask;

	//constant
	constant = ir;
	mask = 255;
	constant &= mask;
}
 
 
void VirtualMachine::execute(){  
	fetch();
	extension = 4294901760u;

	//load
	if (op == 0 && i == 0){   
		if((address > base) && (address < limit)){
			r[rd] = mem[address];
			TIME += 4;
		}
		else{   
			cout <<"runtime error! \n";
		}
	} 

	//loadi
	else if(op == 0 && i == 1){    
		r[rd] = constant;
		if((r[rd] & 128) == 128){
			r[rd] = (r[rd] | 4294967040u);
		}
		TIME += 1;
	}   

	//store
	else if(op == 1 && i == 1){
		if((address > base) && (address < limit)){
			mem[address] = r[rd];  
		}
		TIME += 4;
	} 

	//add
	else if(op == 2 && i == 0){  
		if((r[rs] & 32768) == 32768){
			r[rs] = r[rs] | extension;
		}
		if((r[rd] & 32768) == 32768){
			r[rd] = r[rd] | extension;
		}
		r[rd] = ((r[rd] + r[rs]) & 65535);
		if((r[rd] & 65536) == 65536){//set carry
			sr = sr |1;
		}
		else{    
			sr = sr & 254; 
		} 
		if((r[rd] & 32768) == 32768){
			r[rd] = r[rd] | extension;
		}
		TIME += 1;    
	}  

	//addi
	else if(op == 2 && i == 1){
		r[rd] = ((r[rd] + constant) & 65535);
		if((r[rd] & 65536) == 65536){ //set carry  
			sr = sr |1;
		}
		else{    
			sr = sr & 254; //65534 = 7 ones and one zero
		} 
		if((r[rd] & 32768) == 32768){
			r[rd] = r[rd] | extension;
		}  
		TIME += 1;
	} 

	//addc
	else if(op == 3 && i == 0){
	int carry = sr & 1; //checks if there is a 1 in carry bit
	r[rd] = ((r[rd] + r[rs] + carry) & 65535);
	if((r[rd] & 65536) == 65536)//set carry{  
	  sr = sr |1;
	}
	else{    
	  sr = sr & 254; //65534 = 7 ones and one zero
	} 
	if((r[rd] & 32768) == 32768){
	  r[rd] = r[rd] | extension;
	}
	TIME += 1;
	}

	//addci
	else if(op == 3 && i == 1){
		int carry = sr & 1; //checks if there is a 1 in carry bit
		r[rd] = ((r[rd] + constant + carry) & 65535);
		if((r[rd] & 65536) == 65536){ //set carry  
	  		sr = sr |1;
		}
		else{    
	  		sr = sr & 254; //65534 = 7 ones and one zero
		} 
		if((r[rd] & 32768) == 32768){
	  		r[rd] = r[rd] | extension;
		}
		TIME += 1;
	}

	//sub
	else if(op == 4 && i == 0){
		if((r[rs] & 32768) == 32768){
	  		r[rs] = r[rs] | extension;
		}
		if((r[rd] & 32768) == 32768){
	  		r[rd] = r[rd] | extension;
		}
		r[rd] = r[rd] - r[rs];
		if((r[rd] & 65536) == 65536){//set carry
	  		sr = sr |1;
		}
		else{    
	  		sr = sr & 254; //65534 = 7 ones and one zero
		} 
		if((r[rd] & 32768) == 32768){
	  		r[rd] = r[rd] | extension;
		}
		TIME += 1;
	}

	//subi
	else if(op == 4 && i == 1){
		if((constant & 32768) == 32768){
	  		constant = constant | extension;
		}
		if((r[rd] & 32768) == 32768){
	  		r[rd] = r[rd] | extension;
		}
		r[rd] = r[rd] - constant;
		if((r[rd] & 65536) == 65536){ //set carry  
	  		sr = sr |1;
		}
		else{    
	  		sr = sr & 254; //65534 = 7 ones and one zero
		} 
		if((r[rd] & 32768) == 32768){
	  		r[rd] = r[rd] | extension;
		}
		TIME += 1;
	}

	//subc
	else if(op == 5 && i == 0){   
		int carry = sr & 1; //checks if there is a 1 in carry bit
		if((r[rs] & 32768) == 32768){
	  		r[rs] = r[rs] | extension;
		}
		if((r[rd] & 32768) == 32768){
	  		r[rd] = r[rd] | extension;
		}
		r[rd] = r[rd] - r[rs] - carry;
		if((r[rd] & 65536) == 65536){//set carry   
	  		sr = sr |1;
		}
		else{    
	  		sr = sr & 254; //65534 = 7 ones and one zero
		} 
		if((r[rd] & 32768) == 32768){
	  		r[rd] = r[rd] | extension;
		}
		TIME += 1;
	}   

	//subci
	else if(op == 5 && i == 0){
		int carry = sr & 1; //checks if there is a 1 in carry bit
		if((constant & 32768) == 32768){
	  		constant = constant | extension;
		}
		if((r[rd] & 32768) == 32768){
	  		r[rd] = r[rd] | extension;
		}
		r[rd] = r[rd] - constant - carry;
		if((r[rd] & 65536) == 65536){//set carry
			sr = sr |1;
		}
		else{    
	  		sr = sr & 254; //65534 = 7 ones and one zero
		} 
		if((r[rd] & 32768) == 32768){
	  		r[rd] = r[rd] | extension;
		}
		TIME += 1;
	}	

	//and
	else if(op == 6 && i == 0){
		r[rd] = r[rd] & r[rs];  
		TIME += 1;
	}

	//andi
	else if(op == 6 && i == 1){
		r[rd] = r[rd] & constant;
		TIME += 1;
	}

	//xor
	else if(op == 7 && i == 0){
		r[rd] = r[rd] ^ r[rs];  
		TIME += 1;
	}

	//xori
	else if(op == 7 && i == 1){
		r[rd] = r[rd] ^ constant;
		TIME += 1;
	}

	//compliment
	else if(op == 8 && i == 0){
		r[rd] = ~ r[rd];
		TIME += 1;
	} 

	//shl
	else if(op == 9 && i == 0){
		if((r[rd] & 32768) == 32768){            //if 8th bit is 1.
	  		r[rd] = r[rd] << 1;           
	  		sr = sr | 1;                  //sets carry to 1.
		}
		else{                             //else, 16th bit is 0
	  		r[rd] = r[rd] << 1;           //shift left and put a 0 in 16th bit.
		} 
		TIME += 1;		
	} 

	//shla
	else if(op == 10 && i == 0){
		if((r[rd] & 32768) == 32768){            //if 8th bit is 1.
			r[rd] = r[rd] | extension;
			r[rd] = (r[rd] << 1) | 32768;   //shift left and put a 1 in 8th bit.
			sr = sr | 1;                  //sets carry to 1.
		}
		else{                             //else, 16th bit is 0
			r[rd] = r[rd] << 1 & 32767;   //shift left and put a 0 in 16th bit.
		} 
		TIME += 1;
	}  

	//shr
	else if(op == 11 && i == 0){
		if((r[rd] & 1) == 1){             //if the first bit is 1 set carry
			sr = sr | 1;                //set carry to 1.
			r[rd] = r[rd] >> 1;
		}
		else{                            //else, 16th bit is 0
			r[rd] = r[rd] >> 1;
		}
		TIME += 1;
	}

	//shra
	else if(op == 12 && i == 0){
		if(r[rd] & 1 == 1){             //if the first bit is 1 set carry
			sr = sr | 1;                //set carry to 1.
		}
		if(r[rd] & 32768 == 32768){     
		  	r[rd] = r[rd] | extension;
		  	r[rd] = (r[rd] >> 1) | 32768;         //shift right and put a 1 in 8th bit.
		}
		else{                            //else, 16th bit is 0
		  	r[rd] = r[rd] >> 1 & 127;          //shift right and put a 0 in 8th bit.
		}
		TIME += 1;
	}

	//compr
	else if(op == 13 && i == 0){
		if (r[rd] < r[rs]){
		  	sr = sr & 25; //resetting E and G (11001)
		  	sr = sr | 8;   //sets L bit
		}
		if (r[rd] == r[rs]){
			sr = sr & 21; //resetting L and G (10101)
		  	sr = sr | 4;    // sets E bit
		}
		if (r[rd] > r[rs]){
		  	sr = sr & 19; //resetting E and L (10011)
		  	sr = sr | 2;    //sets G bit
		}	
		TIME += 1;
	}   

	//compri
	else if(op == 13 && i == 1){
		if (r[rd] < constant){
		  	sr = sr & 25; //resetting E and G (11001)
		  	sr = sr | 8;   //sets L bit
		}
		if (r[rd] == constant){
		  	sr = sr & 21; //resetting L and G (10101)
		  	sr = sr | 4;    // sets E bit
		}
		if (r[rd] > constant){
		  	sr = sr & 19; //resetting E and L (10011)
		  	sr = sr | 2;    //sets G bit
		}
		TIME += 1;
	}

	//getstat
	else if(op == 14 && i == 0){
		r[rd] = sr;
		TIME += 1;
	}  

	//putstat
	else if(op == 15 && i == 0){
		sr = r[rd];
		TIME += 1;
	}  

	//jump
	else if(op == 16 && i == 1){
		if(pc > address){
		  	cout << "error";
		}  
		pc = address - 1;
		TIME += 1;
	}

	//jumpl
		else if(op == 17 && i == 1){
		if((sr & 8) == 8){//sr & 8 checks if there is an L bit
		  	pc = address - 1;
		}
		//else nothing happens 
		TIME += 1;
	}  

	//jumpe
	else if(op == 18 && i == 1){
		if((sr & 4) == 4){//sr & 4 checks if there is an E bit
		  	pc = address - 1;
		}
		//else nothing happens 
		TIME += 1;
	}  

	//jumpg
	else if(op == 19 && i == 1){
		if((sr & 2) == 2){//sr & 2 checks if there is an G bit
		  	pc = address - 1;
		}
		//else nothing happens 
		TIME += 1;
	}

	//call
	else if(op == 20 && i == 1){
		if(sp < limit + 6){//prevents stack overflow 
			cout << "Error, Stackoverflow\n";
			exit(4);
		}
		else{
		  	//VM status
		  	//pc, r[0] - r[3], sr
		  	//decrements to build from bottom up
		  	//adding to stack
		  	mem[--sp] = pc;
		  	mem[--sp] = r[0];
		  	mem[--sp] = r[1];
		  	mem[--sp] = r[2];
		  	mem[--sp] = r[3];
		  	mem[--sp] = sr;
		  	//pc is set to address
		  	pc = address;
		  	pc = pc - 1;
		  	TIME += 4;
		}
	} 

	//return
	else if(op == 21 && i == 0){
		//VM status
		//pc, r[0] - r[3], sr
		//increments to from pop stack
		//restores previous status
		sr = mem[sp++];
		r[3] = mem[sp++];
		r[2] = mem[sp++];
		r[1] = mem[sp++];
		r[0] = mem[sp++];
		pc = mem[sp++];
		ir = mem[pc];   //ir is set to next inst(resuming from where it left off)
		op = ir >> 11;  //getting op from new instruction.
		TIME += 4;
	} 

	//read
	else if(op == 22){ 
		//.in file
		string progIn = getFile();
		progIn = progIn.substr(0, progIn.length()-2);
		progIn = "inputs/" + progIn + ".in";
		ifstream infile;
		infile.open(progIn.c_str());
		if(!infile.is_open()){
		  	cout << "program failed to load.\n";
		  	exit(2);
		}
		string readInVal;
		getline(infile, readInVal);
		int holdVal;
		istringstream str(readInVal.c_str());
		str >> holdVal;
		//stores read in instr to r[rd]
		r[rd] = holdVal;
		TIME += 28;
	}

	//write
	else if(op == 23 && i == 0){
		//.out file
		string progOut = getFile();
		progOut = progOut.substr(0, progOut.length()-2);
		progOut = "outputs/" + progOut + ".out";

		ofstream outfile;
		outfile.open(progOut.c_str());    
		if(!outfile.is_open()){
		  	cout << "program failed to write.\n";
		  	exit(3);
		}
		int holdVal = r[rd];
		outfile << holdVal;
		TIME += 28; 
		outfile.close(); 
		writeClock(progOut);   
	}

	//halt
		else if(op == 24 && i == 0){
		TIME += 1;
		cout << "in halt\n";
		exit(5);
	}

	//noop
		else if(op == 25 && i == 0){
		//noop: it does nothing
		TIME += 1;
	}
}

