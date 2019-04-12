# c-world
Your windows console is now a happy place for fish  
This project is being created as a school project for the application development class ITB-83.
Die detaillierte Dokumentation des Codes kann hier gefunden werden: [Documentation Deutsch](Dokumentation.md)
The detailled documentation of the code can be found here: [Documentation English](Documenation.md)

#### Wichtige Hinweise:
* Damit die Anwendung erfolgreich started kann, muss der "designs" Ordner vorhanden und mit Fisch-Designs gefüllt sein.
* Fisch-Designs können im "desings" Ordner im gegebenen Format hinzugefügt werden, allerdings muss mindestens ein Design vorhanden sein, damit die Anwendung lauffähig ist. Sie müssen mit Leerzeichen gepaddet sein.
* Fisch-spezifishe Aktionen (Tastenkürzel wie k = kill oder r = rename) werden mit dem ausgewälten Fisch ausgeführt. Sie können einen Fisch auswählen, indem sie 's' drücken und seinen Namen eingeben.
* Die Zahl neben dem Namen eines Fisches zeigt seine Gesundheit an. Fische verlieren mit der Zeit Gesundheit, Sie können sie wiederherstellen, indem Sie sie füttern. Es können nur alle Fische gleichzeitig gefüttert werden.
* Die "Wasser Contamination" wird mit der Zeit schlimmer. Sie können ihr mit Reinigung entgegenwirken. Je schlimmer die "Wasser Contamination" wird, desto schneller verlieren die Fische auch ihre Gesundheit.
* Die Größe des Konsolenfesters zu ändern ist unterstützt, jedoch nur horizontal und nicht schmaler als die Status Bar. Missachten dieser Anweisung wird Anzeigefehler verursachen.
* Mit vertikaler Größenänderung können Fische sich außerhalb des Konsolenfensters wiederfinden. Diese Fische werden zu den Koordinatien 10,10 verschoben.
* Der Status der Fische und des Aquariums wird über verschiedene Sitzungen gespeichert, allerdings nur wenn die Anwendung mit dem Tastenkürzel 'q' beendet wird. Wenn Sie die Anwendung mit `Alt+F4` oder mit X des Fensters beenden, wird Ihr Fortschritt verloren gehen.
* Die Wahrscheinlichkeiten können leicht modifiziert werden. Sie befinden sich am Beginn des Quellcodes.
* Da der "Cursor" der Konsole im Hauptthread umherbewegt wird, ist `cin` keine akzeptable Methode um die Tastenkürzeleingaben abzufragen. Stattdessen wird `_getch()` verwendet.

#### Things to know:
* For the application to start successfully the folder "designs" needs to be available in the application folder and be filled with fish designs.
* Fish Designs can be added in the "designs" folder by the user in the given format but at least one design needs to be availabe for the application to run successfully. They need to be padded with whitespaces.
* Fish specific actions (shortcuts like k = kill or r = rename) work on the selected fish. You may select a fish by pressing 's' and typing its name.
* The number next to a fish's name indicates its health. Fish lose health over time, you can restore it by feeding. You can only feed all fish at the same time.
* The `waterContamination` gets worse over time. You can mitigate it by cleaning. With rising water contamination the fish start to lose health much quicker.
* Resizing of the console window is supported but only horizontally and not smaller than the statusbar. Failure to comply will cause display errors.
* With vertical resizing it can happen that fish end up outside of the console window. These fish are relocated to the coordinates  10,10.
* The state of the fish and aquarium is saved over sessions but only if the application is exited by pressing 'q'. If you exit with  `Alt+F4` or by pressing X on the window your progress will be lost.
* The probabilites can be easily modified with the constants at the top of the source code.
* Because the console cursor in moved around in the main thread, `cin` is not a valid method for getting the shortcut inputs. `_getch()` is used instead.
