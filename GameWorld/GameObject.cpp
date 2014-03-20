//
//  GameObject.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "GameObject.h"

unsigned GameObject::IDs = 0 ;

vector<string> GameObject::icons = {"🎾", "🔱", "💩", "🍹", "🎅", "👿", "👮", "👹", "🚶", "👩", "💂", "💊"} ;

char GameObject::nameLetters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'} ;

//World will set these
Location * GameObject::GLOBAL_MAX_LOCATION = nullptr ;
Location * GameObject::GLOBAL_MIN_LOCATION = nullptr ;

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
	icon = icons.at(randSeed % icons.size()) ;
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

void GameObject::move(int xoffset, int yoffset) {
	this->loc->x += xoffset ;
	this->loc->y += yoffset ;
}

void GameObject::move(const Location moveTo) {
	
	*(this->loc) = moveTo ;
}

void GameObject::setIcon(const string & icon) {
	this->icon = icon ;
}

string GameObject::getIcon() {
	return this->icon ;
}

ostream & operator<<(std::ostream & os, GameObject & gameObj) {
	gameObj.textDescription(&os) ;
	return os ;
}

const string * GameObject::textDescription() {
	stringstream * ss = new stringstream() ;
	this->textDescription(ss) ;
	const string * s =  new string(ss->str()) ;
	delete ss ;
	return s ;
}

const string * GameObject::toString() {
	return this->textDescription() ;
}

const string GameObject::generateName(unsigned int length) {
	string s = "" ;
	s += std::toupper(GameObject::nameLetters[(rand() % 27)]) ;
	for (unsigned i = 0 ; i < length ; i++) {
		s += nameLetters[(rand() % 27)] ;
	}
	return s ;
}


