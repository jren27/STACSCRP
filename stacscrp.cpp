
#include <iostream>
#include <fstream>
#include <vector>
#include "src/diskstack.h"
#include "src/disk.h"
#include "src/stacvirt.h"

int main() {
	StacVirt sv;
	string filename, line;
	fstream file;
	cout << "Enter file: ";
	cin >> filename;
	file.open(filename);
	if (file.fail()) {
		cout << "Error: File not found" << endl;
		return 1;
	}
	while (!file.eof()) {	
		getline(file, line);
		if (!file.fail() && !sv.isStopped()) {
			sv.appendLine(line);
		}
	}
	if (sv.isStopped()) {
		return 1;
	}
	cout << "Running " << filename << "..." << endl;
	while (!sv.isStopped()) {
		sv.executeLine();
	}

	return 0;
}
