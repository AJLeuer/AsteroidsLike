//
//  Character.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Character__
#define __GameWorld__Character__

#include <iostream>
#include <sstream>

#include "GameData.h"
#include "Message.h"
#include "GameObject.h"

using namespace std ;

class Character : public GameObject {

protected:
	
	string name ;
	
	bool alive ;
	
	/**
	 * The Character's CurrentActivity
	 */
	CurrentActivity activity ;
	
	/**
	 * The Character's current Health (a data structure inheriting from GameData)
	 */
	Health health ;
	
	/**
	 * The Character's current Damage (a data structure inheriting from GameData)
	 */
	Damage damage ;

public:
	/**
	 * Constructs a default Character
	 */
	Character() ;
	
	/**
	 * Copy constructor for Character
	 */
	Character(const Character &) ;
	
	/**
	 * Constructs a Character based on the arguments given
	 *
	 * @param name The name of this Character
	 * @param alive Whether this Character is dead or alive
	 * @param currentActivity The CurrentActivity of this Character
	 * @param health The Health of this Character
	 * @param damage The Damage capability of this Character
	 */
	Character(string name, bool alive, CurrentActivity currentActivity, Health health, Damage damage) ;
	
	/**
	 * Constructs a randomized Character. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param randSeed A seed to initialize the random number generator
	 */
	Character(int randSeed) ;
	
	/**
	 * Assignment operator overload
	 */
	Character operator=(const Character & rhs) ;
	
	
	/**
	 * Another class with a reference to this Character can call this to have the Character perform some
	 * function, as yet undecided. TBI.
	 */
	virtual void notify() ;
	
	
	/**
	 * @return This character's current activity
	 */
	const CurrentActivity * currentActivity() ;
	
	/**
	 * @return a value representing Character's health
	 */
	const Health * checkHealth() ;
	
	/**
	 * Sets this Character's health
	 */
	void modHealth(const Health &) ;
	
	/**
	 * @return a read-only value representing Character's damage
	 */
	const Damage * getDamage() ;

	/**
	 * Sets this Character's damage
	 */
	void modDamage(const Damage &) ;
	
	
	virtual void attack(Character & enemy) ;
	
	/**
	 * Draws a representation of this Character
	 */
	virtual stringstream * draw() ;
  
	
} ;

#endif /* defined(__GameWorld__Character__) */
