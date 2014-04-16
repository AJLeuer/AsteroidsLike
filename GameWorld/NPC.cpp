//
//  NPC.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#include "NPC.h"

NPC::NPC() :
	Character(),
	reaction(Reaction::neutral)
{
}

NPC::NPC(const NPC & other) :
	Character(other),
	reaction(other.reaction)
{
}

NPC::NPC(NPC && other) :
	Character(std::move(other)),
	reaction(std::move(other.reaction))
{
}

NPC::NPC(string symbol, Location<long> * loc, string name, DoA alive, CharacterState state, Health health, Damage damage, Reaction reaction) :
	Character(symbol, loc, name, alive, state, health, damage),
	reaction(reaction)
{
}

NPC::NPC(int randSeed) :
	Character(randSeed)
{
	this->reaction = Reaction(rand() % 5) ;
	//todo finish
}

NPC::~NPC() {
	//todo
}


NPC & NPC::operator=(const NPC & rhs) {
	if (this != &rhs) {
		this->Character::operator=(rhs) ;
		this->reaction = rhs.reaction ;
	}
	return *this ;
}

NPC & NPC::operator=(NPC && rhs) {
	if (this != &rhs) {
		this->Character::operator=(std::move(rhs)) ;
		this->reaction = std::move(rhs.reaction) ;
	}
	return *this ;
}


void NPC::operator()() {
	//todo
}

void NPC::operator()(Character & other) {
	*(Debug::debugOutput) << "" << endl ;//todo
}

void NPC::notify() {
	//todo
}

void NPC::passMessage(Message * message, Character & recipient) {
	//todo
}

void NPC::textDescription(ostream * writeTO) const {
	this->Character::textDescription(writeTO) ;
	*writeTO << "Reaction to player: " << this->reaction << endl ;
}

void NPC::attack(Character & enemy) {
	//todo
}





