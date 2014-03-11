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

#include "GameObject.h"
#include "Message.h"

using namespace std ;

class Character : public GameObject {

protected:
	
	string name ;
	
	/**
	 * The character's current status
	 */
	Status status ;
	
	/**
	 * The character's current health
	 */
	unsigned int health ;
	
	/**
	 * The base amount of damage done by this character
	 */
	unsigned int baseDamage ;

public:
	Character() ;
	Character(string name, Status status, unsigned health, unsigned baseDamage) ;
	
	/**
	 * Returns a pointer to this Character's health, allowing it to be changed.
	 */
	unsigned int* getHealth() ;
	void setHealth(unsigned int val) ;
	
	/**
	 * Returns a read-only value representing Character's health.
	 */
	const unsigned int checkHealth() ;
	
	
	unsigned int getDamage() ;
	void setDamage(unsigned int val) ;
	
	void attack(Character & enemy) ;
	
	/**
	 * Draws a representation of this Character
	 */
	ostream draw() ;
  
	
} ;

#endif /* defined(__GameWorld__Character__) */
