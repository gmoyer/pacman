#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <string>
using namespace std;

enum class EntityType { Player, Enemy, None };

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

	bool collidesWith(Entity* other); //checks if the entity collides with another

	//Position moveRelative(Direction direction); //move the entity in the relative direction.

	Position moveAbsolute(Position pos); //teleport the entity to a position.

	virtual EntityType getType() const { //must be implemented in subclasses
		return EntityType::None; 
	} 
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

	void takeTurn(); //requests player input, and moves accordingly

	void takeDamage(); //removes a player life, and randomly respawns the player
};
class Enemy : public Entity {
private:

public:
	Enemy(Position pos); //creates an enemy at position pos

	EntityType getType() const override { //must be implemented in subclasses
		return EntityType::Enemy; 
	} 
	
	//Position takeTurn(vector<Direction> validMoves); //use enemy AI to move enemy

};

#endif  // _PLAYER_H_
