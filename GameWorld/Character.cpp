//
//  Character.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#define eight_milliseconds 8333

#include "Character.h"

Character::Character() :
	GameObject(),
	name("noName"), reaction(Reaction::noreact),
	alive(DoA(0)),state(),
	health(),damage()
{
}

Character::Character(const Character & other) :
	GameObject(other), reaction(other.reaction),
	name(other.name), alive(other.alive),
	state(other.state), health(other.health),
	damage(other.damage)
{
}

Character::Character(Character && other) :
	GameObject(std::move(other)),
	name(std::move(other.name)), reaction(std::move(other.reaction)),
	alive(std::move(other.alive)),
	state(std::move(other.state)), health(std::move(other.health)),
	damage(std::move(other.damage))
{
}


Character::Character(AssetType type, const string & imageFilename, float sizeMod, const Pos2<float> & loc, string name, Reaction reaction, DoA alive, CharacterState state, unsigned health, unsigned damage) :
	GameObject(type, imageFilename, sizeMod, loc),
	reaction(reaction), name(name), alive(alive),
	state(state), health(new Health(health)), damage(new Damage(damage))
{
}

Character::Character(FastRand<int> rand) :
	GameObject(rand),
	reaction((Reaction)rand.nextValue(-2, 2)), 
	alive(DoA::alive),
	state(CharacterState::normal),
	health{new Health(goRand.nextValue(0, 500))},
	damage{new Damage(goRand.nextValue(0, 50))}
{
	type = AssetType::block ;
	name = generateName(goRand.nextValue(0, 11) + 5) ;
}

Character::~Character() {
	delete health ;
	delete damage ;
}

Character & Character::operator=(const Character &rhs) {
	if (this != &rhs) {
		this->GameObject::operator=(rhs) ;
		this->name = rhs.name ;
		this->reaction = rhs.reaction ;
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
		this->reaction = std::move(rhs.reaction) ;
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

void Character::textDescription(ostream * writeTo) const {
	stringstream ss ;
	ss << "Character name: " << this->name << endl ;;
	ss << "Current status: " << this->state << endl  ;
	ss << "Current health: " << this->health << endl ;
	ss << "Damage capability: " << this->damage << endl ;
	this->GameObject::textDescription(&ss) ;
	*writeTo << ss.rdbuf() ;
}

//using GameObject's implementation for now, may change
void Character::defaultBehaviors() {
	this->GameObject::defaultBehaviors() ;
}


void Character::attack(Character * enemy) {
	(*Debug::debugOutput) << "Fight! " << this->name << " attacked " << enemy->name << '\n' ;
	attack_helper(enemy) ;
	if (this->health > enemy->health) {
		(*Debug::debugOutput) << this->name << " wins!" << '\n' ;
		if (enemy->health->value() <= 0) {
			*enemy->currentlyThreading = false ;
			enemy->markedForDeletion = true ;
		}
	}
	else if (enemy->health > this->health) {
		(*Debug::debugOutput) << enemy->name << " wins!" << '\n' ;
		if (this->health->value() <= 0) {
			*this->currentlyThreading = false ;
			this->markedForDeletion = true ;
		}
	}
	else {
		(*Debug::debugOutput << "A tie!" << '\n') ;
	}
}

void Character::attack_helper(Character * enemy) {
	this->state = CharacterState::inCombat ;
	if (enemy->state != CharacterState::inCombat) {
		enemy->attack_helper(this) ;
	}
	if (this->health->value() <= 0) {
		return ;
	}
	if (enemy->health->value() <= 0) {
		return ;
	}
	*(enemy->health) = ((enemy->health->value()) - (this->damage->value())) ;
	enemy->attack_helper(this) ;
}

CharacterState * Character::getState() {
	CharacterState * st = &(this->state) ;
	return st ;
}

Health * Character::checkHealth() {
	Health * h = this->health ;
	return h ;
}


void Character::modHealth(Health * other) {
	delete health ;
	this->health = other ;
}


Damage * Character::getDamage() {
	Damage * d = this->damage ;
	return d ;
}

void Character::modDamage(Damage * other) {
	delete damage ;
	this->damage = other ;
}









