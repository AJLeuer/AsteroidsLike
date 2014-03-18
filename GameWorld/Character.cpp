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

Character::Character(string symbol, Location * loc, string name, bool alive, CharacterState state, Health health, Damage damage) :
	GameObject(symbol, loc),
	name(name), alive(alive),
	state(state), health(health), damage(damage)
{
}

Character::Character(int randSeed) :
	GameObject(randSeed),
	alive(rand() % 2),
	state((CharacterState) (rand() % 4)),
	health(rand() % 500),
	damage(rand() % 50)
{
	name = (char)(rand() % 255) + (char)(rand() % 255) + (char)(rand() % 255) + (char)(rand() % 255)+ (char)(rand() % 255) ;
}

Character::~Character() {
    GameObject::~GameObject() ;
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

void Character::textDescription(ostream * writeTO) {
	*writeTO << "Character name: " << this->name << endl ;
	string curStatus = "" ;
	if (alive) curStatus = "Alive" ;
	else /*if (alive==false)*/ curStatus = "Deceased" ;
	*writeTO << "Current status: " << curStatus << endl  ;
	*writeTO << "State: " << this->getState() << endl ;
	*writeTO << "Current health: " << this->checkHealth()->value() << endl ;
	*writeTO << "Damage capability: " << this->getDamage()->value() << endl ;
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









