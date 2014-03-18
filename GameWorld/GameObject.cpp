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
	ID(IDs),
	icon("NULL"),
	loc(nullptr)
{
	IDs++ ;
}

GameObject::GameObject(const GameObject & other) :
	ID(IDs),
	icon(other.icon),
	loc(new Location(*(other.loc)))
{
	IDs++ ;
}

GameObject::GameObject(string symbol, Location * loc) :
	ID(IDs),
	icon(symbol),
	loc(loc)
{
	IDs++ ;
}

GameObject::GameObject(int randSeed) :
	ID(IDs),
	icon(string()),
	loc(new Location((rand() % 1000), (rand() % 1000), (rand() % 1000)))
{
	IDs++ ;
    if (randSeed == 0) {
		randSeed = rand() ;
	}
	icon.insert(0, 1, (char)(randSeed % 0xFFFFFF)) ;
	//we mainly needed randSeed to tell us we're using this constructor, we'll only
	//actually use it once (see two lines above) - we want each value initialized randomly on its own
}

GameObject::~GameObject() {
	//delete loc ;
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

void GameObject::textDescription(ostream * writeTO) {
	*writeTO << "GameObject ID#: " << this->ID << endl ;
	*writeTO << "Icon: " << this->icon << endl ;
	if (loc != nullptr) {
		*writeTO << loc->toString() << endl ;
	}
}

void GameObject::setIcon(string icon) {
	this->icon = icon ;
}

string GameObject::getIcon() {
	return draw() ;
}

string GameObject::draw() {
	return this->icon ;
}
