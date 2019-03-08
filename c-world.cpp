#include <iostream>
#include <Windows.h>
#include <chrono>
#include <thread>
using namespace std;

void GoToXY(int column, int line) {
	COORD coord;
	coord.X = column;
	coord.Y = line;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!SetConsoleCursorPosition(hConsole, coord)) {
		throw new exception("Setting Cursor failed :(");
	}
}

int main() {
	//int lines = 24;
	int draw = 1;
	bool ltr = true;

	while (true) {
		GoToXY(draw, 3);
		cout << "fish";
		this_thread::sleep_for(chrono::milliseconds(500));
		
		GoToXY(draw, 3);
		cout << "    ";
		draw++;
	}

	while (true) {
		if (draw > 80) ltr = false;
		if (draw < 1) ltr = true;
	}

	return 0;
}
