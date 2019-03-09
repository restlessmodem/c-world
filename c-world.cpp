#include <iostream>
#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <list>
using namespace std;

void DrawObject(int x, int y, list<string> content, int color) {
	// Declare coord and initialize handle
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;

	for (auto const& row : content) {
		// Set output properties, print to console and move
		SetConsoleCursorPosition(hConsole, coord);
		SetConsoleTextAttribute(hConsole, color);
		cout << row;
		coord.Y++;
	}
}

class Fish {
public:
	// Member variables
	string name = "Fish";
	list<string> fish_ltr, fish_rtl;
	int x, y, speed = 1;
	bool ltr = true;

	// Constructor
	Fish (int x, int y, list<string> fish_ltr, list<string> fish_rtl) {
		this->x = x;
		this->y = y;
		this->fish_ltr = fish_ltr;
		this->fish_rtl = fish_rtl;
	}

	// Methods
	void move() {
		for (int i = 0; i < speed; i++) {
			if (ltr) {
				DrawObject(x, y, fish_ltr, 28);
				x++;
			}
			else {
				DrawObject(x, y, fish_rtl, 26);
				x--;
			}
			cout << name << " ";

			// Check if we reached the edge
			if (x > 80) ltr = false;
			else if (x < 1) ltr = true;
		}
	}
};

int main() {
	// Set up
	system("color 1F");
	srand((unsigned)time(NULL)); // Randomness
	//int tick = 10 + rand() % (500 + 1) - 10;
	int tick = 100;

	// Fish design
	list<string> testfish_rtl = {
		" o   . -= -.   ",
		"  o (       >< ",
		"     `- = -'   " };
	list<string> testfish_ltr = {
		"   . -= -.   o ",
		" ><       ) o  ",
		"   `- = -'     " };

	// Load fish
	Fish *maike = new Fish(74, 12, testfish_ltr, testfish_rtl);
	maike->name = "Maike";

	Fish* anna = new Fish(24, 20, testfish_ltr, testfish_rtl);
	anna->name = "Anna";
	anna->speed = 2;

	Fish* christopher = new Fish(24, 3, testfish_ltr, testfish_rtl);
	christopher->name = "Christopher";
	christopher->speed = 3;

	// Runtime loop
	while (true) {
		christopher->move();
		maike->move();
		anna->move();
		this_thread::sleep_for(chrono::milliseconds(tick));
	}

	return 0;
}
