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

vector<GameObject*> * GameObject::allGameObjects = nullptr ;
GameMap<GameObject> * GameObject::map = nullptr ;

const double GameObject::GLOBAL_MAX_X = 150 ;
const double GameObject::GLOBAL_MIN_X = 0 ;
const double GameObject::GLOBAL_MAX_Y = 30 ;
const double GameObject::GLOBAL_MIN_Y = 0 ;


GameObject::GameObject() :
	ID(IDs),
	icon("no icon"),
	loc(new Location<long>())
{
	IDs++ ;
	map->place((*this->loc), this) ;
}

GameObject::GameObject(const GameObject & other) :
	gObjThread(nullptr),
	ID(IDs),
	icon(other.icon),
	loc(new Location<long>(*(other.loc)))
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
	map->erase(*(other.getLocation())) ;
	map->place(*(this->loc), this) ;
}

GameObject::GameObject(string symbol, Location<long> * loc) :
	ID(IDs),
	icon(symbol)
{
	IDs++ ;
	if (loc->getX() > GLOBAL_MAX_X) {
		cout << "Location<long> x coord is not within the specified limits" << endl ;
		throw new exception() ;
	}
	else if (loc->getY() > GLOBAL_MAX_Y) {
		cout << "Location<long> y coord is not within the specified limits" << endl ;
		throw new exception() ;
	}
	else {
		this->loc = loc ;
	}
	map->place((*this->loc), this) ;
}

GameObject::GameObject(int randSeed) :
	ID(IDs),
	icon(string())
{
	
	IDs++ ;
	
	randSeed = fastRand<unsigned long>::nextValue() ; //we'll get our own
	
	long x = (fastRand<unsigned long>::nextValue() % lrint(GLOBAL_MAX_X)) ;
	long y = (fastRand<unsigned long>::nextValue() % lrint(GLOBAL_MAX_Y)) ;
	loc = new Location<long>(x, y, 0) ;
	map->place((*this->loc), this) ;
	
	icon = icons->at(randSeed % icons->size()) ;

}

GameObject::~GameObject() {
	//delete gObjThread ;
	delete loc ;
	
	//cout << "called GameObj dtor" << endl ;
}

GameObject & GameObject::operator=(const GameObject & rhs) {
	if (this != &rhs) {
		this->gObjThread = nullptr ;
		this->ID = IDs ;
		IDs++ ;
		this->icon = rhs.icon ;
        this->loc = new Location<long>(*(rhs.loc)) ;
	}
	return *this ;
}

GameObject & GameObject::operator=(GameObject && rhs) {
	if (this != &rhs) {
		this->gObjThread = rhs.gObjThread ;
		this->ID = rhs.ID ;
		this->icon = rhs.icon ;
        this->loc = rhs.loc ;
		map->erase(*(rhs.getLocation())) ;
		map->place(*(this->loc), this) ;
		rhs.gObjThread = nullptr ;
		rhs.loc = nullptr ;
	}
	return *this ;
}


void GameObject::operator()() {
	//todo
}

void GameObject::operator()(GameObject &sentObject) {

}

bool GameObject::operator==(GameObject &other) {
	if (this->ID == other.ID) {
		return true ;
	}
	else {
		return false ;
	}
}

void GameObject::notify() {
	//todo
}

