#include "stacvirt.h"
#include <iostream>
#include <sstream>
#include <algorithm>


Operation StacVirt::parseToOperation(vector<string> arguments) {
	Operation op;
	
	if (arguments.size() < 2) {
		cout << "ERROR: Line has less than two words" << endl;
	}

	// Check stack argument (implement default stack 0 later)
	if (arguments[0].size() != 1 || arguments[0][0] - '0' < 0 || arguments[0][0] - '0' > 7) {
		cout << "ERROR: Stack argument must be an integer between 0 or 7, got \"" << arguments[0] << "\" instead\n";
		stop = true;
		return op;
	}
	op.stack = arguments[0][0] - '0';
	std::transform(arguments[1].begin(), arguments[1].end(),arguments[1].begin(), ::toupper);
	op.codeword = arguments[1];

	arguments.erase(arguments.begin(), arguments.begin() + 2);
	op.arguments = arguments;

	return op; // If all went well, should reach this line
}

bool StacVirt::checkArguments(vector<string> arguments, int argCount) {
	if (arguments.size() != argCount) {
		cout << "ERROR: Expected " << argCount << " arguments, got " << arguments.size() - 1 << " instead\n";
		stop = true;
	}
	return !stop;
}

bool StacVirt::isTopLiteral(int stack) {
	if (!stacks[stack].top().isLiteral()) {
		cout << "ERROR: Stack is not a literal" << endl;
		stop = true;
	}
	return !stop;
}

bool StacVirt::getTopTwoLiteral(int stack, double &top, double &secondTop) { // Extract int values of literals for arithmetic
	temp = stacks[stack].pop();
	if (!temp.isLiteral() || !stacks[stack].top().isLiteral()) {
		cout << "ERROR: Cannot perform arithmetic on non-literal disks" << endl;
		stop = true;
		return !stop;
	}
	top = temp.getValue();
	secondTop = stacks[stack].top().getValue();
	return true;
}

void StacVirt::replace(int stack) { // Replace top of stack with temp
	stacks[stack].pop();
	stacks[stack].push(temp);
}

StacVirt::StacVirt() {
	stacks = new DiskStack[8];
	lineNum = 0;
	stop = false;
}


void StacVirt::executeLine(string &line) {
	//Parse line
	stringstream linestream;
	linestream.str(line);
	vector<string> arguments;
	string argument;
	std::size_t prev = 0, pos;
	while ((pos = line.find_first_of(" \t", prev)) != std::string::npos) {
		if (pos > prev) {
			arguments.push_back(line.substr(prev, pos-prev));
		}
		prev = pos+1;
	}
	if (prev < line.length()) {
		arguments.push_back(line.substr(prev, std::string::npos));
	}
	
	Operation op = parseToOperation(arguments);
	if (stop) {
		cout << "On line " << lineNum << endl;
		return;
	}
	//--- ACTUAL EXECUTION LOGIC ---
	double top, secondTop;
	//TODO finish this bigass if statement
	if (op.codeword == "POP") { // Stack manipulation
		if (checkArguments(op.arguments, 0))
			grabbedDisk = stacks[op.stack].pop();
	} else if (op.codeword == "TOP") {
		if (checkArguments(op.arguments, 0))
			grabbedDisk = stacks[op.stack].top();
	} else if (op.codeword == "PUSH") {
		if (checkArguments(op.arguments, 0))
			stacks[op.stack].push(grabbedDisk);
	} else if (op.codeword == "SWAP") {
		if (checkArguments(op.arguments, 0))
			stacks[op.stack].swap();
	} else if (op.codeword == "PEEL") { // wtf	
		if (checkArguments(op.arguments, 0)) {
			temp = stacks[op.stack].top().getStack()->pop();
			stacks[op.stack].push(temp);
		}
	} else if (op.codeword == "WRAP") {
		if (checkArguments(op.arguments, 0)) {
			temp = stacks[op.stack].pop();
			stacks[op.stack].top().getStack()->push(temp);
			}
	} else if (op.codeword == "ADD") { // Arithmetic + logic
		if (getTopTwoLiteral(op.stack, top, secondTop) && checkArguments(op.arguments, 0)) {
			temp = Disk(top + secondTop);
			replace(op.stack);
		}
	} else if (op.codeword == "SUB") {
		if (getTopTwoLiteral(op.stack, top, secondTop) && checkArguments(op.arguments, 0)) {
			temp = Disk(top - secondTop);
			replace(op.stack);
		}
	} else if (op.codeword == "MUL") {
		if (getTopTwoLiteral(op.stack, top, secondTop) && checkArguments(op.arguments, 0)) {
			temp = Disk(top * secondTop);
			replace(op.stack);
		}
	} else if (op.codeword == "DIV") {
		if (getTopTwoLiteral(op.stack, top, secondTop) && checkArguments(op.arguments, 0)) {
			temp = Disk(top / secondTop);
			replace(op.stack);
		}
	} else if (op.codeword == "AND") {
		if (getTopTwoLiteral(op.stack, top, secondTop) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(top && secondTop));
			replace(op.stack);
		}
	} else if (op.codeword == "OR") {
		if (getTopTwoLiteral(op.stack, top, secondTop) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(top || secondTop));
			replace(op.stack);
		}
	} else if (op.codeword == "NOT") {
		if (getTopTwoLiteral(op.stack, top, secondTop) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(!top));
			replace(op.stack);
		}
	} else if (op.codeword == "XOR") {
		if (getTopTwoLiteral(op.stack, top, secondTop) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(!top != !secondTop));
			replace(op.stack);
		}
	} else if (op.codeword == "NEW") {
		if (checkArguments(op.arguments, 1)) {
			temp = Disk();
			switch (op.arguments[0][0]) {
				case '#':
					temp.setValue(stod(op.arguments[0].substr(1)));
					break;
				case '\'':
					temp.setValueChar(op.arguments[0].substr(1)[0]); // If it aint broke dont fix it
					break;
				case '=':
					top = stoi(op.arguments[0].substr(1));
					if (top != 0 && top != 1) {
						cout << "ERROR: Booleans can only be =1 or =0, got =" << top << " instead" << endl;
						stop = true;
						break;
					}
					temp.setValueChar(top);
					break;
				default:
					cout << "ERROR: Value must begin with # (int), . (double), ' (char), = (bool).";
					cout << "Got " << op.arguments[0][0] << " instead" << endl;
			}
			stacks[op.stack].push(temp);
		}
	} else if (op.codeword == "OPUT") {
		if (checkArguments(op.arguments, 0)) {
			if (isTopLiteral(op.stack)) {
				temp = stacks[op.stack].pop();
				cout << temp.getValue() << endl;
			}
		}
	} else {
		cout << "Debug: wtf happened" << endl;
	}
	lineNum++;
}
