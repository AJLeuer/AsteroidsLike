//
//  NPC.cpp
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#include "NPC.h"

NPC::NPC() :
	Pawn() {}

NPC::NPC(const NPC & other) :
	Pawn(other) {}

NPC::NPC(NPC && other) :
	Pawn(std::move(other)) {}

NPC::NPC(const AssetFile & imageFile, float size, const Vect<float> & loc, const Angle rotation,
		 string name, unsigned health, unsigned damage, SafeBoolean monitorVelocity, bool boundsChecking) :

	Pawn(imageFile, size, loc, rotation, name, health, damage, monitorVelocity, boundsChecking) {}

NPC::NPC(Randm<int> rand, AssetType type) :
	Pawn(rand, type)
{
	//todo finish
}

NPC::~NPC() {
	//todo
}

void NPC::operator()() {
	//todo
}

void NPC::operator()(Pawn & other) {
	*(Debug::debugOutput) << "" << endl ;//todo
}

void NPC::textDescription(ostream * writeTo) const {
	stringstream ss ;
	this->Pawn::textDescription(&ss) ;
	*writeTo << ss.rdbuf() ;
}

void NPC::attack(GameObject * enemy) {
	//todo
}





