/* CS106 D6
* Author: Joe Klemmer
* Instructor: Panos Mastrogiannis
* Week 9 - Final Project
*/

// Include required libraries
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <ctime>
#include <cctype>

using namespace std;

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!  WARNING  !!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!  THIS IS NOT PORTABLE CODE  !!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// The definition below _should_ work on most WinXX and Linux
// terminal emulators however it is not garunteed to work at all.
// Do not use this in any production code.
//
// Clear the screen
#ifdef _WIN32
#  define CLEAR "CLS"
#elif defined(linux)
#  define CLEAR "clear"
#else
#error unknown platform
#endif

// Function definitions (I may have gotten carried away here)
void playGame(int gridSize);
void buildGrid(int gridSize);
void displayGrid(int gridSize);
void getUsersCoordinates(int gridSize);
void displayHelpInfo();
void readGameData();
void placeShip();
void pauseHere();
int calculateHit();

// Global variable declarations
string playingGrid[35][35];				// Game board array
string shipLocation[35][35];			// Array to hold the position of the ship on the grid
int easyLevel, normalLevel, hardLevel;	// Used to distinguish game dificulty
int acrossCoord, downCoord;				// User selected coordinates on the grid

// Program entry point
int main(void)
{
	// Variable declarations for main()
	bool playingTheGame = true;	// Used to control menu choice loop
	char choice;				// Stores the user selected menu choice

	// Greetings Professor Falken
	cout << endl << endl
		 << "Welcome to \"Sink My Ship\" "
		 << endl << endl;

	// Seed random number generator
	srand((unsigned)time(0));

	// Main loop of program
	while (playingTheGame)
	{
		// Display main menu
		cout << "Please select your choice:" << endl;
		cout << "1 - Easy" << endl;
		cout << "2 - Normal" << endl;
		cout << "3 - Hard" << endl << endl;
		cout << "H - Help" << endl;
		cout << "Q - Quit" << endl;
		cout << "-------------------------" << endl;
		cout << "Please enter your choice: ";

		// Obtains users menu choice
		cin >> choice;

		// Read in the game data file
		readGameData();

		// Decides which menu option to execute
		switch (choice)
		{
			case '1':
				// Play the easy level
				playGame(easyLevel);
				break;
			case '2':
				// Play the normal level
				playGame(normalLevel);
				break;
			case '3':
				// Play hard level
				playGame(hardLevel);
				break;
			case 'H':
			case 'h':
				// Display help/instructions
				displayHelpInfo();
				break;
			case 'Q':
			case 'q':
				// Get me outta here!
				// Set flag for while loop to end the program
				playingTheGame = false;
				cout << endl
					 << "Thank you for playing." << endl
					 << "Goodbye."
					 << endl << endl;
				break;
			default:
				//incorrect selection error message
				cout << endl
					 << "Sorry, that is an invalid option." << endl
				     << "Please select either \"1\", \"2\", \"3\", \"H\" or \"Q\"." << endl
				     << endl << endl;
		} // End of switch
	} // End of while loop

	// And since we're all done we exit the program
	return 0;

} // End of main

// Read in game data from file GameData.txt
// (I'm tempted to allow for data file input
// on the command line but we can save that
// for next semester)
void readGameData()
{
	// Initialize file streams
	ifstream inFile;

	// Open the file
	inFile.open("GameData.txt");

	// Read data from files
	inFile >> easyLevel >> normalLevel >> hardLevel;

	// Close the files
    inFile.close();
} // End of function readGameData

