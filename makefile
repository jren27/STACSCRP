build:
	clear
	rm -f emulator
	gcc -g -Wall emulator.c src/diskstack.* src/instruction.* -o emulator
	gcc -g -Wall assembler.c src/diskstack.* src/instruction.* src/map.* -o assembler

buildtestbench:
	clear
	rm -f testbench
	gcc -g -Wall testbench.c src/diskstack.* src/instruction.* -o testbench

clean:
	rm -f emulator testbench assembler out.bin
