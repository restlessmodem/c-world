#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>
#include <conio.h>
#include <chrono>
#include <thread>
#include <list>
#include <random>
using namespace std;

// Global variables
string lastEvent, selectedFishName;
unsigned int maxX = 150, maxY = 40, sealevel = 3;
double waterContamination = 0;
bool exitNow = false;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
default_random_engine generator; // for random numbers

// Configuration - global constants
const string FILEPATH = ".\\";
const int PROBABILITY_VERTICAL_MOVE = 1, PROBABILITY_VERTICAL_MOVE_UP = 50, PROBABILITY_TURN = 1, PROBABILITY_DEATH_ON_COLLISION = 5, PROBABILITY_PROCREATION_ON_COLLISION = 5, TICK_DURATION = 500;
int COLOR_RED = 20, COLOR_BLUE = 19, COLOR_GREEN = 26, COLOR_BLACK = 16, COLOR_WHITE = 31, COLOR_AVOID = 17;

// Prototypes
struct fishDesign;
class Fish;
void DrawObject(int, int, list<string>, int);
void print_statusbar(list<Fish>*);
void updateAquarium(int);
void hideConsoleInput();
bool checkIfFishExists(string, list<Fish>*);
void userInput(list<Fish>*);
int randRange(int, int);
void fishlistReadWrite(list<Fish>*, bool);
void drawEnvironment(bool state);
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
	double health = 100; // fish start with 100% health
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
		cout << " " << name << " - ";
		if (health < 30)
			SetConsoleTextAttribute(hConsole, COLOR_RED);
		else
			SetConsoleTextAttribute(hConsole, COLOR_GREEN);
		cout << (int)health << "% ";
		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}
	void move_horizontally() {
		for (int i = 0; i < speed; i++) {
			if (ltr) x++;
			else x--;
			repaint();

			// Check if we reached the edge
			if ((x == maxX - design.rtl.front().length() - name.length() - 5) || x == 1) {
				system("cls");
				ltr = !ltr;
			} else if ((x > maxX - design.rtl.front().length() - name.length() - 1) || x < 1) {
				x = 10; // Relocate poor fish who ended up outside the aquarium on console resize
				y = 10;
			}
		}
		health = health - (0.05 * (waterContamination / 5)); // moving costs health; gets worse with bad water quality
	}
	void move_vertically(bool up) {
		if (y > sealevel && y < maxY - 7) {
			if (up)
				y--;
			else
				y++;
			system("cls");
			repaint();
		}
		health = health - 0.1; // moving costs health
	}
	void turn() {
		if (x < maxX - design.rtl.front().length() && x > 1) { // Don't turn if on the edge
			if (ltr)
				x++;
			else
				x--;
			system("cls");
			ltr = !ltr;
		}
	}
	list<Fish>::iterator kill(list<Fish>* fishlist, list<Fish>::iterator it) {
		lastEvent = it->name + "(" + to_string(it->_id) + ") has been killed";
		system("cls");
		return fishlist->erase(it);
	}
	void checkCollision(list<Fish> * fishlist) {
		list<Fish>::iterator it;
		for (it = fishlist->begin(); it != fishlist->end(); ++it) {
			if (this->x == it->x && this->y == it->y && this->_id != it->_id) { // Collision reached; faster fish have less likely chance
				if ((rand() % 100) < PROBABILITY_DEATH_ON_COLLISION) { // Death
					it = kill(fishlist, it);
					return;
				} else if ((rand() % 100) < PROBABILITY_PROCREATION_ON_COLLISION) { // Procreation
					string childname = "Child of " + it->name + " & " + this->name;
					lastEvent = it->name + "(" + to_string(it->_id) + ") has produced the child " + childname;
					fishlist->push_front(Fish(randRange(1,maxX), randRange(1,maxY - 5), design, randRange(1,5), childname, randRange(COLOR_BLACK,COLOR_WHITE)));
				}
			}
		}
	}
	void rename(string newname) {
		this->name = newname;
	}
	bool operator == (Fish fish) {
		return _id == fish._id;
	}
};
void DrawObject(int x, int y, list<string> content, int color) { // Draw object at coordinates
	// Declare coord and initialize handle
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
	COORD coord;
	coord.X = 1;
	coord.Y = maxY-2;
	SetConsoleCursorPosition(hConsole, coord);
	SetConsoleTextAttribute(hConsole, COLOR_WHITE);

	// Individual fish controls
	cout << "Selected fish: ";
	list<Fish>::iterator it;
	it = fishlist->begin();
	if (selectedFishName == "" && fishlist->size() > 0) // If no fish selected, select first fish in list, if one is available
		selectedFishName = it->name;

	for (it = fishlist->begin(); it != fishlist->end(); ++it) {
		if (it->name == selectedFishName) {
			cout << it->name << " "; // TODO: fishs name in fishs color, error in red
			break;
		}
	}

	// Last event message
	if (lastEvent != "") {
		cout << " | ";
		if (lastEvent.find("ERROR") != -1 || lastEvent.find("WARNING") != -1) // If error or warning, then in red
			SetConsoleTextAttribute(hConsole, COLOR_RED);
		else
			SetConsoleTextAttribute(hConsole, COLOR_BLACK);
		cout << lastEvent;
		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}

	// Aquarium controls
	cout << "\n Fishes: " << fishlist->size() << " | ";
	cout << maxX << "-" << maxY << " | ";
	cout << "Water Contamination: " << (int)waterContamination << "% | ";
	
	cout << "Actions: New [n] Save & Quit [q] Feed All [f] Clean [c] Select [s] Kill [k] Rename [r]";
}
void updateAquarium(int tickcount = 0) {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	if (maxX != csbi.dwSize.X) { // Has console width changed?
		maxX = csbi.dwSize.X;
		system("cls");
	}

	if (tickcount % 2 == 0)
		drawEnvironment(true); // LTR
	else
		drawEnvironment(false); // RTL
	
	if (waterContamination + 0.05 <= 100) 
		waterContamination = waterContamination + 0.05;
	else
		waterContamination = 100;

	if (waterContamination == 40 || waterContamination == 70 || waterContamination == 100)
		lastEvent = "WARNING: Water Contamination has reached " + to_string((int)waterContamination) + "%!";
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
		} while (input != 'N' && input != 'Q' && input != 'S' && input != 'K' && input != 'R' && input != 'F' && input != 'C');
		
		if (input == 'N') { // New
			lastEvent = "Neuer Fisch Name?";
			hideConsoleInput();
			do {getline(cin, fishname);} while (fishname == "");
			if (!checkIfFishExists(fishname, fishlist)) {
				int randomcolor = 0;
				do {
					randomcolor = randRange(COLOR_BLACK, COLOR_WHITE);
				} while (randomcolor == COLOR_AVOID); // Avoid same color as background
				fishlist->push_front(Fish(randRange(1, maxX / 2), randRange(sealevel, maxY / 2), selectRandomFishDesign(fishlist), randRange(1, 5), fishname, randomcolor)); // Spawn somewhere in upper left quadrant
				lastEvent = "Fish has been added";
			} else {
				lastEvent = "ERROR: A fish with that name already exists :(";
			}
		} else if (input == 'Q') { // Quit
			exitNow = true;
		} else if (input == 'S') { // Select by name
			lastEvent = "Auszuwählender Fisch Name?";
			hideConsoleInput();
			do { getline(cin, fishname); } while (fishname == "");
			selectedFishName = fishname;
			if (checkIfFishExists(fishname, fishlist))
				lastEvent = "Fish '" + fishname + "' has been selected";
			else
				lastEvent = "ERROR: Fish '" + fishname + "' does not exist :(";
		} else if (input == 'K') { // Kill
			list<Fish>::iterator it;
			for (it = fishlist->begin(); it != fishlist->end(); ++it) {
				if (it->name == selectedFishName) {
					it = it->kill(fishlist, it);
					break;
				}
			}
		} else if (input == 'F') { // Feed
			list<Fish>::iterator it;
			for (it = fishlist->begin(); it != fishlist->end(); ++it) {
				if (it->health + 20 <= 100) // Don't overfeed
					it->health = it->health + 20;
				else
					it->health = 100;
			}
			lastEvent = "All fish have been fed";
		} else if (input == 'C') { // Clean Aquarium
			waterContamination = 0;
			lastEvent = "Water has been cleaned";
		} else if (input == 'R') { // Rename
			list<Fish>::iterator it;
			lastEvent = "Neuer Name?";
			hideConsoleInput();
			do { getline(cin, fishname); } while (fishname == "");

			for (it = fishlist->begin(); it != fishlist->end(); ++it) {
				if (it->name == selectedFishName) {
					it->rename(fishname);
					break;
				}
			}
		}
		system("cls");
	}
}
void fishlistReadWrite(list<Fish> *fishlist, bool write) {
	if (write) {
		ofstream outfile;
		outfile.open(FILEPATH + "savedfishlist.txt");
		outfile << waterContamination << endl;
		for (auto& fish : *fishlist) {
			outfile << fish.x << endl;
			outfile << fish.y << endl;
			for (auto& row : fish.design.ltr)
				outfile << row << endl;
			outfile << "stop" << endl;
			for (auto& row : fish.design.rtl)
				outfile << row << endl;
			outfile << "stop" << endl;
			outfile << fish.speed << endl;
			outfile << fish.name << endl;
			outfile << fish.color << endl;
			outfile << fish.health << endl;
		}
		cout << "Aquarium state has been saved!";
	} else {
		string tmp;
		ifstream infile;

		infile.open(FILEPATH + "savedfishlist.txt");
		getline(infile, tmp);
		waterContamination = atoi(tmp.c_str());
		while (!infile.eof()) {
			string name;
			fishDesign design;
			unsigned int x, y;
			int speed, color, health;
			bool ltr = true;

			getline(infile, tmp);
			if (tmp == "") break; // quit on empty line
			x = atoi(tmp.c_str());
			getline(infile, tmp);
			y = atoi(tmp.c_str());
			do {
				getline(infile, tmp);
				design.ltr.push_back(tmp);
			} while (tmp != "stop"); // to identify fish being complete
			design.ltr.pop_back();
			do {
				getline(infile, tmp);
				design.rtl.push_back(tmp);
			} while (tmp != "stop"); // to identify fish being complete
			design.rtl.pop_back();
			getline(infile, tmp);
			speed = atoi(tmp.c_str());
			getline(infile, name);
			getline(infile, tmp);
			color = atoi(tmp.c_str());
			getline(infile, tmp);
			health = atoi(tmp.c_str());

			Fish tmpfish = Fish(x, y, design, speed, name, color);
			tmpfish.health = health;
			fishlist->push_front(tmpfish);
			lastEvent = "Saved state has been loaded from file";
		}
	}
}
void drawEnvironment(bool state) {
	list<string> waves, sands;
	string wave = "_.-.", wave2 = "-._.", sand = ".";
	const list<string> seaweed1_ltr = {
		R"(   __  )",
		R"(  / /  )",
		R"( | |   )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( | |   )",
		R"(  \ \  )",
		R"(  |  | )"
	};
	const list<string> seaweed1_rtl = {
		R"(  __	  )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( | |	  )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( |  |  )"
	};
	const list<string> seaweed2_ltr = {
		R"(   __  )",
		R"(  / /  )",
		R"( | |	  )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( | |	  )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( |  |  )"
	};
	const list<string> seaweed2_rtl = {
		R"(  __	  )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( | |	  )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( | |	  )",
		R"(  \ \  )",
		R"(  |  | )"
	};
	const list<string> seaweed3_ltr = {
		R"(   __  )",
		R"(  / /  )",
		R"( | |	  )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( | |	  )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( | |	  )",
		R"(  \ \  )",
		R"(  |  | )"
	};
	const list<string> seaweed3_rtl = {
		R"(  __   )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( | |	  )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( | |	  )",
		R"(  \ \  )",
		R"(   | | )",
		R"(  / /  )",
		R"( |  |  )"
	};

	// Sand
	for (unsigned int x = 0; x < maxX - 1; x++) {
		sand.append(".");
	}
	sands = { sand };
	DrawObject(0, maxY - 3, sands, 31);
	if (state) {
		// Waves
		for (unsigned int x = 0; x < maxX / 2 - 2; x++) {
			if (x % 2 == 0)
				wave.append("_.");
			else
				wave.append("-.");
		}
		waves = {wave};

		// Seaweed
		DrawObject(maxX - 10, 30, seaweed1_ltr, COLOR_GREEN);
		DrawObject(maxX - 20, 28, seaweed2_ltr, COLOR_GREEN);
		DrawObject(maxX - 30, 26, seaweed3_ltr, COLOR_GREEN);
		DrawObject(maxX - 40, 30, seaweed1_ltr, COLOR_GREEN);
		DrawObject(maxX - 50, 28, seaweed2_ltr, COLOR_GREEN);
	} else {
		// Waves
		for (unsigned int x = 0; x < maxX / 2 - 2; x++) {
			if (x % 2 == 0)
				wave2.append("-.");
			else
				wave2.append("_.");
		}
		waves = {wave2};

		// Seaweed
		DrawObject(maxX - 10, 30, seaweed1_rtl, COLOR_GREEN);
		DrawObject(maxX - 20, 28, seaweed2_rtl, COLOR_GREEN);
		DrawObject(maxX - 30, 26, seaweed3_rtl, COLOR_GREEN);
		DrawObject(maxX - 40, 30, seaweed1_rtl, COLOR_GREEN);
		DrawObject(maxX - 50, 28, seaweed2_rtl, COLOR_GREEN);
	}
	DrawObject(0,3,waves,31); // Waves
}
bool checkIfFishExists(string searchname, list<Fish>* fishlist) {
	for (auto& fish : *fishlist) {
		if (fish.name == searchname)
			return true;
	}
	return false;
}
int randRange(int min, int max) {
	//return rand() % (max + 1 - min) + min;
	uniform_int_distribution<int> distribution(min, max);
	auto dice = std::bind(distribution, generator);
	return distribution(generator);
} 
fishDesign selectRandomFishDesign(list<Fish>* fishlist) {
	list<fishDesign> availableFishDesigns;
	// If fish designs havent been loaded yet, do it now
	if (availableFishDesigns.empty()) {
		string line;
		fishDesign tmpStruct;
		ifstream loadfish;

		loadfish.open(FILEPATH + "designs\\fish1_ltr.txt"); // first file
		int i = 1;
		while (loadfish.is_open()) { // Stop if file not found
			// left to right design
			while (getline(loadfish, line))
				tmpStruct.ltr.push_back(line);
			loadfish.close();

			// right to left design
			loadfish.open(FILEPATH + "designs\\fish" + to_string(i) + "_rtl.txt");
			if (!loadfish.is_open()) break;  // Stop if file not found
			while (getline(loadfish, line))
				tmpStruct.rtl.push_back(line);
			loadfish.close();

			availableFishDesigns.push_back(tmpStruct); // Push into Design list
			tmpStruct = {};
			i++;
			loadfish.open(FILEPATH + "designs\\fish" + to_string(i) + "_ltr.txt"); // LTR file for next iteration
		}

		// Check if at least one fish has been loaded
		if (availableFishDesigns.size() < 1) {
			lastEvent = "ERROR: Fish-Designs konnten nicht geladen werden! :(";
			system("cls");
			print_statusbar(fishlist);
			exitNow = true;
		} else {
			lastEvent = "Fish-Designs wurden erfolgreich geladen!";
		}
	}

	// Select random from list
	list<fishDesign>::iterator it = availableFishDesigns.begin();
	int random = randRange(1,availableFishDesigns.size()-1);
	std::advance(it, random); // move iterator to selected list index
	return *it;
}
int main() {
	// Console setup
	system("color 1F"); // Color and Size
	system("mode 150, 40");
	locale::global(locale("German_germany"));
	updateAquarium(); // set application size to console size
	
	// Misc setup
	int tickcount = 0; // tick duration in ms
	list<Fish> fishlist; // Create list of active fish

	// Load old state from file
	fishlistReadWrite(&fishlist, false);

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

		// Check health
		for (it = fishlist.begin(); it != fishlist.end(); ++it) {
			if (it->health <= 0) {
				it = it->kill(&fishlist, it);
				break;
			}
		}
		updateAquarium(tickcount);
		print_statusbar(&fishlist);
		
		tickcount++;
		this_thread::sleep_for(chrono::milliseconds(TICK_DURATION));
	} while (!exitNow);

	fishlistReadWrite(&fishlist, true);
	inputThread.join();
	return 0;
}