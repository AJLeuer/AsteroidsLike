//
//  Character.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Character.h"

Character::Character() :
	name("noName"), status(Status(-1)),
	health(0), baseDamage(0)
{
	
}

Character::Character(string name, Status status, unsigned health, unsigned baseDamage) :
	name(name), status(status),
	health(health), baseDamage(baseDamage)
{
	
}

unsigned * Character::getHealth() {
	unsigned * h = &(this->health) ;
	return h ;
}

const unsigned Character::checkHealth() {
	const unsigned h = this->health ;
	return h ;
}

void Character::setHealth() {
	this->health;
}

unsigned Character::getDamage()
void Character::setDamage()