#include <iostream>
#include <string>
#include <map>
#include <limits>
#include <cctype>
#include <algorithm>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Game state
map<string, int> resources = {{"minerals", 10}, {"wood", 5}};
map<string, int> buildings = {{"mine", 0}, {"lumbermill", 0}, {"barracks", 0}};
map<string, int> units = {{"worker", 1}, {"warrior", 0}};

// Utility to convert string to lowercase
string toLowerCase(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Utility to trim whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last = str.find_last_not_of(" \t");
    if (first == string::npos) return "";
    return str.substr(first, last - first + 1);
}

// Function to display the current game state
void displayGameState() {
    clearScreen();
    cout << "\n--- Game State ---" << endl;
    cout << "Resources: Minerals = " << resources["minerals"] << ", Wood = " << resources["wood"] << endl;
    cout << "Buildings: Mines = " << buildings["mine"] << ", Lumbermills = " << buildings["lumbermill"] << ", Barracks = " << buildings["barracks"] << endl;
    cout << "Units: Workers = " << units["worker"] << ", Warriors = " << units["warrior"] << endl;
    // Progress hint
    cout << "Victory Goal: 100 minerals and 5 warriors (Progress: " 
         << resources["minerals"] << "/100 minerals, " 
         << units["warrior"] << "/5 warriors)" << endl;
    cout << "--------------------" << endl;
}

// Function to gather resources
void gatherResources(const string& resource) {
    if (units["worker"] == 0) {
        cout << "You need workers to gather " << resource << "." << endl;
        return;
    }
    if (resource == "minerals") {
        int amount = units["worker"] * (1 + buildings["mine"]);
        resources["minerals"] += amount;
        cout << "Workers gathered " << amount << " minerals." << endl;
    } else if (resource == "wood") {
        int amount = units["worker"] * (1 + buildings["lumbermill"]);
        resources["wood"] += amount;
        cout << "Workers gathered " << amount << " wood." << endl;
    }
}

// Function to build structures
void buildStructure(const string& building) {
    if (building == "mine") {
        if (resources["minerals"] >= 15) {
            resources["minerals"] -= 15;
            buildings["mine"]++;
            cout << "Mine built." << endl;
        } else {
            cout << "Not enough minerals to build a mine (cost: 15)." << endl;
        }
    } else if (building == "lumbermill") {
        if (resources["wood"] >= 10) {
            resources["wood"] -= 10;
            buildings["lumbermill"]++;
            cout << "Lumbermill built." << endl;
        } else {
            cout << "Not enough wood to build a lumbermill (cost: 10)." << endl;
        }
    } else if (building == "barracks") {
        if (resources["minerals"] >= 20 && resources["wood"] >= 15) {
            resources["minerals"] -= 20;
            resources["wood"] -= 15;
            buildings["barracks"]++;
            cout << "Barracks built." << endl;
        } else {
            cout << "Not enough resources to build a barracks (cost: 20 minerals, 15 wood)." << endl;
        }
    }
}

// Function to produce units
void produceUnit(const string& unit) {
    if (unit == "worker") {
        if (resources["minerals"] >= 10) {
            resources["minerals"] -= 10;
            units["worker"]++;
            cout << "Worker produced." << endl;
        } else {
            cout << "Not enough minerals to produce a worker (cost: 10)." << endl;
        }
    } else if (unit == "warrior") {
        if (buildings["barracks"] == 0) {
            cout << "You need a barracks to produce warriors." << endl;
        } else if (resources["minerals"] >= 15 && resources["wood"] >= 5) {
            resources["minerals"] -= 15;
            resources["wood"] -= 5;
            units["warrior"]++;
            cout << "Warrior produced." << endl;
        } else {
            cout << "Not enough resources to produce a warrior (cost: 15 minerals, 5 wood)." << endl;
        }
    }
}

// Function to process player commands
void processCommand(string command) {
    command = toLowerCase(trim(command));
    
    if (command == "gather minerals") {
        gatherResources("minerals");
    } else if (command == "gather wood") {
        gatherResources("wood");
    } else if (command == "build mine") {
        buildStructure("mine");
    } else if (command == "build lumbermill") {
        buildStructure("lumbermill");
    } else if (command == "build barracks") {
        buildStructure("barracks");
    } else if (command == "produce worker") {
        produceUnit("worker");
    } else if (command == "produce warrior") {
        produceUnit("warrior");
    } else if (command == "status") {
        displayGameState();
    } else if (command == "help") {
        clearScreen();
        cout << "\n--- Commands ---" << endl;
        cout << "gather minerals   - Gather minerals using workers." << endl;
        cout << "gather wood      - Gather wood using workers." << endl;
        cout << "build mine       - Build a mine (cost: 15 minerals, increases mineral gathering)." << endl;
        cout << "build lumbermill - Build a lumbermill (cost: 10 wood, increases wood gathering)." << endl;
        cout << "build barracks   - Build a barracks (cost: 20 minerals, 15 wood, allows warrior production)." << endl;
        cout << "produce worker   - Produce a worker (cost: 10 minerals)." << endl;
        cout << "produce warrior  - Produce a warrior (cost: 15 minerals, 5 wood, requires barracks)." << endl;
        cout << "status           - Display current game state." << endl;
        cout << "help             - Display this help message." << endl;
        cout << "quit             - Exit the game." << endl;
        cout << "------------------" << endl;
    } else if (command == "quit") {
        cout << "Thanks for playing!" << endl;
        exit(0);
    } else {
        cout << "Unknown command. Type 'help' for a list of commands." << endl;
    }
}

int main() {
    clearScreen();
    cout << "Welcome to Simple Strategy!" << endl;
    cout << "You start with some basic resources and a worker." << endl;
    cout << "Type 'help' to see available commands." << endl;

    string command;
    while (true) {
        cout << "> ";
        getline(cin, command);
        processCommand(command);

        // Check for negative resources (defensive programming)
        if (resources["minerals"] < 0) resources["minerals"] = 0;
        if (resources["wood"] < 0) resources["wood"] = 0;

        // Win condition
        if (resources["minerals"] >= 100 && units["warrior"] >= 5) {
            clearScreen();
            cout << "\n--- Victory! ---" << endl;
            cout << "You have amassed significant mineral wealth and a small army!" << endl;
            cout << "Final State:" << endl;
            displayGameState();
            break;
        }
    }

    return 0;
}