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

// Configuration - global constants
const int PROBABILITY_VERTICAL_MOVE = 1;
const int PROBABILITY_VERTICAL_MOVE_UP = 50;
const int PROBABILITY_TURN = 5;
const int PROBABILITY_DEATH_ON_COLLISION = 5;
const int PROBABILITY_PROCREATION_ON_COLLISION = 5;
const int TICK_DURATION = 350; // in milliseconds
const string FILEPATH = "C:\\Users\\pfisterc\\Documents\\git\\c-world\\fishdesigns\\";

// Prototypes
struct fishDesign;
class Fish;
void DrawObject(int, int, list<string>, int);
void print_statusbar(list<Fish>*);
void updateAquariumSize();
void hideConsoleInput();
bool checkIfFishExists(string, list<Fish>*);
int randRange(int, int);
fishDesign selectRandomFishDesign(list<Fish>*);

// Implementation
struct fishDesign {
	list<string> ltr;
	list<string> rtl;
};
class Fish {
public:
	// Member variables
	string name;
	//list<string> fish_ltr, fish_rtl;
	fishDesign design;
	unsigned int x, y;
	int _id, speed, color;
	bool ltr = true;

	// Constructor
	Fish(int px, int py, fishDesign pdesign, int pspeed, string pname, int pcolor) {
		// Unique ID
		static int id = 0;
		_id = id++;

		// Fill member variables with parameters
		x = px;
		y = py;
		design = pdesign;
		//fish_ltr = design.ltr;
		//fish_rtl = design.rtl;
		speed = pspeed;
		name = pname;
		color = pcolor;
	}

	// Methods
	void repaint() {
		if (ltr)
			DrawObject(x, y, design.ltr, color);
		else
			DrawObject(x, y, design.rtl, color);
		cout << name << " ";
	}
	void move_horizontally() {
		for (int i = 0; i < speed; i++) {
			if (ltr) x++;
			else x--;
			repaint();

			// Check if we reached the edge
			if ((x == maxX - design.rtl.front().length() - name.length() - 1) || x == 1)
				ltr = !ltr;
			else if ((x > maxX - design.rtl.front().length() - name.length() - 1) || x < 1) {
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
		if (x < maxX - design.rtl.front().length() && x > 1) // Don't turn if on the edge
			ltr = !ltr;
	}
	void checkCollision(list<Fish> * fishlist) {
		list<Fish>::iterator it;
		for (it = fishlist->begin(); it != fishlist->end(); ++it) {
			if (this->x == it->x && this->y == it->y && this->_id != it->_id) { // Collision reached; faster fish have lower chance
				if ((rand() % 100) < PROBABILITY_DEATH_ON_COLLISION) { // Death
					lastEvent = it->name + "(" + to_string(it->_id) + ") has been killed";
					system("cls");
					it = fishlist->erase(it);
					return;
				} else if ((rand() % 100) < PROBABILITY_PROCREATION_ON_COLLISION) { // Procreation
					string childname = "Child of " + it->name + " & " + this->name;
					lastEvent = it->name + "(" + to_string(it->_id) + ") has produced the child " + childname;
					fishlist->push_front(Fish(randRange(1,maxX), randRange(1,maxY - 5), design, randRange(1,5), childname, randRange(144,159)));
				}
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
void userInput(list<Fish> *fishlist) {
	string fishname;
	int input;

	while (!exitNow) {
		do {
			input = _getch();
			input = toupper(input);
		} while (input != 'N' && input != 'Q' && input != 'S');
		
		if (input == 'N') { // New
			lastEvent = "Neuer Fisch Name?";
			hideConsoleInput();
			do {getline(cin, fishname);} while (fishname == "");
			if (!checkIfFishExists(fishname, fishlist)) {
				system("cls");
				int randomcolor = 0;
				do {
					randomcolor = randRange(144, 159);
				} while (randomcolor == 153); // Avoid same color as background
				fishlist->push_front(Fish(randRange(1, maxX / 2), randRange(1, maxY / 2), selectRandomFishDesign(fishlist), randRange(1, 5), fishname, randomcolor)); // Spawn somewhere in upper left quadrant
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
	}
}
bool checkIfFishExists(string searchname, list<Fish>* fishlist) {
	for (auto& fish : *fishlist) {
		if (fish.name == searchname)
			return true;
	}
	return false;
}
int randRange(int min, int max) {
	return rand() % (max + 1 - min) + min;
}
fishDesign selectRandomFishDesign(list<Fish>* fishlist) {
	list<fishDesign> availableFishDesigns;
	// If fish designs havent been loaded yet, do it now
	if (availableFishDesigns.empty()) {
		string line;
		fishDesign tmpStruct;
		ifstream loadfish;

		loadfish.open(FILEPATH + "fish1_ltr.txt"); // first file
		int i = 1;
		while (loadfish.is_open()) { // Stop if file not found
			// left to right design
			while (getline(loadfish, line))
				tmpStruct.ltr.push_back(line);
			loadfish.close();

			// right to left design
			loadfish.open(FILEPATH + "fish" + to_string(i) + "_rtl.txt");
			if (!loadfish.is_open()) break;  // Stop if file not found
			while (getline(loadfish, line))
				tmpStruct.rtl.push_back(line);
			loadfish.close();

			availableFishDesigns.push_back(tmpStruct); // Push into Design list
			tmpStruct = {};
			i++;
			loadfish.open(FILEPATH + "fish" + to_string(i) + "_ltr.txt"); // LTR file for next iteration
		}

		// Check if at least one fish has been loaded
		if (availableFishDesigns.size() < 1) {
			lastEvent = "Fish-Designs konnten nicht geladen werden! :(";
			system("cls");
			print_statusbar(fishlist);
			exitNow = true;
		} else {
			lastEvent = "Fish-Designs wurden erfolgreich geladen!";
		}
	}

	// Select random list
	list<fishDesign>::iterator it = availableFishDesigns.begin();
	int random = randRange(1,availableFishDesigns.size()-1);
	advance(it, random); // move iterator to selected list index
	return *it;
}
int main() {
	// Console setup
	system("color 9F"); // Color and Size [Range: 144 - 159]
	system("mode 150, 40");
	locale::global(locale("German_germany"));
	updateAquariumSize(); // set application size to console size
	
	// Misc setup
	srand((unsigned)time(NULL)); // randomness seed
	int tick = TICK_DURATION; // tick duration in ms
	list<Fish> fishlist; // Create list of active fish

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
			if ((rand() % 100) < PROBABILITY_VERTICAL_MOVE) // Vertical move
				if ((rand() % 100) < PROBABILITY_VERTICAL_MOVE_UP)
					it->move_vertically(true); // up
				else
					it->move_vertically(false); // down
			if ((rand() % 100) < PROBABILITY_TURN) // Turn horizontally
				it->turn();
		}
		updateAquariumSize();
		print_statusbar(&fishlist);
		this_thread::sleep_for(chrono::milliseconds(tick));
	} while (!exitNow);

	inputThread.join();
	return 0;
}