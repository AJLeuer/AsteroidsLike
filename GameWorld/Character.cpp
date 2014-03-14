//
//  Character.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Character.h"

Character::Character() :
	GameObject(),
	name("noName"), alive(false),
	activity(), health(), damage()
{
}

Character::Character(const Character & other) :
	GameObject(other),
	name(other.name), alive(other.alive),
	activity(other.activity), health(other.health),
	damage(other.damage)
{
}

Character::Character(string name, bool alive, CurrentActivity activity, Health health, Damage damage) :
	GameObject(),
	name(name), alive(alive),
	activity(activity), health(health), damage(damage)
{
	;
}

Character::Character(int randSeed) :
	GameObject()
{
	if (randSeed == 0) {
		randSeed = rand() ;
	}
	//todo
}

Character::Character



const CurrentActivity * Character::currentActivity() {
	CurrentActivity * act = &(this->activity) ;
	return act ;
}

const Health * Character::checkHealth() {
	const Health * h = &(this->health) ;
	return h ;
}


void Character::modHealth(const Health & other) {
	this->health = other ;
}


const Damage * Character::getDamage() {
	const Damage * d = &(this->damage) ;
	return d ;
}

void Character::modDamage(const Damage & other) {
	this->damage = other ;
}

void attack(Character & enemy) {
	//todo implement
}

stringstream * draw() {
	stringstream * ss = new stringstream() ;
	*ss << "♿️" ;
	return ss ;
	
}






