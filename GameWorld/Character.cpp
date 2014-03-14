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
	state(), health(), damage()
{
}

Character::Character(const Character & other) :
	GameObject(other),
	name(other.name), alive(other.alive),
	state(other.state), health(other.health),
	damage(other.damage)
{
}

Character::Character(string name, bool alive, CharacterState state, Health health, Damage damage) :
	GameObject(),
	name(name), alive(alive),
	state(state), health(health), damage(damage)
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

Character::~Character() {
	//todo
}

Character & Character::operator=(const Character &rhs) {
	if (this != &rhs) {
		this->GameObject::operator=(rhs) ;
		this->name = rhs.name ;
		this->alive = rhs.alive ;
		this->state = rhs.state ;
		this->health = rhs.health ;
		this->damage = rhs.damage ;
	}
	return *this ;
}


void Character::notify() {
	//todo implement
}

CharacterState * Character::getState() {
	CharacterState * st = &(this->state) ;
	return st ;
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






