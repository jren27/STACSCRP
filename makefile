build:
	clear
	rm -f stacscrp
	g++ -g -Wall stacscrp.cpp src/* -o stacscrp

run:
	./stacscrp

buildtestbench:
	clear
	rm -f testbench
	g++ -g -Wall testbench.cpp src/* -o testbench -lgtest_main -lgtest -lpthread

runtestbench:
	./testbench

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./stacscrp

valgrindtestbench:
	valgrind --tool=memcheck --leak-check=yes ./testbench
