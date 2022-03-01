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
	double tempVal1, tempVal2;
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
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk(tempVal1 + tempVal2);
			replace(op.stack);
		}
	} else if (op.codeword == "SUB") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk(tempVal2 - tempVal1);
			replace(op.stack);
		}
	} else if (op.codeword == "MUL") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk(tempVal1 * tempVal2);
			replace(op.stack);
		}
	} else if (op.codeword == "DIV") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk(tempVal2 / tempVal1);
			replace(op.stack);
		}
	} else if (op.codeword == "AND") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(tempVal1 && tempVal2));
			replace(op.stack);
		}
	} else if (op.codeword == "OR") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(tempVal1 || tempVal2));
			replace(op.stack);
		}
	} else if (op.codeword == "NOT") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(!tempVal1));
			replace(op.stack);
		}
	} else if (op.codeword == "XOR") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(!tempVal1 != !tempVal2));
			replace(op.stack);
		}

	} else if (op.codeword == "CMEQ") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(tempVal1 == tempVal2));
		}
	} else if (op.codeword == "CMGT") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			// Note that tempVal2 COMES BEFORE tempVal1
			temp = Disk((int)(tempVal1 < tempVal2));
		}
	} else if (op.codeword == "CMLT") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(tempVal1 > tempVal2));
		}
	} else if (op.codeword == "CMGE") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(tempVal1 <= tempVal2));
		}
	} else if (op.codeword == "CMLE") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(tempVal1 >= tempVal2));
		}
	} else if (op.codeword == "CMNE") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = Disk((int)(tempVal1 != tempVal2));
		}
	} else if (op.codeword == "CMRC") {
		if (getTopTwoLiteral(op.stack, tempVal1, tempVal2) && checkArguments(op.arguments, 0)) {
			temp = stacks[op.stack].pop();
			tempVal1 = (temp == stacks[op.stack].top());
			stacks[op.stack].push(temp);
			temp = Disk(tempVal1);
			stacks[op.stack].push(temp);
		}
	} else if (op.codeword == "LTRL") {
		if (checkArguments(op.arguments, 0)) {
			tempVal1 = stacks[op.stack].top().isLiteral();
			temp = Disk(tempVal1);
			stacks[op.stack].push(temp);
		}
	} else if (op.codeword == "NEW") { // Disk manipulation
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
					tempVal1 = stoi(op.arguments[0].substr(1));
					if (tempVal1 != 0 && tempVal1 != 1) {
						cout << "ERROR: Booleans can only be =1 or =0, got =" << tempVal1 << " instead" << endl;
						stop = true;
						break;
					}
					temp.setValueChar(tempVal1);
					break;
				default:
					cout << "ERROR: Value must begin with # (int), . (double), ' (char), = (bool).";
					cout << "Got " << op.arguments[0][0] << " instead" << endl;
			}
			stacks[op.stack].push(temp);
		}
	} else if (op.codeword == "NEWN") {
		if (checkArguments(op.arguments, 1)) {
			temp = Disk(0, false);
			stacks[op.stack].push(temp);
		}
	} else if (op.codeword == "IPUT") {
		if (checkArguments(op.arguments, 0)) {
			cin >> tempVal1;
			temp = Disk(tempVal1);
			stacks[op.stack].push(temp);
		}
	} else if (op.codeword == "OPUT") {
		if (checkArguments(op.arguments, 0)) {
			if (isTopLiteral(op.stack)) {
				cout << stacks[op.stack].pop().getValue() << endl;
			}
		}
	} else {
		cout << "Debug: wtf happened" << endl;
	}

	if (stop) { // If error caught when executing
		cout << "On line " << lineNum << endl;
		return;
	}
	lineNum++;
}
