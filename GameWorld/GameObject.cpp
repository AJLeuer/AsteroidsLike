//
//  GameObject.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "GameObject.h"

/* starts at 1 (0 is a special case */
unsigned GameObject::IDs = 1 ;

vector<string> * GameObject::icons = new vector<string>{"🎾", "🔱", "💩", "🍹", "🎅", "👿", "👮", "👹", "🚶", "👩", "💂", "💊"} ;


bool GameObject::map_is_init = false ;

vector<pair <thread *, GameObject*> > * GameObject::allThreads  = new vector<pair <thread *, GameObject*> >() ;

vector<GameObject *> * GameObject::allGameObjects = new vector<GameObject*>() ;


GameMap<GameObject> * GameObject::map = new GameMap<GameObject>(MAX_X, MAX_Y) ;

const long GameObject::MAX_X { GLOBAL_MAX_X } ;
const long GameObject::MIN_X { GLOBAL_MIN_X } ;
const long GameObject::MAX_Y { GLOBAL_MAX_Y } ;
const long GameObject::MIN_Y { GLOBAL_MIN_Y } ;



fastRand<int> GameObject::goRand(fastRand<int>(0, INT_MAX));


GameObject::GameObject() :
	ID(IDs),
	icon("no icon"),
	loc(new Position<long>(0, 0, 0, Position<long>::defaultCheck)),
	vectDir(new vectorHeading<long>(loc))
{
	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X, MAX_Y) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
	map->place(this->loc, this, Position<long>::defaultCheck, true) ;
	vectDir.update(loc) ;
}

GameObject::GameObject(const GameObject & other) :
	goThread(nullptr),
	ID(IDs),
	icon(other.icon),
	loc(new Position<long>(*(other.loc), Position<long>::defaultCheck)),
	vectDir(new vectorHeading<long>(loc))
{
	/* debug */
	stringstream ss ;
	ss << "Warning: Copy constructor called on GameObject ID# " << other.ID
	<< endl << "Dumping desciption of GameObject to be copied from: " << endl << other << endl ;
	*(Debug::debugOutput) << ss.rdbuf() ;
	/* end debug */
	
	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X, MAX_Y) ;
		map_is_init = true ;
	}
	
	/* places and updates to our new (nearby) Position if place unsuccessful at given Loc */
	map->place(this->loc, this, Position<long>::defaultCheck, true) ;
	vectDir.update(loc) ;
	
	allGameObjects->push_back(this) ;
	
	/* Don't want to copy goThread or goIterator */
	
	/* debug */
	stringstream st ;
	st << "Warning: Copy constructor finished copying GameObject ID# " << other.ID
		<< " to GameObject ID# " << this->ID << endl << "Dumping desciption of GameObject copied to: " << endl << this << endl ;
	*(Debug::debugOutput) << st.rdbuf() ;
	/* end debug */
}

GameObject::GameObject(GameObject && other) :
	currentlyThreading(other.currentlyThreading),
	goThread(other.goThread),
	ID(other.ID),
	icon(std::move(other.icon)),
	loc(other.loc),
	vectDir(new vectorHeading<long>(loc))
{
	/* debug */
	*(Debug::debugOutput) << "Move constructor called \n" ;
	/*end debug */
	
	//don't need to incr IDs
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X, MAX_Y) ;
		map_is_init = true ;
	}
	
	/* There's already references to us on the map and in 
	 allGameObjects, don't need to add us again */
	
	if (!currentlyThreading) {
		other.currentlyThreading = nullptr ;
		other.goThread = nullptr ;
	}
	other.ID = 0 ;
	other.loc = nullptr ;
}

GameObject::GameObject(string symbol, Position<long> * loc) :
	goThread(nullptr),
	ID(IDs),
	icon(symbol),
	loc(loc),
	vectDir(new vectorHeading<long>(loc))
{
	IDs++ ;
	
	loc->checkBounds(Position<long>::defaultCheck) ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X, MAX_Y) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
	map->place(this->loc, this, Position<long>::defaultCheck, true) ;
	vectDir.update(loc) ;
}

