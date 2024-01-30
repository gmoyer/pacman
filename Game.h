

#ifndef _GAME_H_
#define _GAME_H_

#include "Entity.h"
#include <vector>
#include <string>

#define BOARD_SIZE 10 //the width and height of the board
#define BOARD_INDENT "          " //the indent that we want before we start printing the board (makes it look nicer)

using namespace std;

//forward declarations
struct Position;
class Entity; class Player; class Enemy;


//enums
enum class SquareType { Wall, Dots, Treasure, Empty };
enum class Direction { Up, Down, Left, Right };

SquareType getSquareTypeFromChar(char c); //get the square time from a character

class Square {
private:
	Position pos;
	SquareType type; 
	Entity* occupant;
public:
	Square(Position pos, SquareType type_); //initializes a square
	Entity* getOccupant() const; //gets whoever is currently on the square (player/enemy)
	SquareType getType() const; //gets the type of the square
	char getChar(); //get what the square should display as
	bool isMoveValid(); //returns if an entity can move here
	bool isFillValid(); //returns if the square is open to be filled with enemy

	void changeType(SquareType newType); //change the type of the square (ex. Dots -> Empty)
	void changeOccupant(Entity* entity); //change the occupant of the square
};

class Board {
private:
	Square* arr[BOARD_SIZE][BOARD_SIZE];
	string inFile;
public:
	Board(string inputFile); //crease a board using the provided file

	Square* getSquare(Position pos); //returns the square at the position
	
	Position getRandomEmptySpace(); //returns a random space on the board that is empty (no wall or entity)

	void parseBoard(); //loads the board into array
	vector<Enemy*> populateBoard(int enemyCount); //populates the board with enemies and the same number of treasures. Returns the list of enemies

	//SquareType getSquareType(Position pos); //returns the type of the square at a position
	//void setSquareType(Position pos, SquareType type); //change the type of the square at a position
	//Entity* getSquareOccupant(Position pos); //returns the occupant of the entity at a position

	vector<Direction> getValidMoves(Position pos); //get the valid moves from a position. Moving onto the player counts as a valid move, but not onto enemies
	string getValidMovesString(Position pos); //get the valid moves but as a string

	Square* move(Entity* entity, Position pos); //move entity to pos, return the new square the entity is on. All moves MUST go through this function

	string printBoard(); //prints the entire board
};

class Game {
private:
	Board* board;
	Player* player;
	vector<Enemy*> enemies;
	int turnCount;
public:
	Game(); //creates a new game with 5 enemies and default board board.txt
	Game(string inputFile); //creates a new game with 5 enemies and custom board
	Game(const int enemyCount); // creates a new game with a specified number of enemies and default board
	Game(string inputFile, const int enemyCount); //creates a new game with custom enemies and custom board

	void setup(string inputFile, const int enemyCount); //prepares the game

	void startGame(); //cycles taking turns until game over

	void takeTurn(); //turn logic, performs player move and then performs enemy moves

	void killEnemy(Enemy* enemy); //delete enemy, remove its address from enemies array

	void gameOver(bool win); //ends the game, displays win or loss screen based on bool

	~Game(); //clean up memory

};

#endif  // _GAME_H_
