//
//  Character.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Character.h"

Character::Character() :
	name("noName"), alive(false),
	activity(), health(), damage()
{
	
}

Character::Character(string name, bool alive, CurrentActivity activity, Health health, Damage damage) :
	name(name), alive(alive),
	activity(activity), health(health), damage(damage)
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
	return d ;
}

Damage * Character::modDamage() {
	return &(this->damage) ;
}

void attack(Character & enemy) {
	//todo implement
}

stringstream * draw() {
	stringstream * ss = new stringstream() ;
	*ss << "♿️" ;
	return ss ;
	
}