// The actual game loop; play continues until ship is sunk
// User may use Ctrl-C to break out of program
void playGame(int gridSize)
{
	// Hit/Sunk counter
	int isItSunk, playCounter;

	// Build the game board and randomly place the ship;
	// then display the grid
	buildGrid(gridSize);
	placeShip();
	displayGrid(gridSize);

	// Initialize counters to zerro
	isItSunk = 0;
	playCounter = 0;

	// Game play loop
	while (isItSunk < 4)
	{
		// Incriment the play counter
		playCounter += 1;
		// Prompt user for coordinates
		getUsersCoordinates(gridSize);
		// Check if ship is sunk
		isItSunk += calculateHit();
		// Display the grid
		displayGrid(gridSize);
	} // End of while loop

	// (I was tempted to add a high score file here.  Just a flat file
	// where users can can enter their names and have their hit counts
	// saved for posterity.  Maybe keep the top 5 or 10 scores for each
	// level and add a high score option to the main menu.  But I'll
	// save that for fun over the winter break.  Maybe)

	// Display the winning grid
	displayGrid(gridSize);

	// Congradulatiions for winning
	cout << endl << endl << "\"Hey!  You sunk my ship!\"" << endl;
	cout << "It took you " << playCounter << " shots to sink it." << endl;
	cout << endl << endl;

	// Call for a pause
	pauseHere();

} // End of function playGame

// Build and initialize the grid
void buildGrid(int gridSize)
{
	// Loop counter variables
	int i, j;

	// Build playing grid
	// Put a "." in every array position
	for (i=0; i<gridSize; i++)
	{
		for (j=0; j<gridSize; j++)
		{
			playingGrid[i][j] = ".";
		} // End for loop
	} // End for loop
} // End of function buildGrid

// Display the grid
void displayGrid(int gridSize)
{
	// Temporaty use of system("cls") to clear the screen
	// It's ugly and definitely not portable but it'll do for now
	// SEE WARNING AT BEGINNING OF CODE
	system(CLEAR);

	// Loop counter variables
	int i, j;

	// Display numbers across top
	for (i=0; i < gridSize; i++)
	{
		// Funkyness to get the across display all neat and tidy
		// Primarily to satisfy my Obsessive/Complusive'ness
		if (i == 0)
		{
			cout << "   ";
		} // End if

		if (i < 9)
		{
			cout << i+1 << " ";
		}
		else
		{
			cout << i+1;
		} // End if
	} // End for loop

	cout << endl << "  /";
	for (i=0; i< gridSize; i++)
		cout << "--";
	cout << endl;

	// Display playing grid
	for (i=0; i<gridSize; i++)
	{
		// Display down numbers
		cout << setw(2) << i+1 << "|";

		// Display the actual grid
		for (j=0; j<gridSize; j++)
		{
			cout << playingGrid[i][j] << " ";
		} // End for loop
		cout << endl;
	} // End for loop
} // End of function displayGrid

// Get the coordinated from user input
void getUsersCoordinates(int gridSize)
{
	// Prompt user
	cout << "Enter coordinates" << endl;

	// Get the across coordinate; must be within the range of the grid
	do
	{
		cout << "Across: ";
		cin  >> acrossCoord;

		if (acrossCoord > gridSize || acrossCoord <= 0)
		{
			cout << "Value must be greater than zero and less than or equal to " << gridSize << endl;
		} // End if
	} while (acrossCoord > gridSize || acrossCoord <= 0);  // End do..while loop

	// Get the down coordinate; must be within the range of the grid
	do
	{
		cout << "Down: ";
		cin  >> downCoord;

		if (downCoord > gridSize || downCoord <= 0)
		{
			cout << "Value must be greater than zero and less than or equal to " << gridSize << endl;
		} // End if
	}  while (downCoord > gridSize || downCoord <= 0);  // End do..while loop

	// Mark it as potential hit
	playingGrid[downCoord-1][acrossCoord-1] = "@";
} // End of function getUsersCoordinates

