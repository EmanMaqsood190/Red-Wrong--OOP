#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <conio.h>
#include <cmath>  // For abs()
#ifdef _WIN32
#include <windows.h>
#endif
using namespace std;

// Delay utility class
class Delay {
public:
    static void wait(double seconds) {
        #ifdef _WIN32
        Sleep((DWORD)(seconds * 1000));
        #else
        time_t start = time(0);
        while (difftime(time(0), start) < seconds) {}
        #endif
    }
};

// Display-related functions
class Display {
public:

    void clearScreenWithTitle() {
        system("cls");
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

        // Set text color to bright red
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);

        cout << "\t\t\t\t\t====================================\n";
        cout << "\t\t\t\t\t   \t  RED WRONG\n";
        cout << "\t\t\t\t\t====================================\n\n";

        // Reset to default white
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    void setTextColor(int color) {
        #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
        #endif
    }

    void shakeEffect() {
        setTextColor(12); // Red
        cout << "\n\n\t\tWRONG GUESS!\n\n";
        setTextColor(7);
        Delay::wait(0.5);
        cout << "\n";
    }

    void wrongGuessBuzzer() {
        setTextColor(12);
        #ifdef _WIN32
        Beep(1500, 100);
        #else
        cout << "\a";
        #endif
        setTextColor(7);
    }

    void winBuzzer() {
        setTextColor(10);
        #ifdef _WIN32
        Beep(1200, 100);
        Beep(1500, 100);
        Beep(1800, 100);
        #else
        cout << "\a\a";
        #endif
        setTextColor(7);
    }
};

// Hint class - original hints fully restored
class Hint {
public:
    void provideHint(int secretNumber, int attempt, int maxAttempts) {
        if (attempt == maxAttempts) return; // No hint on last attempt

        switch (attempt % 3) {
            case 1:
                cout << "Hint: The secret number is " 
                     << (secretNumber % 2 == 0 ? "even." : "odd.") << endl;
                break;
            case 2:
                cout << "Hint: The secret number is " 
                     << (secretNumber % 5 == 0 ? "a multiple of 5." : "not a multiple of 5.") << endl;
                break;
            case 0:
                if (secretNumber < 49) {
                    cout << "Hint: The secret number is in the range 1 to 50." << endl;
                }
                break;
        }
    }
};

// Player class
class Player {
private:
    string name;
    int score;
    int lastGuess;
    bool hasGuessed;

public:
    Player(string playerName) : name(playerName), score(0), lastGuess(0), hasGuessed(false) {}

    string getName() { return name; }
    int getScore() { return score; }
    int getLastGuess() { return lastGuess; }
    bool hasMadeGuess() { return hasGuessed; }

    void addScore(int points) { score += points; }
    void setLastGuess(int guess) { 
        lastGuess = guess; 
        hasGuessed = true;
    }
};

// Difficulty class to manage attempts and levels
class Difficulty {
private:
    int maxAttempts;
    int level;

public:
    Difficulty() : maxAttempts(0), level(0) {}

    void setLevel(int choice) {
        level = choice;
        switch (choice) {
            case 1: maxAttempts = 7; break;
            case 2: maxAttempts = 5; break;
            case 3: maxAttempts = 3; break;
            default: maxAttempts = 5;
        }
    }

    int getMaxAttempts() { return maxAttempts; }
    int getLevel() { return level; }
};

// InputHandler class for timed input with 10 seconds limit
class InputHandler {
public:
    // Returns -1 if time up or invalid input; otherwise returns guessed number
    static int getTimedGuess(int timeLimit) {
        time_t start = time(0);
        int secondsLeft = timeLimit;
        string input;
        bool inputComplete = false;

        while (secondsLeft > 0) {
            if (kbhit()) {
                char c = getch();
                if (c == '\r' || c == '\n') {
                    inputComplete = true;
                    break;
                } else if (c == '\b' && !input.empty()) {
                    input.erase(input.length() - 1, 1);
                    cout << "\b \b";
                    cout.flush();
                } else if (isdigit(c)) {
                    input += c;
                    cout << c;
                    cout.flush();
                }
            }

            time_t now = time(0);
            int elapsed = difftime(now, start);
            if (elapsed >= timeLimit - secondsLeft + 1) {
                secondsLeft--;
            }
        }

        cout << "\n";

        if (!inputComplete) return -1;
        if (input.empty()) return -1;

        int guess = atoi(input.c_str());
        if (guess < 1 || guess > 100) return -1;

        return guess;
    }
};

// SecretNumberGenerator class for secret number generation
class SecretNumberGenerator {
public:
    static int generate() {
        srand(time(0));
        return 1 + (rand() % 100);
    }
};

// GameLogic class to handle the gameplay mechanics per turn
class GameLogic {
private:
    int secretNumber;
    Difficulty difficulty;
    Hint hint;
    Display display;

public:
    GameLogic(int secret, Difficulty diff) : secretNumber(secret), difficulty(diff) {}

