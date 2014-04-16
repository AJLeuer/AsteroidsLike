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

bool GameObject::map_is_init = false ;

list<thread *> * GameObject::allThreads  = new list<thread *>() ;
list<thread *>::iterator GameObject::lastAddedThread = allThreads->begin() ;

list<GameObject *> * GameObject::allGameObjects = new list<GameObject*>() ;
list<GameObject *>::iterator GameObject::next_goIterator = (allGameObjects->begin()) ;

GameMap<GameObject> * GameObject::map = new GameMap<GameObject>(MAX_X, MAX_Y) ;

const long GameObject::MAX_X = GLOBAL_MAX_X ;
const long GameObject::MIN_X = GLOBAL_MIN_X ;
const long GameObject::MAX_Y = GLOBAL_MAX_Y ;
const long GameObject::MIN_Y = GLOBAL_MIN_Y ;


GameObject::GameObject() :
	ID(IDs),
	icon("no icon"),
	loc(new Location<long>(0, 0, 0)),
	goIterator(next_goIterator)
{
	IDs++ ;
	//next_goIterator++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X, MAX_Y) ;
		map_is_init = true ;
	}
	
	next_goIterator = allGameObjects->insert(this->goIterator, this) ;
	this->loc = map->place(this->loc, this) ;
}

GameObject::GameObject(const GameObject & other) :
	goThread(nullptr),
	ID(IDs),
	icon(other.icon),
	loc(new Location<long>(*(other.loc))),
	goIterator(next_goIterator)
{
	IDs++ ;
	//next_goIterator++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X, MAX_Y) ;
		map_is_init = true ;
	}
	
	next_goIterator = allGameObjects->insert(this->goIterator, this) ;
	this->loc = map->place(this->loc, this) ;
}

GameObject::GameObject(GameObject && other) :
	goThread(other.goThread),
	ID(other.ID),
	icon(std::move(other.icon)),
	loc(other.loc),
	goIterator(next_goIterator)
{
	//next_goIterator++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X, MAX_Y) ;
		map_is_init = true ;
	}
	
	allGameObjects->erase(other.goIterator) ;
	map->erase(*(other.getLocation())) ;
	
	other.goThread = nullptr ;
	other.loc = nullptr ;
	
	next_goIterator = allGameObjects->insert(this->goIterator, this) ;
	this->loc = map->place(this->loc, this) ;
}

GameObject::GameObject(string symbol, Location<long> * loc) :
	goThread(nullptr),
	ID(IDs),
	icon(symbol),
	loc(loc),
	goIterator(next_goIterator)
{
	IDs++ ;
	//next_goIterator++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X, MAX_Y) ;
		map_is_init = true ;
	}
	
	next_goIterator = allGameObjects->insert(this->goIterator, this) ;
	this->loc = map->place(this->loc, this) ;
}

GameObject::GameObject(int randSeed) :
	goThread(nullptr),
	ID(IDs),
	icon(""),
	loc(nullptr),
	goIterator(next_goIterator)
{
	IDs++ ;
	//next_goIterator++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X, MAX_Y) ;
		map_is_init = true ;
	}
	
	randSeed = fastRand<int>::nextValue() ; //we'll get our own
	
	icon = icons->at(randSeed % icons->size()) ;
	
	long x = (fastRand<unsigned long>::nextValue() % lrint(MAX_X)) ;
	long y = (fastRand<unsigned long>::nextValue() % lrint(MAX_Y)) ;
	
	loc = new Location<long>(x, y, 0) ;
	
	next_goIterator = allGameObjects->insert(this->goIterator, this) ;
	this->loc = map->place(this->loc, this) ;
}

GameObject::~GameObject() {
	next_goIterator = allGameObjects->erase(goIterator) ;
	map->erase(*(getLocation())) ;
	
	
	//delete goThread ;
	delete loc ;
}

GameObject & GameObject::operator=(const GameObject & rhs) {
	if (this != &rhs) {
		this->goThread = nullptr ;
		this->ID = IDs ;
		this->icon = rhs.icon ;
        this->loc = new Location<long>(*(rhs.loc)) ;
		this->goIterator = next_goIterator ;
		
		IDs++ ;
		//next_goIterator++ ;
		
		next_goIterator = allGameObjects->insert(this->goIterator, this) ;
		this->loc = map->place(this->loc, this) ;
	}
	return *this ;
}

