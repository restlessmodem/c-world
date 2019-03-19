#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <list>
using namespace std;

// Global variables
string lastEvent, selectedFishName;
unsigned int maxX, maxY;
bool exitNow = false;

// Prototypes
class Fish;
void DrawObject(int, int, list<string>, int);
void print_statusbar(list<Fish>*);
void updateAquariumSize();
void hideConsoleInput();
void checkIfFishExists(string);

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
			if ((x == maxX - fish_rtl.front().length() - name.length() - 1) || x == 1)
				ltr = !ltr;
			else if ((x > maxX - fish_rtl.front().length() - name.length() - 1) || x < 1) {
				x = 10; // Relocate fish who ended up outside the aquarium on console resize
				y = 10;
			}
		}
	}
	void move_vertically(bool up) {
		if (y > 1 && y < maxY - 7) {
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
void print_statusbar(list<Fish>* fishlist) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coord;
	coord.X = 1;
	coord.Y = maxY-1;
	SetConsoleCursorPosition(hConsole, coord);
	SetConsoleTextAttribute(hConsole, 159);

	// Aquarium controls
	cout << "Fishes: " << fishlist->size() << " | ";
	cout << maxX << "-" << maxY << " | ";
	if (lastEvent != "") cout << lastEvent << " | ";
	cout << "Actions: New [n] Quit [q] Select [s] | Ausgewählter Fisch: ";

	// Individual fish controls
	list<Fish>::iterator it;
	it = fishlist->begin();

	if (selectedFishName == "" && fishlist->size() > 0) // If no fish selected, select first fish in list, if one is available
		selectedFishName = it->name;

	for (it = fishlist->begin(); it != fishlist->end(); ++it) {
		if (it->name == selectedFishName) {
			cout << it->name << " "; // TODO: fishs name in fishs color
			break;
		}
	}
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
void hideConsoleInput() {
	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode = 0;
	GetConsoleMode(hStdin, &mode);
	SetConsoleMode(hStdin, mode & (~ENABLE_ECHO_INPUT));
}
bool checkIfFishExists(string searchname, list<Fish> *fishlist) {
	for (auto& fish : *fishlist) {
		if (fish.name == searchname)
			return true;
	}
	return false;
}
void userInput(list<Fish> *fishlist) {
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
	string fishname;

	int input;
	do {
		do {
			input = _getch();
			input = toupper(input);
		} while (input != 'N' && input != 'Q' && input != 'S');
		
		if (input == 'N') { // New
			lastEvent = "Neuer Fisch Name?";
			hideConsoleInput();
			do {getline(cin, fishname);} while (fishname == "");
			if (!checkIfFishExists(fishname, fishlist)) {
				fishlist->push_front(Fish(13, 12, testfish_ltr, testfish_rtl, 1, fishname, 145));
				system("cls");
				lastEvent = "Fish has been added";
			} else {
				lastEvent = "A fish with that name already exists :(";
			}
		} else if (input == 'Q') { // Quit
			exitNow = true;
		} else if (input == 'S') { // Prev
			lastEvent = "Auszuwählender Fisch Name?";
			hideConsoleInput();
			do { getline(cin, fishname); } while (fishname == "");
			selectedFishName = fishname;
			system("cls");
			if (checkIfFishExists(fishname, fishlist))
				lastEvent = "Fish '" + fishname + "' has been selected";
			else
				lastEvent = "Fish '" + fishname + "' does not exist :(";
		}
	} while (!exitNow);
}

int main() {
	// Console setup
	system("color 9F"); // Color and Size [144 - 159]
	system("mode 150, 40");
	locale::global(locale("German_germany"));
	updateAquariumSize(); // set application size to console size
	
	// Misc setup
	srand((unsigned)time(NULL)); // randomness seed
	int tick = 200; // tick duration in ms

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

	// Runtime loop
	thread inputThread(userInput, &fishlist);
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
	} while (!exitNow);

	inputThread.join();
	return 0;
}