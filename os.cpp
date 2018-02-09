//OS.cpp
//Javier Zarate
//Ivan Rodriguez
#include "assembler.h"
#include "virtualmachine.h"

int main(int argc, char *argv[]){
    Assembler assm;
    VirtualMachine vrm;
    assm.converter(argv[1]);
	vrm.load(argv[1]); 
}

 