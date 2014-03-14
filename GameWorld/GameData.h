//
//  GameData.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_GameData_h
#define GameWorld_GameData_h


/**
 * A parent type that will define the basic functions and members for Damage and Health (and other similar data types
 * if we need them)
 */
struct GameData {
	
protected:
	unsigned long baseValue ;
	unsigned long modifier ;
	
public:
	GameData() :
		baseValue(0),
		modifier(0) {}
	
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
	 * @return The total amount of this GameData value (e.g. Health or Damage
	 */
	unsigned long value() {
		return baseValue + modifier ;
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
	
	Health(long b, long m) :
		GameData(b, m) {}
	
	Health(const Health & other) :
		GameData(other) {}
} ;

#endif