    bool playTurn(Player& player, int attempt) {
        cout << "\n\n" << player.getName() << "'s turn (Attempt " << attempt << "/" << difficulty.getMaxAttempts() << "):\n";
        cout << "You have 10 seconds to guess!\n";

        int guess = InputHandler::getTimedGuess(10);
        if (guess == -1) {
            display.wrongGuessBuzzer();
            cout << "Time's up or invalid guess! You took too long or input was invalid.\n";
            Delay::wait(1);
            return false;
        }

        player.setLastGuess(guess);
        hint.provideHint(secretNumber, attempt, difficulty.getMaxAttempts());

        if (guess == secretNumber) {
            display.winBuzzer();
            display.setTextColor(10);
            cout << player.getName() << " wins! " << guess << " is the secret number!\n";
            display.setTextColor(7);
            player.addScore(10 * (difficulty.getMaxAttempts() - attempt + 1));
            Delay::wait(1);
            return true;
        } else {
            display.wrongGuessBuzzer();
            display.shakeEffect();
            if (guess > secretNumber) {
                cout << "Too high!\n";
            } else {
                cout << "Too low!\n";
            }
            Delay::wait(1);
            return false;
        }
    }

    int getSecretNumber() { return secretNumber; }
    Difficulty getDifficulty() { return difficulty; }
};

// WinnerDeterminer class to decide the winner in multiplayer mode
class WinnerDeterminer {
private:
    Display display;

public:
    void determineWinner(Player& p1, Player& p2, int secretNumber) {
        cout << "\nGame Over! Secret number was " << secretNumber << ".\n";

        if (!p1.hasMadeGuess() && !p2.hasMadeGuess()) {
            cout << "Neither player made a valid guess! It's a tie.\n";
        } else if (!p1.hasMadeGuess()) {
            cout << p2.getName() << " wins (Player 1 didn't make a guess)!\n";
            p2.addScore(5);
        } else if (!p2.hasMadeGuess()) {
            cout << p1.getName() << " wins (Player 2 didn't make a guess)!\n";
            p1.addScore(5);
        } else {
            int diff1 = abs(p1.getLastGuess() - secretNumber);
            int diff2 = abs(p2.getLastGuess() - secretNumber);
            if (diff1 < diff2) {
                cout << p1.getName() << " wins (closer guess)!\n";
                p1.addScore(5);
            } else if (diff2 < diff1) {
                cout << p2.getName() << " wins (closer guess)!\n";
                p2.addScore(5);
            } else {
                cout << "It's a tie!\n";
            }
        }
        cout << p1.getName() << "'s score: " << p1.getScore() << endl;
        cout << p2.getName() << "'s score: " << p2.getScore() << endl;
        Delay::wait(2);
    }
};

// Main function

       int main() {
    
    char playAgain;

    do {
        Display display;
        display.clearScreenWithTitle();

        cout << "Welcome to the Number Guessing Game, Now roam around the numbers from 1 to 100!\n";
        cout << "Choose mode:\n1. Single Player\n2. Multiplayer\nEnter choice: ";
        int modeChoice;
        cin >> modeChoice;
        cin.ignore();

        cout << "Choose difficulty level:\n1. Easy (7 attempts)\n2. Medium (5 attempts)\n3. Hard (3 attempts)\nEnter choice: ";
        int diffChoice;
        cin >> diffChoice;
        cin.ignore();

        Difficulty difficulty;
        difficulty.setLevel(diffChoice);

        int secretNumber = SecretNumberGenerator::generate();

        if (modeChoice == 1) {
            string playerName;
            cout << "Enter your name: ";
            getline(cin, playerName);

            Player player(playerName);
            GameLogic game(secretNumber, difficulty);

            for (int attempt = 1; attempt <= difficulty.getMaxAttempts(); ++attempt) {
                if (game.playTurn(player, attempt)) {
                    break; // player won
                }
            }

            cout << "\nGame Over! Secret number was " << secretNumber << ".\n";
            cout << player.getName() << "'s score: " << player.getScore() << endl;
        } else if (modeChoice == 2) {
            string p1Name, p2Name;
            cout << "Enter Player 1 name: ";
            cin.ignore(); // To handle leftover newline
            getline(cin, p1Name);
            cout << "Enter Player 2 name: ";
            getline(cin, p2Name);

            Player player1(p1Name);
            Player player2(p2Name);
            GameLogic game(secretNumber, difficulty);
            WinnerDeterminer winnerDeterminer;

            for (int attempt = 1; attempt <= difficulty.getMaxAttempts(); ++attempt) {
                if (game.playTurn(player1, attempt)) break;
                if (game.playTurn(player2, attempt)) break;
            }

            winnerDeterminer.determineWinner(player1, player2, secretNumber);
        } else {
            cout << "Invalid mode choice.\n";
        }

        cout << "Thank you for playing!\n";
        cout << "\nDo you want to play again? (Y/N): ";
        cin >> playAgain;

    } while (playAgain == 'Y' || playAgain == 'y');

    cout << "\nPress any key to exit...";
    while (!kbhit()) {}
    getch();
    return 0;
}
