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
#include "GameObject.h"
#include "Message.h"

using namespace std ;

class Character : public GameObject {

protected:
	
	string name ;
	
	bool alive ;
	
	/**
	 * The character's CurrentActivity
	 */
	CurrentActivity activity ;
	
	/**
	 * The character's current health
	 */
	Health health ;
	
	Damage damage ;

public:
	Character() ;
	Character(string name, bool alive, CurrentActivity currentActivity, Health health, Damage damage) ;
	
	/**
	 * @return This character's current activity
	 */
	CurrentActivity * currentActivity() ;
	
	/**
	 * @return a value representing Character's health
	 */
	const Health checkHealth() ;
	
	/**
	 * @return a pointer to this characters health,
	 * allowing it to be changed
	 */
	Health * modHealth(unsigned long val) ;
	
	/**
	 * @return a read-only value representing Character's damage
	 */
	const Damage getDamage() ;

	/**
	 * @return A pointer to the amount of damage done by this character,
	 * which may be changed
	 */
	Damage * modDamage() ;
	
	
	void attack(Character & enemy) ;
	
	/**
	 * Draws a representation of this Character
	 */
	stringstream * draw() ;
  
	
} ;

#endif /* defined(__GameWorld__Character__) */
