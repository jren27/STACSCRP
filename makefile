build:
	clear
	rm -f STACSCRP
	g++ -g -Wall stacscrp.cpp src/* -o STACSCRP

run:
	./STACSCRP

buildtestbench:
	clear
	rm -f testbench
	g++ -g -Wall testbench.cpp src/* -o testbench -lgtest_main -lgtest -lpthread

runtestbench:
	./testbench

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./STACSCRP

valgrindtestbench:
	valgrind --tool=memcheck --leak-check=yes ./testbench
