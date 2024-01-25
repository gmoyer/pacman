#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
#include "Game.h"

using namespace std;

struct Position {
	int row;
	int col;

	bool operator==(const Position &other) const {
		return row == other.row && col == other.col;
	}
};

class Entity { //never create just an entity
public:

	Position getPosition() const {return pos; }  // returns the position of the entity

	bool collidesWith(Entity* other); //checks if the entity collides with another

	Position moveRelative(Direction direction); //move the entity in the relative direction. MUST use board.move()

	Position moveAbsolute(Position pos); //teleport the entity to a position. MUST use board.move()

private:
	Position pos;
	Board* board;
	Game* game;
};

class Player : public Entity {
private:
	bool treasure;
	int lives;
	int points;
public:
	Player(Game* game); //creates a new player in the top left corner.

	bool hasTreasure() {return treasure; } //returns if the player currently has a treasure or not
	int getLives() { return lives; } //returns the lives of the player
	int getPoints() { return points; } //return the number of points the player has

	void takeTurn(); //requests player input, and moves accordingly

	void takeDamage(); //removes a player life, and randomly respawns the player
};
class Enemy : public Entity {
private:

public:
	Enemy(Game* board); //creates an enemy at a random valid position
	Enemy(Game* board, Position pos); //creates an enemy at position pos

	void takeTurn(Board* board); //use enemy AI to move enemy, resolve anything

};

#endif  // _PLAYER_H_
