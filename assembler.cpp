//Assembler.cpp
//Javier Zarate
//Ivan Rodriguez
#include <cstdlib>  // for exit()
#include <iostream> // for cout, ...
#include <fstream>  // for fstream::open()
#include <string>
#include <sstream>  // for istringstream
#include "assembler.h" 
 
using namespace std;
 
Assembler::Assembler() //constructor
{
    op = 0;
    i = 0;
    instruction = 0;
    format = 0; 
}
 
void Assembler::converter(string infile){   
    string outfile;
    ifstream progS;                 //ifstream to read in source file 
    string file = "programs/"+infile; 
    progS.open(file.c_str());

    if(!progS.is_open())            {
        cout << "Error: " + file +  " failed to open" << endl;
        exit(1);
    }
 
    getline(progS, line);   
    while(!progS.eof()){
        while(line[0] == '!'){      //even where the first character read is                           // a comment
            getline(progS, line);   //getline retrieves that entire line
        }
        for (int x = 1; x < line.size(); x++){
            if(line[x] == '!'){//for and if that traverse through line and                  //erase it if is starts with comment. 
                line = line.erase((x -1), (line.size() - x));
            }
        }
        rd = -1; rs = -1; constant = -129;
        format = 0; //format = 0 OP|RD|I|RS|Unused 
                    //*all = 0 unless stated otherwise
                    //format = 1 OP|RD|I|ADDR/CONST
                    //format = specialcase 
        int specialcase = 2;
        istringstream str(line.c_str());
        str >> opcode;
        if (opcode == "load"){
            str >> rd >> constant;
            op = 0;
            i = 0;
            format = 3;         
        }
        else if (opcode == "loadi"){
            str >> rd >> constant;
            op = 0;
            i = 1;
            format = 1;
        }
        else if(opcode == "store"){
            str >> rd >> constant;
            op = 1;
            i = 1;
            format = 1;
        }
        else if (opcode == "add"){
            str >> rd >> rs;
            op = 2;
            i = 0;
        }
        else if (opcode == "addi"){
            str >> rd >> constant;
            op = 2;
            i = 1;
            format = 1;
        }
        else if(opcode == "addc"){
            str >> rd >> rs;
            op = 3;
            i = 0;
        }
        else if(opcode == "addci"){
            str >> rd >> constant;
            op = 3;
            i = 1;
            format = 1;
        }
        else if(opcode == "sub"){
            str >> rd >> rs;
            op = 4;
            i = 0;
        }
        else if(opcode == "subi"){
            str >> rd >> constant;
            op = 4;
            i = 1;
            format = 1;
        }
        else if(opcode == "subc"){
            str >> rd >> rs;
            op = 5; 
            i = 0;
        }
        else if(opcode == "subci"){
            str >> rd >> constant;
            op = 5;
            i = 1;
            format = 1;
        }
        else if(opcode == "and"){
            str >> rd >> rs;
            op = 6;
            i = 0;
        }
        else if(opcode == "andi"){
            str >> rd >> constant;
            op = 6;
            i = 1;
            format = 1;
        }
        else if(opcode == "xor"){
            str >> rd >> rs; 
            op = 7; 
            i = 0;   
        }
        else if(opcode == "xori"){
            str >> rd >> constant;
            op = 7;
            i = 0;
            format = 1;
        }
        else if(opcode == "compl"){ //doesnt fit format 1 and 2
            op = 8;
            str >> rd;
            op = op << 11;
            rd = rd << 9; 
            instruction = op | rd;
            format = specialcase;
 
        }
        else if(opcode == "shl"){
            op = 9;
            str >> rd;
            op = op << 11;
            rd = rd << 9;
            instruction = op | rd;
            format = specialcase;
        }
        else if(opcode == "shla"){
            op = 10;
            str >> rd;
            op = op << 11;
            rd = rd << 9;
            instruction = op | rd;
            format = specialcase;
        }
        else if(opcode == "shr"){
            op = 11;
            str >> rd;
            op = op << 11;
            rd = rd << 9;
            instruction = op | rd;
            format = specialcase;
        }
        else if(opcode == "shra"){
            op = 12;
            str >> rd;
            op = op << 11;
            rd = rd << 9;
            instruction = op | rd;
            format = specialcase;
        }
        else if(opcode == "compr"){
            op = 13;
            i = 0;
            str >> rd >> rs;
        }
        else if(opcode == "compri"){
            op = 13;
            i = 1;
            str >> rd >> constant;
            format = 1;
        }
        else if(opcode == "getstat"){
            op = 14;
            str >> rd;
            op = op << 11;
            rd = rd << 9;
            instruction = op | rd;
            format = specialcase;
        }
        else if(opcode == "putstat"){
            op = 15;
            str >> rd;
            op = op << 11;
            rd = rd << 9;
            instruction = op | rd;
            format = specialcase;
        }
        else if(opcode == "jump"){
            op = 16;
            i = 1;
            str >> constant; //constant is either const or ADDR
            op = op << 11;
            i = i << 8;
            instruction = op | i | constant;
            format = specialcase;
        }
        else if(opcode == "jumpl"){
            op = 17;
            i = 1;
            str >> constant;
            op = op << 11;
            i = i << 8;
            instruction = op | i | constant;
            format = specialcase;
        }
        else if(opcode == "jumpe"){
            op = 18;
            i = 1;
            str >> constant;
            op = op << 11;
            i = i << 8;
            instruction = op | i | constant;
            format = specialcase;
        }
        else if(opcode == "jumpg"){
            op = 19;
            i = 1;
            str >> constant;
            op = op << 11;
            i = i << 8;
            instruction = op | i | constant;
            format = specialcase;
        }
        else if(opcode == "call"){
            op = 20;
            i = 1;
            str >> constant;
            op = op << 11;
            i = i << 8;
            instruction = op | i | constant;
            format = specialcase;
        }
        else if(opcode == "return"){
            op = 21;
            instruction = op << 11;
            format = specialcase;
        }
        else if(opcode == "read"){
            op = 22;
            str >> rd;
            op = op << 11;
            rd = rd << 9;
            instruction = op | rd;
            format = specialcase;
        }
        else if(opcode == "write"){
            op = 23;
            str >> rd;
            op = op << 11;
            rd = rd << 9;
            instruction = op | rd;
            format = specialcase;
        }
        else if(opcode == "halt"){
            op = 24;
            instruction = op << 11;
            format = specialcase;
        }
        else if(opcode == "noop"){
            op = 25;
            instruction = (op << 11);
            format = specialcase;
        }
        else{
            cout << "Error, illegal Op" << endl;
            exit(2);
        }
 
        //formats ---------------------
        if(format == 1)
        {
            op = op << 11;
            rd = rd << 9;
            i = i << 8;
            if(constant < 128 && constant > -129){   
                if(constant < 0){
                    instruction = op | rd | i | constant + 256 ;
                }
                else{
                    instruction = op | rd | i | (constant);
                }
           
            }
            else{
                cout << "Error, Invalid constant" << endl;
                exit(3);
            }
        }
        else if(format == 3){
            op = op << 11;
            rd = rd << 9;
            i = i << 8;
 
            instruction = op | rd | i | constant;
 
        }
        else if(format == 0){
            op = op << 11;
            rd = rd << 9;
            i = i << 8;
            rs = rs << 6;
            instruction = op | rd | i | rs;
        }

        string outfile = infile.substr(0, infile.length()-2);
        outfile = outfile + ".o"; 
        ofstream progO(outfile.c_str(), ios::app);
        while(progO.good()){  
            progO << instruction << '\n';
            progO.close();  
        }
        getline(progS, line);   
    }
}
 