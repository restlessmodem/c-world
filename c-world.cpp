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

	// Set output properties, print to console and move
	SetConsoleTextAttribute(hConsole, color);
	for (auto const& row : content) {
		SetConsoleCursorPosition(hConsole, coord);
		cout << row;
		coord.Y++;
	}
}

class Fish {
public:
	// Member variables
	string name;
	list<string> fish_ltr, fish_rtl;
	int x, y, speed = 1, color = 23; // default color is white
	bool ltr = true;

	// Constructor
	Fish(int x, int y, list<string> fish_ltr, list<string> fish_rtl, string name) {
		this->x = x;
		this->y = y;
		this->fish_ltr = fish_ltr;
		this->fish_rtl = fish_rtl;
	}
	Fish(int x, int y, list<string> fish_ltr, list<string> fish_rtl, string name, int speed, int color) {
		this->x = x;
		this->y = y;
		this->fish_ltr = fish_ltr;
		this->fish_rtl = fish_rtl;
		this->name = name;
		this->speed;
		this->color;
	}

	// Methods
	void move() {
		for (int i = 0; i < speed; i++) {
			if (ltr) {
				DrawObject(x, y, fish_ltr, color);
				x++;
			}
			else {
				DrawObject(x, y, fish_rtl, color);
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
	list<Fish> fishlist;
	fishlist.push_front(Fish(74, 12, testfish_ltr, testfish_rtl, "Maike"));
	fishlist.push_front(Fish(24, 20, testfish_ltr, testfish_rtl, "Anna", 2, 26));
	fishlist.push_front(Fish(24, 3, testfish_ltr, testfish_rtl, "Christopher", 3, 28));

	// Runtime loop
	while (true) {
		for (auto& fish : fishlist) fish.move();
		this_thread::sleep_for(chrono::milliseconds(tick));
	}
	
	return 0;
}