GameObject::GameObject(int randSeed) :
	goThread(nullptr),
	ID(IDs),
	icon(""),
	loc(nullptr),
	vectDir(vectorHeading<long>(0))
{
	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X, MAX_Y) ;
		map_is_init = true ;
	}
	
	auto rnd = fastRand<long>(0, 100) ; //we'll get our own
	
	auto sz = icons->size()-1 ;
	auto tmprnd = rnd.nextValue<vector<string>::size_type>(0, sz) ;
	icon = icons->at(tmprnd) ;
	
	long x = (rnd.nextValue(0, lrint(MAX_X))) ;
	long y = (rnd.nextValue(0, lrint(MAX_Y))) ;
	
	loc = new Position<long>(x, y, 0, Position<long>::defaultCheck) ;
	
	allGameObjects->push_back(this) ;
	map->place(this->loc, this, Position<long>::defaultCheck, true) ;
	vectDir.update(loc) ;
}


GameObject::~GameObject() {
	eraseByID(this->ID) ;
	
	if ((currentlyThreading != nullptr) && (*currentlyThreading == false)) {
		delete currentlyThreading ;
		if (goThread != nullptr) {
			delete this->goThread ;
		}
		if (loc != nullptr) {
			map->erase(*loc) ;
			delete loc ;
		}
	}
}

GameObject & GameObject::operator=(const GameObject & rhs) {
	if (this != &rhs) {
		*(this->currentlyThreading) = false ;
		this->goThread = nullptr ;
		this->ID = IDs ;
		this->icon = rhs.icon ;
		if (this->loc != nullptr) {
			map->erase(*(this->loc)) ;
			delete loc ;
		}
        this->loc = new Position<long>(*(rhs.loc), Position<long>::defaultCheck) ;
		map->place(this->loc, this, Position<long>::defaultCheck, true) ;
		allGameObjects->push_back(this) ;
		
		IDs++ ;
	}
	return *this ;
}

GameObject & GameObject::operator=(GameObject && rhs) {
	if (this != &rhs) {
		if (this->currentlyThreading != nullptr) {
			delete this->currentlyThreading ;
		}
		this->currentlyThreading = rhs.currentlyThreading ;
		if (this->goThread != nullptr) {
			delete this->goThread ;
		}
		this->goThread = rhs.goThread ;
		
		
		if (!currentlyThreading) {
			rhs.currentlyThreading = nullptr ;
			rhs.goThread = nullptr ;
		}
		
		this->icon = std::move(rhs.icon) ;
		if (this->loc != nullptr) {
			delete this->loc ;
		}
        this->loc = rhs.loc ;
		loc->checkBounds(Position<long>::defaultCheck) ;
		
		this->ID = rhs.ID ;
		rhs.ID = 0 ;
		rhs.loc = nullptr ;

	}
	return *this ;
}

void GameObject::operator()() {
	//todo
}

void GameObject::operator()(GameObject &sentObject) {

}

bool GameObject::operator==(GameObject & other) const {
	if (this->ID == other.ID) {
		return true ;
	}
	else {
		return false ;
	}
}

void GameObject::eraseByID(unsigned ID) {
	for (auto i = 0 ; i < allGameObjects->size() ; i++) {
		if (((allGameObjects->at(i)) != nullptr) && ((allGameObjects->at(i)->ID) == ID)) {
			allGameObjects->at(i) = nullptr ;
			break ;
		}
	}
}

void GameObject::notify() {
	//todo
}

void GameObject::passMessage(Message * message, GameObject & recipient) {
	//todo
}

void GameObject::startThreading(std::thread * goThr, bool wait) {
	*(this->currentlyThreading) = true ;
	pair<thread *, GameObject *> threadPair = pair<thread *, GameObject *>(this->goThread, this) ;
	GameObject::allThreads->push_back(threadPair) ;
	if (wait) {
		goThread->join() ;
		allThreads->pop_back() ;
		delete this->goThread ;
		this->goThread = nullptr ;
	}
}

void GameObject::endThreading(bool join) {
	*(this->currentlyThreading) = false ;
	if (join) {
		goThread->join() ;
	}
	for (auto i = 0 ; i < allThreads->size() ; i++) {
		if (get<0>(allThreads->at(i)) == this->goThread) {
			get<0>(allThreads->at(i)) = nullptr ;
		}
	}
	//delete this->goThread ;
	//this->goThread = nullptr ;
}

