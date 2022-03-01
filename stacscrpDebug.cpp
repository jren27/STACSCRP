
#include <iostream>
#include "src/diskstack.h"
#include "src/disk.h"
#include "src/stacvirt.h"

int main() {
	StacVirt sv;
	string input;
	while (input != "exit") {
		cout << ">>> ";
		getline(cin, input);
		sv.executeLine(input);
	}
	return 0;
}
