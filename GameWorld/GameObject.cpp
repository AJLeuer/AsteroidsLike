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

list<thread *> * GameObject::allThreads  = new list<thread *>() ;
list<thread *>::iterator GameObject::lastAddedThread = allThreads->begin() ;

const double GameObject::GLOBAL_MAX_X = 500 ;
const double GameObject::GLOBAL_MIN_X = 0 ;
const double GameObject::GLOBAL_MAX_Y = 500;
const double GameObject::GLOBAL_MIN_Y = 0 ;


GameObject::GameObject() :
	ID(IDs),
	icon("no icon"),
	loc(new Location())
{
	IDs++ ;
}

GameObject::GameObject(const GameObject & other) :
	gObjThread(nullptr),
	ID(IDs),
	icon(other.icon),
	loc(new Location(*(other.loc)))
{
	IDs++ ;
}

GameObject::GameObject(GameObject && other) :
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
	if (loc->getX() > GLOBAL_MAX_X) {
		cout << "Location x coord is not within the specified limits" << endl ;
		throw new exception() ;
	}
	else if (loc->getY() > GLOBAL_MAX_Y) {
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
	cout << "called GameObj dtor" << endl ;
}

GameObject & GameObject::operator=(const GameObject & rhs) {
	if (this != &rhs) {
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

list<thread *>::iterator GameObject::startThreading(std::thread * gObjThr, bool wait) {
	GameObject::allThreads->insert(lastAddedThread, this->gObjThread) ;
	auto i = lastAddedThread ;
	lastAddedThread++ ;
	if (wait) {
		gObjThread->join() ;
	}
	return i ;
}

void GameObject::endThreading(list<thread *>::iterator pos) {
	GameObject::allThreads->erase(pos) ;
	delete this->gObjThread ;
	this->gObjThread = nullptr ;
}

void GameObject::textDescription(ostream * writeTO) const {
	*writeTO << "GameObject ID#: " << this->ID << endl ;
	*writeTO << "Icon: " << this->icon << endl ;
	if (loc != nullptr) {
		*writeTO << "Current location: " << loc->toString() << endl ;
	}
}

void GameObject::move(double xoffset, double yoffset) {
	this->loc->modify(xoffset, yoffset, 0) ;
}

void GameObject::move(const Location & moveTo) {
	delete loc ;
	this->loc = new Location(moveTo) ;
}

void GameObject::wander(double xyOffset, long time) {
	auto i = startThreading(this->gObjThread, false) ;
	gObjThread = new std::thread(&GameObject::wander_threaded, std::move(this), xyOffset, time, i) ;
}

void GameObject::wander_threaded(double xyOffset, long time, list<thread *>::iterator pos) {
	BasicTime timer ;
	timer.startTimer() ;
	while (timer.checkTimeElapsed() < time) {
		double randX = randSignFlip((rand() % ((int)(xyOffset)))) ;
		if (((loc->getX() + randX) > GameObject::GLOBAL_MAX_X) || ((loc->getX() + randX) < GameObject::GLOBAL_MIN_X)) {
			randX = (randX * -1) ;
		}
		//repeat for y coord
		double randY = randSignFlip((rand() % ((int)(xyOffset)))) ;
		if (((loc->getY() + randY) > GameObject::GLOBAL_MAX_Y) || ((loc->getY() + randY) < GameObject::GLOBAL_MIN_Y)) {
			randY = (randY * -1) ;
		}
		*Debug::debugFile << "randX: " << randX << " rand Y: " << randY << endl ;
		move(randX, randY) ;
		usleep(5000) ;
	}
	endThreading(pos) ;
}


void GameObject::setIcon(const string & icon) {
	this->icon = icon ;
}

string & GameObject::getIcon() {
	return this->icon ;
}

ostream & operator<<(std::ostream & os, GameObject & gameObj) {
	gameObj.textDescription(&os) ;
	return os ;
}

string GameObject::toString() const {
	stringstream * ss = new stringstream() ;
	this->textDescription(ss) ;
	string s = string(ss->str()) ;
	delete ss ;
	return std::move(s) ;
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
	for (list<thread *>::iterator i = allThreads->begin() ; i != allThreads->end() ; i++) {
		(*i)->join() ;
		delete *i ;
	}
}


