//
//  NPC.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#include "NPC.h"

NPC::NPC() :
	Character() {}

NPC::NPC(const NPC & other) :
	Character(other) {}

NPC::NPC(NPC && other) :
	Character(std::move(other)) {}

NPC::NPC(AssetType type, const string & imageFilename, float size, const Pos2<float> & loc, string name, DoA alive, CharacterState state, Health * health, Damage * damage, Reaction reaction) :
	Character(type, imageFilename, size, loc, name, reaction, alive, state, health, damage) {}

NPC::NPC(FastRand<int> rand) :
	Character(rand)
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
	}
	return *this ;
}

NPC & NPC::operator=(NPC && rhs) {
	if (this != &rhs) {
		this->Character::operator=(std::move(rhs)) ;
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

void NPC::textDescription(ostream * writeTo) const {
	stringstream ss ;
	this->Character::textDescription(&ss) ;
	ss << "Reaction to player: " << this->reaction << endl ;
	*writeTo << ss.rdbuf() ;
}

void NPC::attack(Character * enemy) {
	//todo
}





