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
	for (auto &row : content) {
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
	int _id;
	int x, y, speed = 1, color = 23; // default color is white
	bool ltr = true;

	// Constructor
	Fish(int x, int y, list<string> fish_ltr, list<string> fish_rtl, int speed) {
		static int id = 0;
		_id = id++;
		this->x = x;
		this->y = y;
		this->fish_ltr = fish_ltr;
		this->fish_rtl = fish_rtl;
		this->speed = speed;
	}
	Fish(int x, int y, list<string> fish_ltr, list<string> fish_rtl, int speed, string name, int color) {
		static int id = 0;
		_id = id++;
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

	bool operator == (Fish fish) {
		return name == fish.name;
	}

	void checkCollision(list<Fish> *fishlist) { // TODO: speed
		list<Fish>::iterator it;
		for (it = fishlist->begin(); it != fishlist->end(); ++it) {
			if (this->x == it->x && this->y == it->y && this->_id != it->_id && (rand() % 100) < 2) { // 2% probability
				// Collision reached
				it = fishlist->erase(it);
				return;
			}
		}
	}
};

void fill_randomly(list<Fish> *fishlist, list<string> testfish_ltr, list<string> testfish_rtl, int count = 5) {
	for (int i = 0; i <= count - 1; i++)
		fishlist->push_front(Fish(rand() % 80 + 1, rand() % 24 + 1, testfish_ltr, testfish_rtl, 1, "Fish", rand() % (159 + 1 - 144) + 144));
}

void print_info(list<Fish> *fishlist) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = 1;
	coord.Y = 1;

	SetConsoleCursorPosition(hConsole, coord);
	cout << fishlist->size(); // Fish count
}

int main() {
	// Set up
	system("color 9F"); // 144 - 159
	system("mode 110, 30");
	srand((unsigned)time(NULL)); // randomness seed
	int tick = 50;

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
	fishlist.push_front(Fish(70, 12, testfish_ltr, testfish_rtl, 1, "Anna", 159));
	fishlist.push_front(Fish(79, 12, testfish_ltr, testfish_rtl, 1, "Maike", 159));
	fill_randomly(&fishlist, testfish_ltr, testfish_rtl, 50);

	// Runtime loop
	list<Fish>::iterator it;
	do {	
		for (it = fishlist.begin(); it != fishlist.end(); ++it) {
			it->move();
			it->checkCollision(&fishlist);
			// cout << it->_id << "    "; // Print fish's id
		}
		print_info(&fishlist);
		this_thread::sleep_for(chrono::milliseconds(tick));
	} while (fishlist.size() > 0);

	return 0;
}