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

void removeFish(list<Fish> fishlist, string name) {
	for (auto& fish : fishlist) {
		if (fish.name == name) {
			fishlist.remove(fish);
		}
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
	Fish(int x, int y, list<string> fish_ltr, list<string> fish_rtl, int speed) {
		this->x = x;
		this->y = y;
		this->fish_ltr = fish_ltr;
		this->fish_rtl = fish_rtl;
		this->speed = speed;
	}
	Fish(int x, int y, list<string> fish_ltr, list<string> fish_rtl, int speed, string name, int color) {
		this->x = x;
		this->y = y;
		this->fish_ltr = fish_ltr;
		this->fish_rtl = fish_rtl;
		this->speed = speed;
		this->name = name;
		this->color = color;
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
	void turn() {
		ltr = !ltr;
	}
	void checkCollision(list<Fish> *fishlist) { // TODO: speed
		for (auto fish : *fishlist) {
			if (this->x == fish.x && this->y == fish.y && this->name != fish.name) {
				cout << "COLLISION";
				this_thread::sleep_for(chrono::milliseconds(10000));

				/*if (this->fish_ltr.size() > fish.fish_ltr.size())
					fishlist->remove(fish);
				else
					fishlist->remove(*this);*/
			}
		}
	}
};

void fill_randomly(list<Fish> * fishlist, list<string> testfish_ltr, list<string> testfish_rtl, int count = 5) {
	for (int i = 0; i <= count; i++)
		fishlist->push_front(Fish(rand() % 80 + 1, rand() % 24 + 1, testfish_ltr, testfish_rtl, rand() % 5 + 1));
}

int main() {
	// Set up
	system("color 1F");
	system("mode 110, 30");
	srand((unsigned)time(NULL)); // randomness seed
	int tick = 200;

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
	fishlist.push_front(Fish(74, 12, testfish_ltr, testfish_rtl, 1, "Maike", 29));
	fishlist.push_front(Fish(24, 12, testfish_ltr, testfish_rtl, 1, "Anna", 26));
	//fill_randomly(&fishlist, testfish_ltr, testfish_rtl);

	// Runtime loop
	while (true) {
		for (auto& fish : fishlist) {
			fish.move();
			fish.checkCollision(&fishlist);
		}
		this_thread::sleep_for(chrono::milliseconds(tick));
	}

	return 0;
}