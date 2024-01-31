#ifndef _PLAYER_H_
#define _PLAYER_H_


#include <string>
#include <vector>
using namespace std;

enum class EntityType { Player, Enemy, None };
enum class Direction { Up, Down, Left, Right, None };

struct Position {
	int row;
	int col;

	bool operator==(const Position &other) const {
		return row == other.row && col == other.col;
	}
};

class Entity { //never create just an entity
protected:
	Position pos;
public:

	Position getPosition() const {return pos; }  // returns the position of the entity

	virtual EntityType getType() const { //must be implemented in subclasses
		return EntityType::None; 
	}

	void setPosition(Position p); //moves the entity to new position
	virtual ~Entity() {}
};

class Player : public Entity {
private:
	bool treasure;
	int lives;
	int points;
public:
	Player(); //creates a new player in the top left corner.

	EntityType getType() const override { //must be implemented in subclasses
		return EntityType::Player; 
	} 
	
	bool hasTreasure() {return treasure; } //returns if the player currently has a treasure or not
	int getLives() { return lives; } //returns the lives of the player
	int getPoints() { return points; } //return the number of points the player has
	
	void setTreasure(bool t); //sets treasure
	void addPoints(int p); //adds p points

	bool takeDamage(); //removes a player life, return true if player dies
	
	
	Position takeTurn(vector<Direction> validDirections); //requests player input, and moves accordingly

};
class Enemy : public Entity {
private:

public:
	Enemy(Position pos); //creates an enemy at position pos

	EntityType getType() const override { //must be implemented in subclasses
		return EntityType::Enemy; 
	} 
	
	Position takeTurn(vector<Direction> validMoves, Position playerPos); //use enemy AI to move enemy

};

#endif  // _PLAYER_H_
