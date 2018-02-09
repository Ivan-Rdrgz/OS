#call make run to compile 
run: os.o assembler.o virtualmachine.o
	g++ os.o assembler.o virtualmachine.o -o run

os.o: os.cpp
	g++ -c os.cpp

assembler.o: assembler.cpp assembler.h
	g++ -c assembler.cpp

virtualmachine.o: virtualmachine.cpp virtualmachine.h
	g++ -c virtualmachine.cpp

clean:
	rm *.o run