// Randomly place ship on grid
void placeShip(void)
{
	// Function variables
	int direction;
	int limiter;
	int gridSize;
	int i;
	// Use enum for main directional values
	enum positions {NORTH, EAST, SOUTH, WEST};
	positions shipOrentationOnGrid = NORTH;

	// Generate random number between 0 and 3 for direction
	direction = rand()%4;
	// Generate random numbers for starting position of ship on the grid
	// Make sure that the limiter does not allow for ships to be positioned off the board
	limiter = rand()%6;
	gridSize = rand()%10;

	// Get the position pointer for the direction
	shipOrentationOnGrid = static_cast<positions> (shipOrentationOnGrid + direction);

	// Place the ship in the proper direction and mark the coordinates where it is sitting; the
	// ship is set at a size of four.  This can be made a veriable size by using a random generator,
	// adding ship sizes to the input file or some other method.  (If I get the Final exam finished
	// with enough time to spare I'll make the ship size random between 4 and 8 (Maybe)).
	//
	// Mark the position of the ship with the "@" sign
	switch (shipOrentationOnGrid)
	{
		case NORTH:
			for (i=0; i<4; i++)
			{
				shipLocation[limiter+i][gridSize] = "@";
				//playingGrid[limiter+i][gridSize] = "@";           //<----- These are for testing
			}													    // Uncomment to see where
			break;												    // the ship is
		case SOUTH:
			for (i=0; i<4; i++)
			{
				shipLocation[(limiter + 3) - i][gridSize] = "@";
				//playingGrid[(limiter + 3) - i][gridSize] = "@";   //<--- These are for testing
			}														// Uncomment to see where
			break;													// the ship is
		case EAST:
			for (i=0; i<4; i++)
			{
				shipLocation[gridSize][(limiter + 3) - i] = "@";
				//playingGrid[gridSize][(limiter + 3) - i] = "@";	//<--_ These are for testing
			}														// Uncomment to see where
			break;													// the ship is
		case WEST:
			for (i=0; i<4; i++)
			{
				shipLocation[gridSize][limiter + i] = "@";
				//playingGrid[gridSize][limiter + i] = "@";         //<----- These are for testing
			}													    // Uncomment to see where
			break;												    // the ship is
		default:
			// If we get here something really bad has happened
			cout << "Something blew up!" << endl;
	} // End of switch
} // End of function placeShip

// Figure out if the user got a hit; keep track of hits
// and mark the grid with a hit or miss
int calculateHit()
{
	// Hit counter
	int hitCounter = 0;

	// See if there's been a hit.  If yes mark the game board
	// with an "X" for a hit or "*" for a miss and set the
	// hit counter.  (There's probably a better way to do this
	// with boolians)
	if (playingGrid[downCoord-1][acrossCoord-1] == shipLocation[downCoord-1][acrossCoord-1])
	{
		playingGrid[downCoord-1][acrossCoord-1] = "X";
		hitCounter++;
	}
	else
	{
		playingGrid[downCoord-1][acrossCoord-1] = "*";
	} // End if

	// Send back the results of the hit check
	return hitCounter;
} // End of function calculateHit

// Display help instructions if the user requests them
void displayHelpInfo()
{
	cout << endl << endl << "This game is similar to Battle Ship." << endl << endl;

	cout << "The object of the game is to sink the ship.  There will be one ship " << endl
		 << "on the grid.  The player will choose horizontal and virtical coordinates " << endl
		 << "and the game will calculate whether those coordiantes were a hit or a " << endl
		 << "miss.  An \"X\" will mark a hit and \"*\" a miss.  The difficulty levels " << endl
		 << "will be set as so; " << endl;

	cout << "\t\tEasy will be a grid of " << easyLevel << " x " << easyLevel << endl
		 << "\t\tNormal will be a grid of " << normalLevel << " x " << normalLevel << endl
		 << "\t\tHard will be a grid of " << hardLevel << " x " << hardLevel << endl << endl;

	cout << "You may exit the game at any time by hitting \"Ctrl-C\"." << endl;

	// Call for a pause
	pauseHere();

} // End of function displayHelpInfo

void pauseHere(void)
{
	cout << endl << "Press the \"Enter\" key to continue . . . ";

	cin.ignore();
	cin.ignore();

	cout << endl;
}
