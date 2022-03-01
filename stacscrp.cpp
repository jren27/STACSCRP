
#include <iostream>
#include <fstream>
#include "src/diskstack.h"
#include "src/disk.h"
#include "src/stacvirt.h"

int main() {
	StacVirt sv;
	string filename, line;
	fstream file;
	cout << "Enter file: " << endl;
	cin >> filename;
	file.open(filename);
	if (file.fail()) {
		cout << "Error: File not found" << endl;
		return 1;
	}
	while (!file.eof()) {	
		getline(file, line);
		if (!file.fail()) {
			sv.executeLine(line);
		}
	}

	return 0;
}
