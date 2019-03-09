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

class Fish {
public:
	// Member variables
	string name = "Fish";
	int x, y, speed = 1;
	bool ltr = true;

	// Constructor
	Fish (int x, int y) {
		this->x = x;
		this->y = y;
	}

	// Methods
	void move(string fish_ltr[], string fish_rtl[]) {
		for (int i = 0; i < speed; i++) {
			if (ltr) {
				DrawObject(x, y, 3, fish_ltr, 28);
				cout << name << " ";
				x++;
			}
			else {
				DrawObject(x, y, 3, fish_rtl, 26);
				cout << name << " ";
				x--;
			}

			if (x > 80) ltr = false;
			else if (x < 1) ltr = true;
		}
	}
};

int main() {
	// Set up
	system("color 1F");
	srand((unsigned)time(NULL)); // Randomness
	int speed = 10 + rand() % (500 + 1) - 10;

	// Fish design
	string testfish_rtl[] = {
		" o   . -= -.   ",
		"  o (       >< ",
		"     `- = -'   " };
	string testfish_ltr[] = {
		"   . -= -.   o ",
		" ><       ) o  ",
		"   `- = -'     " };

	// Load fish
	Fish *maike = new Fish(74, 3);
	maike->name = "Maike";

	Fish* anna = new Fish(24, 20);
	anna->name = "Anna";

	Fish* christopher = new Fish(24, 12);
	christopher->name = "Christopher";
	christopher->speed = 2;

	// Runtime loop
	while (true) {
		maike->move(testfish_ltr, testfish_rtl);
		anna->move(testfish_ltr, testfish_rtl);
		christopher->move(testfish_ltr, testfish_rtl);
		this_thread::sleep_for(chrono::milliseconds(speed));
	}

	return 0;
}
