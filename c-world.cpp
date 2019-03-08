#include <iostream>
#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

void DrawAtXY(int column, int line, string content, bool rev) {
	COORD coord;
	coord.X = column;
	coord.Y = line;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!SetConsoleCursorPosition(hConsole, coord)) {
		// oh oh
	}

	if (rev) reverse(content.begin(), content.end());
	cout << content;
}

int main() {
	//int lines = 24;
	int draw = 1;
	bool ltr = true;

	while (true) {
		if (ltr) {
			DrawAtXY(draw, 3, "fish", false);
		} else {
			DrawAtXY(draw, 3, "fish", true);
		}
		
		this_thread::sleep_for(chrono::milliseconds(100));
		
		DrawAtXY(draw, 3, "    ", false);
		cout << "    ";
		
		if (ltr) {
			draw++;
		} else {
			draw--;
		}

		if (draw > 75) ltr = false;
		if (draw < 1) ltr = true;
	}

	return 0;
}
