#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <list>
using namespace std;

// Global variables
string lastEvent;
unsigned int maxX, maxY;

// Prototypes
class Fish;
void DrawObject(int, int, list<string>, int);
void fill_randomly(list<Fish>*, list<string>, list<string>, int);
void print_statusbar(list<Fish>*);
void updateAquariumSize();

// Implementation
class Fish {
public:
	// Member variables
	string name;
	list<string> fish_ltr, fish_rtl;
	unsigned int x, y;
	int _id, speed, color;
	bool ltr = true;

	// Constructor
	Fish(int px, int py, list<string> pfish_ltr, list<string> pfish_rtl, int pspeed, string pname, int pcolor) {
		// Unique ID
		static int id = 0;
		_id = id++;

		// Fill member variables with parameters
		x = px;
		y = py;
		fish_ltr = pfish_ltr;
		fish_rtl = pfish_rtl;
		speed = pspeed;
		name = pname;
		color = pcolor;
	}

	// Methods
	void repaint() {
		if (ltr)
			DrawObject(x, y, fish_ltr, color);
		else
			DrawObject(x, y, fish_rtl, color);
		cout << name << " ";
	}
	void move_horizontally() {
		for (int i = 0; i < speed; i++) {
			if (ltr) x++;
			else x--;
			repaint();

			// Check if we reached the edge
			if (x > maxX - fish_rtl.front().length() - name.length() - 1) ltr = false;
			else if (x < 1) ltr = true;
		}
	}
	void move_vertically(bool up) {
		if (y > 1 && y < maxY - 5) {
			if (up)
				y--;
			else
				y++;
			system("cls");
			repaint();
		}
	}
	void turn() {
		if (x < maxX - fish_rtl.front().length() && x > 1) // Don't turn if on the edge
			ltr = !ltr;
	}
	void checkCollision(list<Fish> * fishlist) {
		list<Fish>::iterator it;
		for (it = fishlist->begin(); it != fishlist->end(); ++it) {
			if (this->x == it->x && this->y == it->y && this->_id != it->_id && (rand() % 100) < 2) { // 2% probability; faster fish have better chance
				// Collision reached
				lastEvent = it->name + "(" + to_string(it->_id) + ") has been killed";
				system("cls");
				it = fishlist->erase(it);
				return;
			}
		}
	}
	bool operator == (Fish fish) {
		return name == fish.name;
	}
};
void DrawObject(int x, int y, list<string> content, int color) { // Draw object at coordinates
	// Declare coord and initialize handle
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = x;
	coord.Y = y;

	// Set output properties, print to console and move
	SetConsoleTextAttribute(hConsole, color);
	for (auto& row : content) {
		SetConsoleCursorPosition(hConsole, coord);
		cout << row;
		coord.Y++;
	}
}
void fill_randomly(list<Fish> * fishlist, list<string> testfish_ltr, list<string> testfish_rtl, int count = 5) {
	for (int i = 0; i <= count - 1; i++)
		fishlist->push_front(Fish(rand() % maxX + 1, rand() % maxY + 1, testfish_ltr, testfish_rtl, rand() % 5 + 1, "Fish", rand() % (159 + 1 - 144) + 144));
}
void print_statusbar(list<Fish>* fishlist) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = 1;
	coord.Y = maxY-1;

	SetConsoleCursorPosition(hConsole, coord);
	SetConsoleTextAttribute(hConsole, 159);
	cout << "Fishes: " << fishlist->size() << " | ";
	cout << maxX << "-" << maxY << " | ";
	if (lastEvent != "") cout << lastEvent << " | ";
	cout << "Actions: New [n] Feed [f] Kill [k]";
}
void updateAquariumSize() {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	if (maxX != csbi.dwSize.X || maxY != csbi.dwSize.Y) {
		maxX = csbi.dwSize.X;
		maxY = csbi.dwSize.Y;
		system("cls");
	}
}

int main() {
	// Set up
	system("color 9F"); // 144 - 159
	system("mode 105, 30");
	system("cls");
	updateAquariumSize();
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
	list<string> derAAL_ltr = {
		"    __--__--__--__--___--__     ",
		" ///               (    o  \\   ",
		" \\\\\\__--__--__--__--___--__/ ",
		"                                " };
	list<string> derAAL_rtl = {
		"    __---__---__---__---__      ",
		"   / o    )               \\\\  ",
		"   \\__---__---__---__---__///  ",
		"                                " };

	// Load fish
	list<Fish> fishlist;
	fishlist.push_front(Fish(13, 12, testfish_ltr, testfish_rtl, 1, "Chiara", 159));
	fishlist.push_front(Fish(30, 12, derAAL_ltr, derAAL_rtl, 2, "Anna", 158));
	fill_randomly(&fishlist, testfish_ltr, testfish_rtl, 10);

	// Runtime loop
	list<Fish>::iterator it;
	it = fishlist.begin();
	do {
		// For all fish
		for (it = fishlist.begin(); it != fishlist.end(); ++it) {
			it->move_horizontally();
			it->checkCollision(&fishlist);

			// Random behaivor
			if ((rand() % 100) < 1) // 1% probability
				if ((rand() % 100) < 50) // 50% probability
					it->move_vertically(true); // up
				else
					it->move_vertically(false); // down
			if ((rand() % 100) < 5) // 5% probability
				it->turn();
		}
		updateAquariumSize();
		print_statusbar(&fishlist);
		this_thread::sleep_for(chrono::milliseconds(tick));
	} while (fishlist.size() > 0);

	return 0;
}