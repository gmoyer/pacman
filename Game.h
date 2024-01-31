

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
enum Direction; enum EntityType;


//enums
enum class SquareType { Wall, Dots, Treasure, Empty };

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
	string getStr(); //get what the square should display as
	
	bool isMoveValid(bool isPlayer); //returns if an entity can move here
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

	vector<Direction> getValidMoves(Position pos); //get the valid moves from a position. Moving onto the player counts as a valid move, but not onto enemies
	
	Square* move(Entity* entity, Position pos); //move entity to pos, return the new square the entity is on. All moves MUST go through this function

	string printBoard(); //prints the entire board
};

class Game {
private:
	Board* board;
	Player* player;
	vector<Enemy*> enemies;
	int turnCount;
	string statusDisplay;
public:
	Game(); //creates a new game with 5 enemies and default board board.txt
	Game(string inputFile); //creates a new game with 5 enemies and custom board
	Game(const int enemyCount); // creates a new game with a specified number of enemies and default board
	Game(string inputFile, const int enemyCount); //creates a new game with custom enemies and custom board

	void setup(string inputFile, const int enemyCount); //prepares the game

	void startGame(); //cycles taking turns until game over

	string validMovesString(vector<Direction> validDirections); //get the valid moves but as a string
	
	string printState(string custom); //print the current game state

	void playerTurn(); //player turn logic

	void moveEntity(Entity *entity, Position newPos); //move entity to a new position

	void playerCollide(Enemy* enemy); //handle player collision

	void enemiesTurn(); //all enemy turn logic
	
	void gameOver(bool win); //ends the game, displays win or loss screen based on bool

	~Game(); //clean up memory

};

#endif  // _GAME_H_
