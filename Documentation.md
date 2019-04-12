# Dokumentation C-World (English)
### Structs and Classes
`struct fishDesign`: {string lists for the ASCII designs of the fish; ltr(left to right), rtl(right to left)}  
`Class fish`: {fish: name, design, position, unique Id, speed, color, health, bool for direction (ltr/rtl)
#### Fish methods:
  * `Fish(int px, int py, fishDesign pdesign, int pspeed, string pname, int pcolor)` (Constuctor): (creates fish and gives ID, gives color, name, position, design, speed)  
  * `void repaint()`: draws fish (ltr or rtl), name, health(with changing color (green above 30%; red below 30%)   
  * `void move_horizontally()`: change x coordinate for fish (speed 5 equals five “steps”) redraw; checks if fish reaches border of console (yes: changes direction; also checks if console window width was changed, if so: is fish out of bounce? (yes: relocate to x:10 and y:10) for every step each fish loses 0.05% of health  
  * `void move_vertically(bool up)`: Boolean which lets the fish move upwards or downwards; checks if fish is right below sealevel or above sand level to never redraw the fish above the water or in the sand, every vertical step a fish takes consumes 0.1% of its health  
  * `void turn()`: checks if fish coordinate equals border and turns fish if that isn’t the case to avoid double turning  
  * `list<Fish>::iterator kill(list<Fish>* fishlist, list<Fish>::iterator it)`: needs a pointer to the fishlist and gets it via parameters. The second parameter is an iterator of that fishlist. Returns a different iterator, which sets the global string “lastevent” to [fishname][fishId], erases fish the given iterator points on in the given fishlist  
  * `void checkCollision(list<Fish> * fishlist)`: again a pointer to the fishlist is needed, iterates over the given fishlist and compares the position of the executing fish instance to the position of all other fish instances; if they meet a collision is reached (two different outcomes: 1. Fish gets killed (5%) 2. Procreation of the colliding fish and new childfish gets name of the colliding parent fish (5%) (child of fish1 and fish2) “lastevent” is updated accordingly (every parameter except name is random in certain defined bounds)  
  * `void rename(string newname)`: gets the current name of a fish and overwrites the name with the user input  
  * `bool operator == (Fish fish)`: to be able to compare two different fishes with “==” (by comparing the fish unique id’s)  

### Other Methods
`void DrawObject(int x, int y, list<string> content, int color)`: needs a position, a list of strings (fish) to be drawn and a color, sets cursor of console to given position, changes color on said position to the given color and draws the string for each line   
`void print_statusbar(list<Fish>* fishlist)`: is always presented at the bottom of the aquarium, contents are data about the impressive ecosystem and the currently “selected” fish, gives information about the latest event via “lastevent” (is shown in red colors when confronted with errors (eg. Fish can’t be displayed) also displays available shortcuts  
`void updateAquarium(int tickcount = 0)`: gets the width of the console window and sets the aquarium width to console width (height is ignored)  
also sets the background color depending on its status
`void hideConsoleInput()`: makes user input to the console invisible  
`void userInput(list<Fish> *fishlist)`: needs parameter which contains a pointer to fishlist, gets keyboard input via _getch(), checks if user input equals the predefined shortcuts; loops until “exit now” is true
> #### Available Shortcuts:
> *	n: ask user for name for new fish and creates new fish object (everything besides name is given random in predefined bounds)
> *	q: sets global variable “exit now” to true; effectively causes application to quit
> *	s: asks for fish name and selects the fish with given name
> *	f: feeds the fish to regain a part of everyone’s health
> *	k: kills selected fish
> *	r: asks for fishname, asks then for new fishname and changes the old name of the selected fish to the new one 
 
`void fishlistReadWrite(list<Fish> *fishlist, bool write)`: needs pointer to fishlist and gets Boolean to check if we are reading or writing  
  *	Writing: writes fishlist contents to savedfishlist.txt  
  *	Reading: reads content from savedfishlist.txt and recreates fishes accordingly  
  
`void drawEnvironment(bool state)`: gets parameter state to alternate between two different layouts to simulate animation (waves, seaweed, sand) width of sand and waves are responsive to console width  
`bool checkIfFishExists(string searchname, list<Fish>* fishlist)`: gets parameter “searchname” and a pointer to the fishlist, checks if a name is already taken for any of the other fish  
`int randRange(int min, int max)`: takes two int parameters “min” & “max”, uses library random to return random number between “min” and “max”  
`fishDesign selectRandomFishDesign(list<Fish>* fishlist)`: gets a parameter with pointer to fishlist and returns fish design; if fishdesigns has not been loaded yet, it loads fishdesigns from the given files in the design directory and selects randomly; application fails if method can’t be executed  

`int main()`:  
* prepares console (with colors, locale and size)
* fishlist gets created and will be read with method “Fishlist read/write”
* a second thread for method “user input” gets invoked
* afterwards the runtime loop is started until “exit now” is set to true
* for every fish there the following steps will be executed:
  * execute `void move_horizontally()`
  * execute `void checkCollision(list<Fish> * fishlist)`, 1% chance “move vertical” and in this 50% chance for a movement for either upwards or downwards, 1% chance for “turn”
  * check health (if health equals 0 -> kill fish)
  * call on methods 
    * `void updateAquarium(int tickcount = 0)`
    * `void print_statusbar(list<Fish>* fishlist)`
    * `void drawEnvironment(bool state)` (alternates between true/false for every tick)
* at last the main thread will be set to sleep for one tic duration, after runtime loop is exited
* `void fishlistReadWrite(list<Fish> *fishlist, bool write)` method will be writing to savedfishlist.txt and input-thread is joined with main-thread
* Application exits gracefully while returning 0  