void GameObject::joinThreads() {
	for (auto i = allThreads->begin() ; i != allThreads->end() ; i++) {
		get<0>(*i)->join() ;
		*(get<1>(*i)->currentlyThreading) = false ;
	}
}

void GameObject::textDescription(ostream * writeTo) const {
	stringstream ss ;
	ss << "GameObject ID#: " << this->ID << endl ;
	ss << "Icon: " << this->icon << endl ;
	if (loc != nullptr) {
		ss << "Current Position: " << loc->toString() << endl ;
	}
	*writeTo << ss.rdbuf() ;
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
	map->erase(*(this->getPosition())) ;
	this->loc->modify(xoffset, yoffset, 0, Position<long>::defaultCheck) ;
	map->place(this->loc, this, Position<long>::defaultCheck, true) ; // place may need to modify this->loc again to find spot on map
	vectDir.update(loc) ;
}

void GameObject::move(const Position<long> & moveTo) {
	Position<long> mt = Position<long>(moveTo, Position<long>::defaultCheck) ;
	map->erase(*(this->getPosition())) ;
	*(this->loc) = mt ;
	map->place(this->loc, this, Position<long>::defaultCheck, true) ;
	vectDir.update(loc) ;
}

void GameObject::wander(long xyOffset, unsigned timeInterval, long time) {
	void (GameObject::*wanderThrPtr)(long, unsigned, long) = &GameObject::wander_threaded ;
	this->currentlyThreading = new bool{true} ;
	goThread = new std::thread(wanderThrPtr, std::move(this), xyOffset, timeInterval, time) ;
	startThreading(this->goThread, false) ;
}

void GameObject::wander(long xyOffset, unsigned timeInterval, bool * run) {
	void (GameObject::*wanderThrPtr)(long, unsigned, bool *) = &GameObject::wander_threaded ;
	this->currentlyThreading = new bool{true} ;
	goThread = new std::thread(wanderThrPtr, std::move(this), xyOffset, timeInterval, run) ;
	startThreading(this->goThread, false) ;
}


void GameObject::wander_threaded(long xyOffset, unsigned timeInterval, long time) {
	Time timer ;
	timer.startTimer() ;
	while (timer.checkTimeElapsed() < time) {
		long nX = randSignFlip(xyOffset) ;
		if (((loc->getX() + nX) > GameObject::MAX_X) || ((loc->getX() + nX) < GameObject::MIN_X)) {
			nX = (nX * -1) ;
		}
		
		//repeat for y coord
		long nY = randSignFlip(xyOffset) ;
		if (((loc->getY() + nY) > GameObject::MAX_Y) || ((loc->getY() + nY) < GameObject::MIN_Y)) {
			nY = (nY * -1) ;
		}
		move(nX, nY) ;
		usleep(timeInterval) ; //i.e. 8.33 milliseconds x 25
	}
}

void GameObject::wander_threaded(long xyOffset, unsigned timeInterval, bool * run) {
	while (*run) {
		long nX = randSignFlip(xyOffset) ;
		if (((loc->getX() + nX) > GameObject::MAX_X) || ((loc->getX() + nX) < GameObject::MIN_X)) {
			nX = (nX * -1) ;
		}
		//repeat for y coord
		long nY = randSignFlip(xyOffset) ;
		if (((loc->getY() + nY) > GameObject::MAX_Y) || ((loc->getY() + nY) < GameObject::MIN_Y)) {
			nY = (nY * -1) ;
		}
		move(nX, nY) ;
		usleep(timeInterval) ; //i.e. 8.33 milliseconds x 25
	}
}


void GameObject::setIcon(const string & icon) {
	this->icon = icon ;
}

const string & GameObject::getIcon() const {
	return this->icon ;
}

ostream & operator<<(std::ostream & os, const GameObject & gameObj)  {
	gameObj.textDescription(&os) ;
	return os ;
}

string GameObject::toString() const {
	stringstream ss ;
	this->textDescription(&ss) ;
	return (ss.str()) ;
}





