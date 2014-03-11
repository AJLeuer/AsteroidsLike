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

CurrentActivity * Character::currentActivity() {
	CurrentActivity * act = &(this->activity) ;
	return act ;
}

const Health Character::checkHealth() {
	const Health h = this->health ;
	return h ;
}


Health * Character::modHealth(unsigned long val) {
	return &(this->health) ;
}


const Damage Character::getDamage() {
	const Damage d = this->damage ;
}


Damage * Character::modDamage() ;








