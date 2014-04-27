//
//  GameData.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_GameData_h
#define GameWorld_GameData_h

#define GLOBAL_MAX_X_ 179
#define GLOBAL_MIN_X_ 0
#define GLOBAL_MAX_Y_ 37
#define GLOBAL_MIN_Y_ 0


#include <iostream>
#include <ostream>

#include "../Util/BoundsCheck.hpp"

using namespace std ;

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
	
	GameData(unsigned long b) :
		baseValue(b),
		modifier(1) {}
	
	
	GameData(unsigned long b, unsigned m) :
		baseValue(b),
		modifier(m) {}
	
	
	GameData(const GameData & other) :
		baseValue(other.baseValue),
		modifier(other.modifier) {}
	
	GameData(GameData && other) :
		baseValue(other.baseValue),
		modifier(other.modifier) {}
	
	
	GameData & operator=(const GameData & rhs) {
		if (this != &rhs) {
			this->baseValue = rhs.baseValue ;
			this->modifier = rhs.modifier ;
		}
		return *this ;
	}
	
	GameData & operator=(GameData && rhs) {
		if (this != &rhs) {
			this->baseValue = rhs.baseValue ;
			this->modifier = rhs.modifier ;
		}
		return *this ;
	}
	
	bool operator==(GameData & rhs) {
		return (this->value() == rhs.value()) ;
	}
	
	bool operator!=(GameData & rhs) {
		return !(this->operator==(rhs)) ;
	}
	
	bool operator>(GameData & rhs) {
		return (this->value() > rhs.value()) ;
	}
	
	bool operator<(GameData & rhs) {
		return (this->value() < rhs.value()) ;
	}
	
	bool operator>=(GameData & rhs) {
		return (this->value() >= rhs.value()) ;
	}
	
	bool operator<=(GameData & rhs) {
		return (this->value() <= rhs.value()) ;
	}
	
	GameData & operator+(const GameData & rhs) {
		this->baseValue += rhs.baseValue ;
		this->modifier += rhs.modifier ;
		return *this ;
	}
	
	GameData & operator-(const GameData & rhs) {
		this->baseValue -= rhs.baseValue ;
		this->modifier -= rhs.modifier ;
		return *this ;
	}
	
	/**
	 * Override the << output stream operator
	 */
	inline
	friend ostream & operator<<(std::ostream & os, GameData & gmd) {
		os << gmd.value() ;
		return os ;
	}
	
	/**
	 * Override the << output stream operator
	 */
	inline
	friend ostream & operator<<(std::ostream & os, const GameData & gmd) {
		os << gmd.value() ;
		return os ;
	}
	
	
	/**
	 * @return The value of this GameData object (e.g. Health or Damage)
	 */
	unsigned long value() const {
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
	
	Damage(unsigned long b, unsigned m) :
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
	
	Health(unsigned long b, unsigned m) :
		GameData(b, m) {}
	
	Health(const Health & other) :
		GameData(other) {}

	
} ;



/* Below are a few enums we can use in various ways through the game.
   Note for all enums here: the value -1 essentially can be considered as essentially 
   as an equivalent to NULL */
enum class Alert {
	
	noalert = -1,
	danger = 0,
	nearbyEnemy = 1,
	lowHealth = 2,
	lowEnergy = 3, /*i.e. for abilities*/
	lowResource = 4,
	nearbyMoney = 5,
	nearbyBoost = 6
	//add more here
	
} ;

enum class CharacterState {
	nocs = -1,
	/*typically for player and NPCs*/
	normal = 0,
	inCombat = 1,
	inDialogue = 2,
	idle = 3

} ;

enum class DoA {
	nodoa = -1,
	dead = 0,
	alive = 1
	
} ;

enum class Reaction {
	
	noreact = -100,
	/*typically npc*/
	loyal = 2,
	friendly = 1,
	neutral = 0,
	unfriendly = (-friendly),
	hostile = (-loyal)
	//add more here
} ;


inline
std::ostream & operator<<(std::ostream & os, const Alert & alt) {
	switch (alt) {
		case Alert::noalert:
			os << "No alert";
			break ;
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
		case CharacterState::nocs:
			os << "No character state";
			break ;
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
		case DoA::nodoa:
			os << "No DoA value" ;
			break ;
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
		case Reaction::noreact:
			os << "No reaction" ;
			break ;
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



struct Locking {
	static std::mutex sharedMutex ;
};

extern bool GLOBAL_CONTINUE_SIGNAL ;

extern const long GLOBAL_MAX_X ;
extern const long GLOBAL_MIN_X ; 
extern const long GLOBAL_MAX_Y ;
extern const long GLOBAL_MIN_Y ;

static const BoundsCheck<long> defaultCheck(GLOBAL_MAX_X, GLOBAL_MIN_X, GLOBAL_MAX_Y, GLOBAL_MIN_Y) ;





#endif
