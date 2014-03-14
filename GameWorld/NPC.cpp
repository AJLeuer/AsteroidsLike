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

NPC::NPC(string symbol, string name, bool alive, CharacterState state, Health health, Damage damage, Reaction reaction) :
	Character(symbol, name, alive, state, health, damage),
	reaction(reaction)
{
}

NPC::NPC(int randSeed) :
	Character(randSeed)
{
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


void NPC::operator()() {
	//todo
}

void NPC::operator()(Character & other) {
	cout << "" << endl ;//todo
}

void NPC::notify() {
	//todo
}

void NPC::passMessage(Message * message, Character & recipient) {
	//todo
}


void NPC::attack(Character & enemy) {
	cout << "" << endl ;
	//todo
}