GameObject & GameObject::operator=(GameObject && rhs) {
	if (this != &rhs) {
		this->goThread = rhs.goThread ;
		this->ID = rhs.ID ;
		this->icon = std::move(rhs.icon) ;
        this->loc = rhs.loc ;
		this->goIterator = next_goIterator ;
		
		//next_goIterator++ ;
		
		allGameObjects->erase(rhs.goIterator) ;
		map->erase(*(rhs.getLocation())) ;
		
		rhs.goThread = nullptr ;
		rhs.loc = nullptr ;
		
		next_goIterator = allGameObjects->insert(this->goIterator, this) ;
		this->loc = map->place(this->loc, this) ;
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

list<thread *>::iterator GameObject::startThreading(std::thread * goThr, bool wait) {
	
	GameObject::allThreads->insert(lastAddedThread, this->goThread) ;
	auto i = lastAddedThread ;
	lastAddedThread++ ;
	if (wait) {
		goThread->join() ;
	}
	return i ;
}

void GameObject::endThreading(list<thread *>::iterator pos, bool wait) {
	//GameObject::allThreads->erase(pos) ;
	if (wait) {
		goThread->join() ;
	}
	delete this->goThread ;
	this->goThread = nullptr ;
}

void GameObject::joinThreads() {
	
	for (list<thread *>::iterator i = allThreads->begin() ; i != allThreads->end() ; i++) {
		(*i)->join() ;
		delete *i ;
		*i = nullptr ;
	}
	
}

void GameObject::joinThread() {
	this->goThread->join() ;
	delete goThread ;
	goThread = nullptr ;
}

void GameObject::textDescription(ostream * writeTO) const {
	*writeTO << "GameObject ID#: " << this->ID << endl ;
	*writeTO << "Icon: " << this->icon << endl ;
	if (loc != nullptr) {
		*writeTO << "Current Location: " << loc->toString() << endl ;
	}
}

void GameObject::move(long xoffset, long yoffset) {
	if ((loc->x + xoffset) >= MAX_X) {
		xoffset -= ((loc->x + xoffset) - MAX_X + 1) ;
	}
	else if ((loc->x + xoffset) < 0) {
		xoffset -= (loc->x + xoffset) ;
	}
	if ((loc->y + yoffset) >= MAX_Y) {
		yoffset -= ((loc->y + yoffset) - MAX_Y + 1) ;
	}
	else if ((loc->y + yoffset) < 0) {
		yoffset -= (loc->y + yoffset) ;
	}
	map->erase(*(this->getLocation())) ;
	this->loc->modify(xoffset, yoffset, 0) ;
	this->loc = map->place(this->loc, this) ;
}

void GameObject::move(const Location<long> & moveTo) {
	Location<long> mt = Location<long>(moveTo) ;
	if (mt.x >= MAX_X) {
		mt.x = MAX_X - 1 ;
	}
	else if (mt.x < 0) {
		mt.x = 0 ;
	}
	if (mt.y >= MAX_Y) {
		mt.y = MAX_Y - 1 ;
	}
	else if (mt.y < 0) {
		mt.y = 0 ;
	}
	map->erase(*(this->getLocation())) ;
	delete this->loc ;
	this->loc = new Location<long>(mt) ;
	this->loc = map->place(this->loc, this) ;
}

void GameObject::wander(double xyOffset, long time) {
	void (GameObject::*wanderThrPtr)(double, long, list<thread *>::iterator) = &GameObject::wander_threaded ;
	goThread = new std::thread(wanderThrPtr, std::move(this), xyOffset, time, lastAddedThread) ;
	startThreading(this->goThread, false) ;
}

void GameObject::wander(double xyOffset, bool * run) {
	void (GameObject::*wanderThrPtr)(double, bool *, list<thread *>::iterator) = &GameObject::wander_threaded ;
	goThread = new std::thread(wanderThrPtr, std::move(this), xyOffset, run, lastAddedThread) ;
	startThreading(this->goThread, false) ;
}


void GameObject::wander_threaded(double xyOffset, long time, list<thread *>::iterator pos) {
	BasicTime timer ;
	timer.startTimer() ;
	while (timer.checkTimeElapsed() < time) {
		double nX = randSignFlip(xyOffset) ;
		if (((loc->getX() + nX) > GameObject::MAX_X) || ((loc->getX() + nX) < GameObject::MIN_X)) {
			nX = (nX * -1) ;
		}
		//repeat for y coord
		double nY = randSignFlip(xyOffset) ;
		if (((loc->getY() + nY) > GameObject::MAX_Y) || ((loc->getY() + nY) < GameObject::MIN_Y)) {
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
		if (((loc->getX() + nX) > GameObject::MAX_X) || ((loc->getX() + nX) < GameObject::MIN_X)) {
			nX = (nX * -1) ;
		}
		//repeat for y coord
		double nY = randSignFlip(xyOffset) ;
		if (((loc->getY() + nY) > GameObject::MAX_Y) || ((loc->getY() + nY) < GameObject::MIN_Y)) {
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