void GameObject::passMessage(Message * message, GameObject & recipient) {
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

void GameObject::endThreading(list<thread *>::iterator pos, bool wait) {
	//GameObject::allThreads->erase(pos) ;
	if (wait) {
		gObjThread->join() ;
	}
	delete this->gObjThread ;
	this->gObjThread = nullptr ;
}

void GameObject::joinThreads() {
	
	for (list<thread *>::iterator i = allThreads->begin() ; i != allThreads->end() ; i++) {
		(*i)->join() ;
		delete *i ;
		*i = nullptr ;
	}
	
}

void GameObject::joinThread() {
	this->gObjThread->join() ;
	delete gObjThread ;
	gObjThread = nullptr ;
}

void GameObject::textDescription(ostream * writeTO) const {
	*writeTO << "GameObject ID#: " << this->ID << endl ;
	*writeTO << "Icon: " << this->icon << endl ;
	if (loc != nullptr) {
		*writeTO << "Current Location: " << loc->toString() << endl ;
	}
}

void GameObject::move(long xoffset, long yoffset) {
	if ((loc->x + xoffset) >= GLOBAL_MAX_X) {
		xoffset -= ((loc->x + xoffset) - GLOBAL_MAX_X + 1) ;
	}
	else if ((loc->x + xoffset) < 0) {
		xoffset -= (loc->x + xoffset) ;
	}
	if ((loc->y + yoffset) >= GLOBAL_MAX_Y) {
		yoffset -= ((loc->y + yoffset) - GLOBAL_MAX_Y + 1) ;
	}
	else if ((loc->y + yoffset) < 0) {
		yoffset -= (loc->y + yoffset) ;
	}
	map->erase(*(this->getLocation())) ;
	this->loc->modify(xoffset, yoffset, 0) ;
	map->place(*(this->loc), this) ;
}

void GameObject::move(const Location<long> & moveTo) {
	Location<long> mt = Location<long>(moveTo) ;
	if (mt.x >= GLOBAL_MAX_X) {
		mt.x = GLOBAL_MAX_X - 1 ;
	}
	else if (mt.x < 0) {
		mt.x = 0 ;
	}
	if (mt.y >= GLOBAL_MAX_Y) {
		mt.y = GLOBAL_MAX_Y - 1 ;
	}
	else if (mt.y < 0) {
		mt.y = 0 ;
	}
	map->erase(*(this->getLocation())) ;
	delete this->loc ;
	this->loc = new Location<long>(mt) ;
	map->place(*(this->loc), this) ;
}

void GameObject::wander(double xyOffset, long time) {
	void (GameObject::*wanderThrPtr)(double, long, list<thread *>::iterator) = &GameObject::wander_threaded ;
	gObjThread = new std::thread(wanderThrPtr, std::move(this), xyOffset, time, lastAddedThread) ;
	startThreading(this->gObjThread, false) ;
}

void GameObject::wander(double xyOffset, bool * run) {
	void (GameObject::*wanderThrPtr)(double, bool *, list<thread *>::iterator) = &GameObject::wander_threaded ;
	gObjThread = new std::thread(wanderThrPtr, std::move(this), xyOffset, run, lastAddedThread) ;
	startThreading(this->gObjThread, false) ;
}


void GameObject::wander_threaded(double xyOffset, long time, list<thread *>::iterator pos) {
	BasicTime timer ;
	timer.startTimer() ;
	while (timer.checkTimeElapsed() < time) {
		double nX = randSignFlip(xyOffset) ;
		if (((loc->getX() + nX) > GameObject::GLOBAL_MAX_X) || ((loc->getX() + nX) < GameObject::GLOBAL_MIN_X)) {
			nX = (nX * -1) ;
		}
		//repeat for y coord
		double nY = randSignFlip(xyOffset) ;
		if (((loc->getY() + nY) > GameObject::GLOBAL_MAX_Y) || ((loc->getY() + nY) < GameObject::GLOBAL_MIN_Y)) {
			nY = (nY * -1) ;
		}
		//*Debug::debugFile << "nX: " << nX << " rand Y: " << nY << endl ;
		move(nX, nY) ;
		usleep(5000) ;
	}
}

void GameObject::wander_threaded(double xyOffset, bool * run, list<thread *>::iterator pos) {
	while (*run) {
		double nX = randSignFlip(xyOffset) ;
		if (((loc->getX() + nX) > GameObject::GLOBAL_MAX_X) || ((loc->getX() + nX) < GameObject::GLOBAL_MIN_X)) {
			nX = (nX * -1) ;
		}
		//repeat for y coord
		double nY = randSignFlip(xyOffset) ;
		if (((loc->getY() + nY) > GameObject::GLOBAL_MAX_Y) || ((loc->getY() + nY) < GameObject::GLOBAL_MIN_Y)) {
			nY = (nY * -1) ;
		}
		//*Debug::debugFile << "nX: " << nX << " rand Y: " << nY << endl ;
		move(nX, nY) ;
		usleep(5000) ;
	}
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
	s += std::toupper(GameObject::nameLetters[(fastRand<unsigned long>::nextValue() % 27)]) ;
	for (unsigned i = 0 ; i < length ; i++) {
		s += nameLetters[(fastRand<unsigned long>::nextValue() % 27)] ;
	}
	return s ;
}




