build:
	clear
	rm -f stacscrp
	gcc -g -Wall src/stacscrp.c src/diskstack.* src/instruction.* -o stacscrp

run:
	./stacscrp

buildtestbench:
	clear
	rm -f testbench
	gcc -g -Wall testbench.c src/diskstack.* src/instruction.* -o testbench

runtestbench:
	./testbench

buildassembler:
	clear
	rm -f assembler
	gcc -g -Wall assembler.c src/diskstack.* src/instruction.* -o assembler

buildsimpleassembler:
	clear
	rm -f testassembler
	gcc -g -Wall simpleassembler.c src/instruction.* -o simpleassembler

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./stacscrp

valgrindassembler:
	valgrind --tool=memcheck --leak-check=yes ./assembler

valgrindtestbench:
	valgrind --tool=memcheck --leak-check=yes ./testbench

clean:
	rm -f stacscrp testbench assembler simpleassembler
