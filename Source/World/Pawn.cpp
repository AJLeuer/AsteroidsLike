//
//  Pawn.cpp
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#include "Pawn.h"

Pawn::Pawn() :
	GameObject(),
	name("noName"),
	health(), damage()
{
}

Pawn::Pawn(const Pawn & other) :
	GameObject(other),
	name(other.name),
	health(other.health),
	damage(other.damage)
{
}

Pawn::Pawn(Pawn && other) :
	GameObject(std::move(other)),
	name(std::move(other.name)),
	damage(other.damage)
{
	other.health = nullptr ;
	other.damage = nullptr ;
}


Pawn::Pawn(const AssetFile & imageFile, float sizeModifier, const Vect<float> & loc, const Angle rotation, string name, unsigned health, unsigned damage, SafeBoolean monitorVelocity, bool boundsChecking) :

	GameObject(imageFile, sizeModifier, loc, rotation, true, monitorVelocity, (SafeBoolean)boundsChecking, SafeBoolean::t),
	name(name), health(new Health(health)),
	damage(new Damage(damage))
{
}

Pawn::Pawn(Randm<int> rand, AssetType type) :
	GameObject(rand, type, true),
	health(new Health(goRand.nextValue(0, 500))),
	damage(new Damage(goRand.nextValue(0, 50)))
{
	name = generateName(goRand.nextValue(0, 11) + 5) ;
}

Pawn::~Pawn() {
	if (health != nullptr) {
		delete health ;
	}
	if (damage != nullptr) {
		delete damage ;
	}
}


void Pawn::operator()() {
	//todo
}

void Pawn::operator()(GameObject * other) {
	//todo
}


void Pawn::textDescription(ostream * writeTo) const {
	stringstream ss ;
	ss << "Pawn name: " << this->name << endl ;;
	ss << "Current health: " << this->health << endl ;
	ss << "Damage capability: " << this->damage << endl ;
	this->GameObject::textDescription(&ss) ;
	*writeTo << ss.rdbuf() ;
}

//using GameObject's implementation for now, may change
void Pawn::doDefaultBehavior(bool initialCall) {
	aiBehaviors() ;
}

void Pawn::aiBehaviors() {
	
}


void Pawn::attack(GameObject * enemy) {
    //TODO
}

void Pawn::attack_helper(Pawn * enemy) {

}


Health * Pawn::checkHealth() {
	Health * h = this->health ;
	return h ;
}


void Pawn::modHealth(Health * other) {
	delete health ;
	this->health = other ;
}


Damage * Pawn::getDamage() {
	Damage * d = this->damage ;
	return d ;
}

void Pawn::modDamage(Damage * other) {
	delete damage ;
	this->damage = other ;
}









