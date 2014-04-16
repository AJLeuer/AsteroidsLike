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
	name("noName"), alive(DoA(0)),
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

Character::Character(Character && other) :
	GameObject(std::move(other)),
	name(std::move(other.name)), alive(std::move(other.alive)),
	state(std::move(other.state)), health(std::move(other.health)),
	damage(std::move(other.damage))
{
}


Character::Character(string symbol, Location<long> * loc, string name, DoA alive, CharacterState state, Health health, Damage damage) :
	GameObject(symbol, loc),
	name(name), alive(alive),
	state(state), health(health), damage(damage)
{
}

Character::Character(int randSeed) :
	GameObject(randSeed),
	alive((DoA)(fastRand<unsigned long>::nextValue() % 2)),
	state((CharacterState) (fastRand<unsigned long>::nextValue() % 4)),
	health(fastRand<unsigned long>::nextValue() % 500),
	damage(fastRand<unsigned long>::nextValue() % 50)
{
	name = GameObject::generateName((fastRand<unsigned long>::nextValue() % 12) + 5) ;
}

Character::~Character() {
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

Character & Character::operator=(Character && rhs) {
	if (this != &rhs) {
		this->GameObject::operator=(std::move(rhs)) ;
		this->name = std::move(rhs.name) ;
		this->alive = std::move(rhs.alive) ;
		this->state = std::move(rhs.state) ;
		this->health = std::move(rhs.health) ;
		this->damage = std::move(rhs.damage) ;
	}
	return *this ;
}

void Character::operator()() {
	//todo
}

void Character::operator()(GameObject & other) {
	//todo
}

void Character::notify() {
	//todo implement
}

void Character::passMessage(Message *message, GameObject &recipient) {
	//todo
}

void Character::textDescription(ostream * writeTO) const {
	*writeTO << "Character name: " << this->name << endl ;;
	*writeTO << "Current status: " << this->state << endl  ;
	*writeTO << "Current health: " << this->health << endl ;
	*writeTO << "Damage capability: " << this->damage << endl ;
	this->GameObject::textDescription(writeTO) ;
}

void Character::attack(Character & enemy) {
	//todo
}

CharacterState * Character::getState() {
	CharacterState * st = &(this->state) ;
	return st ;
}

Health * Character::checkHealth() {
	Health * h = &(this->health) ;
	return h ;
}


void Character::modHealth(const Health & other) {
	this->health = other ;
}


Damage * Character::getDamage() {
	Damage * d = &(this->damage) ;
	return d ;
}

void Character::modDamage(const Damage & other) {
	this->damage = other ;
}









