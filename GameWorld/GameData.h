//
//  GameData.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_GameData_h
#define GameWorld_GameData_h

#include <iostream>
#include <ostream>

/**
 * A parent type that will define the basic functions and members for Damage and Health (and other similar data types
 * if we need them)
 */
struct GameData {
	
protected:
	
	unsigned long baseValue ;
	unsigned int modifier ;
	
public:
	
	GameData() :
		baseValue(0),
		modifier(1) {}
	
	GameData(long b) :
		baseValue(b),
		modifier(1) {}
	
	
	GameData(long b, long m) :
		baseValue(b),
		modifier(m) {}
	
	
	GameData(const GameData & other) :
		baseValue(other.baseValue),
		modifier(other.modifier) {}
	
	
	GameData & operator=(const GameData & rhs) {
		if (this != &rhs) {
			this->baseValue = rhs.baseValue ;
			this->modifier = rhs.modifier ;
		}
		return *this ;
	}
	
	
	/**
	 * @return The value of this GameData object (e.g. Health or Damage)
	 */
	unsigned long value() {
		return baseValue * modifier ;
	}
	
	/**
	 * See value()
	 *
	 * @return The total amount of this GameData value (e.g. Health or Damage
	 */
	unsigned long operator()() {
		return value() ;
	}
	
} ;



/**
 * A data structure that will hold damage values
 */
struct Damage : public GameData {
	
public:
	
	Damage() :
		GameData() {}
	
	Damage(long b) :
		GameData(b) {}
	
	Damage(long b, long m) :
		GameData(b, m) {}
	
	Damage(const Damage & other) :
		GameData(other) {}
	
} ;



/**
 * A data structure that will hold health values
 */
struct Health : public GameData {

public:
	
	Health() :
		GameData() {}
	
	Health(long b) :
		GameData(b) {}
	
	Health(long b, long m) :
		GameData(b, m) {}
	
	Health(const Health & other) :
		GameData(other) {}
	
} ;



/* Below are a few enums we can use in various ways through the game.
   Note for all enums here: the value -1 essentially can be considered as essentially 
   as an equivalent to NULL */
enum class Alert {
	
	danger = 0,
	nearbyEnemy = 1,
	lowHealth = 2,
	lowEnergy = 3, /*i.e. for abilities*/
	lowResource = 4,
	nearbyMoney = 5,
	nearbyBoost = 6,
	//add more here
	
} ;

enum class CharacterState {
	
	/*typically for player and NPCs*/
	normal = 0,
	inCombat = 1,
	inDialogue = 2,
	idle = 3,
	
} ;

enum class DoA {
	
	dead = 0,
	alive = 1
	
} ;

enum class Reaction {
	
	/*typically npc*/
	loyal = 0,
	friendly = 1,
	neutral = 2,
	unfriendly = 3,
	hostile = 4
	//add more here
} ;

enum Misc {
	//anything else?
} ;

inline
std::ostream & operator<<(std::ostream & os, const Alert & alt) {
	switch (alt) {
		case Alert::danger:
			os << "Danger" ;
			break;
		case Alert::nearbyEnemy:
			os << "Enemy nearby" ;
			break ;
		case Alert::lowHealth:
			os << "Health low" ;
			break ;
		case Alert::lowEnergy:
			os << "Energy low" ;
			break ;
		case Alert::lowResource:
			os << "Resource low" ;
			break ;
		case Alert::nearbyMoney:
			os << "Money nearby" ;
			break ;
		case Alert::nearbyBoost:
			os << "Nearby boost" ;
			break ;
	}
	return os ;
}

inline
std::ostream & operator<<(std::ostream & os, const CharacterState & cs) {
	switch (cs) {
		case CharacterState::idle:
			os << "Idle" ;
			break;
		case CharacterState::inCombat:
			os << "In combat" ;
			break ;
		case CharacterState::inDialogue:
			os << "In dialogue" ;
			break ;
		case CharacterState::normal:
			os << "Normal" ;
			break ;
		default:
			break;
	}
	return os ;
}

inline
std::ostream & operator<<(std::ostream & os, const DoA & doa) {
	switch (doa) {
		case DoA::alive:
			os << "Alive" ;
			break;
		case DoA::dead:
			os << "Dead" ;
			break ;
		default:
			break;
	}
	return os ;
}

inline
std::ostream & operator<<(std::ostream & os, const Reaction & react) {
	switch (react) {
		case Reaction::loyal:
			os << "Loyal" ;
			break ;
		case Reaction::friendly:
			os << "Friendly" ;
			break ;
		case Reaction::neutral:
			os << "Neutral" ;
			break ;
		case Reaction::unfriendly:
			os << "Unfriendly" ;
			break ;
		case Reaction::hostile:
			os << "Hostile" ;
			break ;
	}
	return os;
}






#endif
