#include <fstream>
#include "Qint.h"
#include "QFloat.h"
#include "Calculator.h"

int main(int argc, char** argv) {
	if (argc > 2) {
		ifstream fi(argv[1]);
		ofstream fo(argv[2]);
		Compute(fi, fo);
	}
	else if (argc == 2) {
		cout << "Tham so khong dung!";
	}
	else {
		ComputeMenus();
	}
	return 0;
}
