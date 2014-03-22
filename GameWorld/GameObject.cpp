//
//  GameObject.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "GameObject.h"

unsigned GameObject::IDs = 0 ;

vector<string> * GameObject::icons = new vector<string>{"🎾", "🔱", "💩", "🍹", "🎅", "👿", "👮", "👹", "🚶", "👩", "💂", "💊"} ;

char * GameObject::nameLetters = new char[26] {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'} ;

vector<thread *> * GameObject::allThreads  = new vector<thread *>() ;

const double GameObject::GLOBAL_MAX_X = 500 ;
const double GameObject::GLOBAL_MAX_Y = 500;


GameObject::GameObject() :
	ID(IDs),
	icon("no icon"),
	loc(new Location())
{
	IDs++ ;
}

GameObject::GameObject(const GameObject & other) :
	wanderXYOffset(other.wanderXYOffset),
	wanderTime(other.wanderTime),
	gObjThread(nullptr),
	ID(IDs),
	icon(other.icon),
	loc(new Location(*(other.loc)))
{
	IDs++ ;
}

GameObject::GameObject(GameObject && other) :
	wanderXYOffset(other.wanderXYOffset),
	wanderTime(other.wanderTime),
	gObjThread(other.gObjThread),
	ID(other.ID),
	icon(other.icon),
	loc(other.loc)
{
	other.gObjThread = nullptr ;
	other.loc = nullptr ;
	cout << "Called GameObject move ctor" << endl ;
}

GameObject::GameObject(string symbol, Location * loc) :
	ID(IDs),
	icon(symbol)
{
	IDs++ ;
	if (loc->x > GLOBAL_MAX_X) {
		cout << "Location x coord is not within the specified limits" << endl ;
		throw new exception() ;
	}
	else if (loc->y > GLOBAL_MAX_Y) {
		cout << "Location y coord is not within the specified limits" << endl ;
		throw new exception() ;
	}
	else {
		this->loc = loc ;
	}
}

GameObject::GameObject(int randSeed) :
	ID(IDs),
	icon(string())
{
	
	IDs++ ;
    if (randSeed == 0) {
		randSeed = rand() ;
	}
	icon = icons->at(randSeed % icons->size()) ;
	//we mainly needed randSeed to tell us we're using this constructor, we'll only
	//actually use it once (see two lines above) - we want each value initialized randomly on its own
	long x = (rand() % lrint(GLOBAL_MAX_X)) ;
	
	long y = (rand() % lrint(GLOBAL_MAX_Y)) ;

	
	loc = new Location(x, y, 0) ;
}

GameObject::~GameObject() {
	//delete gObjThread ;
	delete loc ;
	cout << "called dtor" << endl ;
}

GameObject & GameObject::operator=(const GameObject & rhs) {
	if (this != &rhs) {
		this->wanderXYOffset = rhs.wanderXYOffset ;
		this->wanderTime = rhs.wanderTime ;
		this->gObjThread = nullptr ;
		this->ID = IDs ;
		IDs++ ;
		this->icon = rhs.icon ;
        this->loc = new Location(*(rhs.loc)) ;
	}
	return *this ;
}

GameObject & GameObject::operator=(GameObject && rhs) {
	if (this != &rhs) {
		this->wanderXYOffset = rhs.wanderXYOffset ;
		this->wanderTime = rhs.wanderTime ;
		this->gObjThread = rhs.gObjThread ;
		this->ID = rhs.ID ;
		this->icon = rhs.icon ;
        this->loc = rhs.loc ;
		rhs.gObjThread = nullptr ;
		rhs.loc = nullptr ;
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
		*writeTO << "Current location: " << loc->toString() << endl ;
	}
}

void GameObject::move(double xoffset, double yoffset) {
	this->loc->change(xoffset, yoffset, 0) ;
}

void GameObject::move(const Location & moveTo) {
	delete loc ;
	this->loc = new Location(moveTo) ;
}

void GameObject::wander(double xyOffset, long time) {
	this->wanderTime = time ;
	this->wanderXYOffset = xyOffset ;
	GameObject::gObjThread = new std::thread(&GameObject::wanderThreaded, *this) ;
	GameObject::allThreads->push_back(this->gObjThread) ;
}

void GameObject::wanderThreaded() {
	BasicTime timer ;
	timer.startTimer() ;
	while (timer.checkTimeElapsed() < wanderTime) {
		double randX = randSignFlip((rand() % (static_cast<int>(wanderXYOffset)))) ;
		if ((loc->x + randX) > GameObject::GLOBAL_MAX_X) {
			randX = (randX * -1) ;
		}
		//repeat for y coord
		double randY = randSignFlip((rand() % (static_cast<int>(wanderXYOffset)))) ;
		if ((loc->y + randY) > GameObject::GLOBAL_MAX_Y) {
			randY = (randY * -1) ;
		}
		move(randX, randY) ;
	}
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

void GameObject::joinThreads() {
	for (vector<thread *>::size_type i = 0 ; i < allThreads->size() ; i++) {
		(*allThreads)[i]->join() ;
		delete (*allThreads)[i] ;
	}
}


