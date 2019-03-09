#include <iostream>
#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
using namespace std;

void DrawObject(int x, int y, int rows, string content[], int color) {
	// Declare coord and initialize handle
	COORD coord;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < rows; i++) {
		// Set location
		coord.X = x;
		coord.Y = y;

		// Set output properties and print to console
		SetConsoleCursorPosition(hConsole, coord);
		SetConsoleTextAttribute(hConsole, color);
		cout << content[i];
		y++;
	}
}

int main() {
	// Set up
	system("color 1F");
	int startX = 74;
	bool ltr = true;

	// Randomness
	srand((unsigned)time(NULL));
	int random = 10 + rand() % (500 + 1) - 10;

	// Load fish
	string testfish_rtl[] = {
		" o   . -= -.   ",
		"  o (       >< ",
		"     `- = -'   "};

	string testfish_ltr[] = {
		"   . -= -.   o ",
		" ><       ) o  ",
		"   `- = -'     " };

	// Runtime loop
	while (true) {
		if (ltr) {
			DrawObject(startX, 3, 3, testfish_ltr, 29);
			startX++;
		} else {
			DrawObject(startX, 3, 3, testfish_rtl, 26);
			startX--;
		}
		
		if (startX > 80) ltr = false;
		else if (startX < 1) ltr = true;

		
		cout << "Maike - " << random << "  ";
		this_thread::sleep_for(chrono::milliseconds(random));
	}

	return 0;
}
