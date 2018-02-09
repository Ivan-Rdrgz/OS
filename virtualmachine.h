//VirtualMachine.h
//Javier Zarate
//Ivan Rodriguez
#include <iostream> 
#include <fstream> 
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H


using namespace std;

class VirtualMachine {
private:
    static const int REG_FILE_SIZE = 4;
    static const int MEM_SIZE = 256; //0 -255
    vector <int> r;
    vector <int> mem;
    int pc; //prog counter
    int ir; //inst register
    int limit; // limit of program being read in. 
    int sp; //stack pointer
    int base;
    int sr; //status register
    int rd; //dest register
    int address; //address
    int constant;  
    int op; //opcode
    int i; //immeidiate 
    int rs; //source register
    int mask;
    int TIME;
    unsigned int extension;
    string infile;
    string outfile;
    string file;

public:
    VirtualMachine();
    void load(string oFile); //reads in file and stores it memory
    void fetch(); //
    void execute();
    void startClock();
    void writeClock(string& readOut);
    void setFile(string& file);
    string getFile();
};
#endif
