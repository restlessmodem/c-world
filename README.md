# c-world
Your windows console is now a happy place for fish.
This project has been created as a career school project for the application development class ITB-83 to demonstrate basic C/C++ language knowledge.     

---

The detailled documentation of the code can be found here: **[Documentation English](Documentation.md)**  
You can run the finished application right away with the files in **[c-world-release.zip](c-world-release.zip)**  

#### Things to know:
* **For the application to start successfully the folder "designs" needs to be available in the application folder and be filled with fish designs.**
* Fish Designs can be added in the "designs" folder by the user in the given format but at least one design needs to be availabe for the application to run successfully. Nine are available by default. They need to be padded with whitespaces.
* Fish specific actions (shortcuts like k = kill or r = rename) work on the selected fish. You may select a fish by pressing 's' and typing its name.
* The number next to a fish's name indicates its health. Fish lose health over time, you can restore it by feeding. You can only feed all fish at the same time.
* The `waterContamination` gets worse over time. You can mitigate it by cleaning. With rising water contamination the fish start to lose health much quicker.
* Resizing of the console window is supported but only horizontally and not smaller than the statusbar. Failure to comply will cause display errors.
* With vertical resizing it can happen that fish end up outside of the console window. These fish are relocated to the coordinates  10,10.
* The state of the fish and aquarium is saved over sessions but only if the application is exited by pressing 'q'. If you exit with  `Alt+F4` or by pressing X on the window your progress will be lost.
* The probabilites can be easily modified with the constants at the top of the source code.
* For tests and simulations the "tick" duration can be lowered.
* Because the console cursor in moved around in the main thread, `cin` is not a valid method for getting the shortcut inputs. `_getch()` is used instead.
* Sometimes the cursor of the windows console paints characters to the wrong location while moving. This is a known issue of the windows console.
