# Red-Wrong--OOP
OOP Project for second semester , A number guessing game named as Red Wrong

This project is a console-based number guessing game developed in C++ using Object-Oriented Programming (OOP) principles. It features Single and Multiplayer modes, a countdown timer, a smart hint system, difficulty levels, and score tracking.

---

## Project Features

* Single and Multiplayer modes
* 10-second countdown timer using `time.h` (works in Dev C++ 5.11)
* Hint system to help players get closer to the secret number
* Difficulty levels: Easy, Medium, Hard
* Score tracking and automatic winner announcement
* Modular OOP structure using multiple classes

---

## OOP Concepts Used

* Encapsulation: All class data is private and controlled through public methods
* Abstraction: Internal logics like hints and timers are hidden from the main program
* Association: Classes interact without deep dependency
* Aggregation: Classes like `Player`, `Hint`, and `Delay` are used together in game logic

---

## Team Contributions

### Eman Maqsood (Team Lead & Main Developer)

* Designed full game structure and connected all classes
* Developed main gameplay loop and player turn system
* Implemented scoring and winner logic
  Worked on: `main.cpp`, `GameLogic`, `WinnerDeterminer`

---

### Abeera Zainab

* Created the player input system and difficulty selection logic
* Managed screen messages and display prompts
  Worked on: `InputHandler`, `Display`, `Difficulty`

---

### Eshal Naveed

* Developed the timer system using `time.h` and `kbhit()` from `conio.h`
* Built the hint system and secret number generator
* Created the Player class
  Worked on: `Delay`, `Hint`, `SecretNumberGenerator`, `Player`

---

## Technologies Used

* Language: C++
* Compiler: Dev C++ 5.11
* Libraries: `<iostream>`, `<stdlib.h>`, `<conio.h>`, `<time.h>`

---

## Future Improvements

* Add file handling to save high scores
* Introduce a graphical interface using SFML or QT
* Add detailed player statistics
* Make hint logic more intelligent based on difficulty

---

## How to Run

1. Open the project in Dev C++ 5.11 or any compatible C++ compiler.

2. Compile and run `main.cpp`.

3. Follow the menu to choose game mode and difficulty.

4. Enjoy playing the game.

---
