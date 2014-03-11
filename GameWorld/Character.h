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

#include "Message.h"

using namespace std ;

class Character : public GameObject {

protected:
	
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
	
	void attack(Character & enemy)
  
	
} ;

#endif /* defined(__GameWorld__Character__) */
