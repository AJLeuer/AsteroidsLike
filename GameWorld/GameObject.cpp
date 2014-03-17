//
//  GameObject.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "GameObject.h"

unsigned GameObject::IDs = 0 ;

GameObject::GameObject() :
	ID(IDs)
{
	IDs++ ;
    loc = nullptr ;
}

GameObject::GameObject(const GameObject & other) :
	ID(IDs)
{
	IDs++ ;
	this->icon = other.icon ;
    this->loc = new Location(*(other.loc)) ;
}

GameObject::GameObject(string symbol, Location * loc) :
	ID(IDs), icon(symbol), loc(loc)
{
	IDs++ ;
}

GameObject::GameObject(int randSeed)
{
    if (randSeed == 0) {
		randSeed = rand() ;
	}
	//todo
}

GameObject::~GameObject() {
	delete loc ;
}

GameObject & GameObject::operator=(const GameObject & rhs) {
	if (this != &rhs) {
		this->ID = IDs ;
		IDs++ ;
		this->icon = rhs.icon ;
        this->loc = new Location(*(rhs.loc)) ;
	}
	return *this ;
}

void GameObject::operator()() {
	//todo
}

void GameObject::operator()(GameObject &sentObject) {
	//todo
}

void GameObject::notify() {
	//todo
}

void GameObject::passMessage(Message * message, GameObject &recipient) {
	//todo
}


string GameObject::draw() {
	return this->icon ;
}
