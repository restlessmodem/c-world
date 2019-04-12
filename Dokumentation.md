# Dokumentation C-World (Deutsch)  
### Struckturen und Klassen  
`struct fishDesign`: {string listen ltr(links nach rechts), rtl(rechts nach links)  
`Class fish`: {fish: name, design, position, unique Id, speed, color, health, bool für Richtung (ltr/rtl)  

#### Methoden der Klasse Fish:  
  * `Fish(int px, int py, fishDesign pdesign, int pspeed, string pname, int pcolor)` (Constuctor): (erschafft Fische und weist ihnen eine ID, Farbe, Name, Position, Design und Geschwindigkeit zu)  
  * `void repaint()`: zeichnet Fische (ltr or rtl), deren Namen, Gesundheitsstatus (mit Farbwechsel (grün über 30%; rot unter 30%)   
  * `void move_horizontally()`: ändert die X-Koordinate für Fische (Geschwindigkeit: 5 bedeutet fünf “Schritte”) zeichnen; prüft ob ein Fisch die Kante erreicht (Ja: Richtung ändern; prüft ebenso, ob Konsolengröße verändert wurde, wenn ja: Ist der Fisch noch innerhalb des Fensters? (Ja: setze den Fisch auf x:10 und y:10), jeder “Schritt” bedeutet 0,05% Gesundheitsverlust für jeden Fisch  
  * `void move_vertically(bool up)`: Boolsche Abfrage die den Fisch vertikal nach oben oder unten setzt, überprüft, ob der Fisch direkt über dem sealevel oder dem sand ist, um diesen niemals über Wasser oder auf Sand dazustellen, jeder vertikale Schritt kostet den Fisch 0.1% seiner Gesundheit  
  * `void turn()`: überprüft, ob die Fischkoordinate gleich einer der Konsolenrändern ist (um Doppeldrehung zu vermeiden) und ändert ja nach Situation die Richtung  
  * `list<Fish>::iterator kill(list<Fish>* fishlist, list<Fish>::iterator it)`: benötigt einen Pointer zur fishlist und erhält diesen via Parameter. Der zweite Parameter ist ein Iterator dieser fishlist. Gibt einen anderen Iterator zurück, welcher den globalen String, “lastevent” auf [fishname][fishId] setzt und diesen Fisch der übergegebenen fishlist dann beseitigt  
  * `void checkCollision(list<Fish> * fishlist)`: erneut ein Pointer auf die fishlist, iteriert über die fishlist und vergleicht die Position der gewählten Fischinstanz und vergleicht diese dann mit den Positionen aller anderen Fischinstanzen; sollten diese Instanzen kollidieren, kann dieses zu zwei Situationen führen:  1. Einer der kollidierenden Fische wird eliminiert (5%), oder 2. Es kommt zu einer Fortpflanzung (5%). Der neue Fisch erhält dann eine Kombination aus den Namen beider Elternfische und den “Hinweis” child of und benannt. “lastevent” wird der Situation entsprechend geupdated  
  * `void rename(string newname)`: übernimmt den User-Input als neuen Namen der gewählten Fisch Instanz  
  * `bool operator == (Fish fish)`: um zwei Fischinstanzen mit “==” vergleichen zu können (es werden dabei die einzigartigen ID’s verglichen)  

### Andere Methoden  
`void DrawObject(int x, int y, list<string> content, int color)`: benötigt eine Position, eine Liste an Strings (fish), und eine Farbe, um mit diesen Informationen ein Objekt zu den gegebenen Bedingungen auf die Konsole zu zeichnen   
`void print_statusbar(list<Fish>* fishlist)`: wird immer am unteren Rand der Konsole ausgespielt. Enthält Informationen über das Ökosystem an sich und die aktuell ausgewählte Fischinstanz. Außerdem wird “lastevent” abgebildet (wenn dies in roter Schrift angezeigt wird, handelt es sich bei der gebenen Informtion um eine Fehlermeldung (z.B. Fischdesign kann nicht geladen werden)) und alle verfügbaren Shortcuts für User-Input werden aufgelistet.   
`void updateAquarium(int tickcount = 0)`: greift sich die Breite des Konsolenfensters und setzt die Aquariumbreite auf diesen Wert (höhe wird ignoriert), außerdem wird die Hintergrundfarbe an den Wasserstatus angepasst    
`void hideConsoleInput()`: macht den User-Input auf der Konsole unsichtbar    
`void userInput(list<Fish> *fishlist)`: benötigt Parameter welcher einen Pointer zur fishlist liefert (mit  _getch() ), überprüft ob der User-Input zu den vordefinierten Shortcuts passt und wiederholt dies solange, bis  “exit now” auf true gesetzt wird.    
> #### Verfügbare Tastenkürzel:  
> *	n: Erstellt eine neue Fischinstanz und fragt den User nach einem Namen (alle Parameter bis auf den Namen wird bedingt zufällig gesetzt  
> *	q: setzt die Variabe “exit now” auf true, was das Programm beendet  
> *	s: Abfrage nach Fischnamen und wählt entsprechend aus  
> *	f: füttert die Fische um etwas Gesundheit zu regenerieren  
> *	k: eliminiert ausgewählten Fisch  
> *	r: Abfrage nach Fischnamen um einen Fisch anzuwählen und nach neuem Fischnamen, um den alten Namen mit dem Neuen zu ersetzen  

`void fishlistReadWrite(list<Fish> *fishlist, bool write)`: Pointer zur Fischliste und einen boolschen Wert um folgende zwei mögliche Befehle zu triggern   
 * Writing: schreibt den Inhalt der Fischliste in savedfishlist.txt  
 * Reading: liest den Inhalt der savedfishlist.txt und erstellt die Fische   
 
`void drawEnvironment(bool state)`: bekommt einen Parameter, um zwischen zwei unterschiedlichen Layouts des gleichen Designs hin und her zu animieren (waves, seaweed, sand) sowohl die Breite des Sandes als auch die der Wellen sind responsiv zur Konsolenbreite  
`bool checkIfFishExists(string searchname, list<Fish>* fishlist)`: erhält Parameter “searchname” und einen Pointer zur fishlist, überprüft ob ein Fischname bereits vorhanden ist  
`int randRange(int min, int max)`: nimmt die int-Parameter “min” & “max” an und nutzt die Library random um eine zufällige Zahl zu generieren  
`fishDesign selectRandomFishDesign(list<Fish>* fishlist)`:erhält einen Parameter zur fishlist und gibt ein Fischdesign zurück; wenn fishdesigns noch nicht geladen wurde, werden die Designs zufällig aus dem Design-Ordner eingespielt; Applikation schlägt fehl, wenn die Methode nicht aufgerufen werden kann  
 
`int main()`:  
* bereitet die Konsole vor (mit color, locale und size)  
* Methode “Fishlist read/write” wird lesend aufgerufen  
* die fishlist wird erstellt  
* ein zweiter Thread wird gestartet um den “User input” zu starten  
* danach wird die Laufzeit-Schleife gestartet, welche läuft bis “exit now” auf true gesetzt wird  
* jede Fischinstanz durchläuft folgende Schritte:  
  * `void move_horizontally()` ausführen 
  * `void checkCollision(list<Fish> * fishlist)` ausführen, 1%ige Chance auf “move vertical” und dabei 50%ige Chance für eine Abwärts- oder Aufwärtsbewegung , 1%ige Chance, dass “turn” ausgeführt wird  
  * überprüft Gesundheitsstatus (wenn Gesundheit auf 0 ist, wird “kill” ausgeführt)  
  * Aufruf der Methoden  
    * `void updateAquarium(int tickcount = 0)`
    * `void print_statusbar(list<Fish>* fishlist)`
    * `void drawEnvironment(bool state)` (springt zwischen true/false für jeden Durchgang (tick))”  
* Anschließend wird der Main-Thread für die Dauer eines Durchganges auf sleep gesetzt  
* nachdem die Laufzeit-Schleife beendet ist wird die `void fishlistReadWrite(list<Fish> *fishlist, bool write)`-Methode schreibend ausgeführt  
* Input- und Main-Thread werden zusammengeführt   
*	Die Applikation wird fehlerfrei mit dem Rückgabewert 0 beendet  

