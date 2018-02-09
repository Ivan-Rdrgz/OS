//Assembler.h
//Javier Zarate
//Ivan Rodriguez
#include <iostream>
#include <string>
#include <cstdlib>  // for exit()
#include <iostream> // for cout, ...
#include <fstream>  // for fstream::open()
#include <string>
#include <sstream>  // for istringstream
#ifndef ASSEMBLER_H
#define ASSEMBLER_H

using namespace std;
 class Assembler{
 private: 
 	int op, rd, rs, constant, i, instruction;
 	string opcode, line;
 	int format;			

 public:
 	Assembler();
 	void converter(string infile);
 };
 #endif
