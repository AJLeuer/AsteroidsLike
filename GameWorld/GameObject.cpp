//
//  GameObject.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#define eight_milliseconds 8333

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
	loc(new Position<long>(0, 0, 0, defaultCheck)),
	vectDir(vectorHeading<long>(loc))
{
	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X+1, MAX_Y+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
	map->place(this->loc, this, defaultCheck, true) ;
}

GameObject::GameObject(const GameObject & other) :
	goThread(nullptr),
	ID(IDs),
	icon(other.icon),
	loc(new Position<long>(*(other.loc), defaultCheck)),
	vectDir(vectorHeading<long>(other.vectDir))
{
	/* debug */
	stringstream ss ;
	ss << "Warning: Copy constructor called on GameObject ID# " << other.ID
	<< endl << "Dumping desciption of GameObject to be copied from: " << endl << other << endl ;
	*(Debug::debugOutput) << ss.rdbuf() ;
	/* end debug */
	
	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X+1, MAX_Y+1) ;
		map_is_init = true ;
	}
	
	/* places and updates to our new (nearby) Position if place unsuccessful at given Loc */
	map->place(this->loc, this, defaultCheck, true) ;
	
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
	vectDir(std::move(other.vectDir))
{
	/* debug */
	*(Debug::debugOutput) << "Move constructor called \n" ;
	/*end debug */
	
	//don't need to incr IDs
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X+1, MAX_Y+1) ;
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
	vectDir(vectorHeading<long>(loc))
{
	IDs++ ;
	
	loc->checkBounds(defaultCheck) ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X+1, MAX_Y+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
	map->place(this->loc, this, defaultCheck, true) ;
}

GameObject::GameObject(fastRand<long> rand) :
	goThread(nullptr),
	ID(IDs),
	icon(""),
	loc(new Position<long>(rand, defaultCheck)),
	vectDir(vectorHeading<long>(loc))
{
	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(MAX_X+1, MAX_Y+1) ;
		map_is_init = true ;
	}
	
	
	auto sz = icons->size()-1 ;
	auto tmprnd = rand.nextValue<vector<string>::size_type>(0, sz) ;
	icon = icons->at(tmprnd) ;
	
	allGameObjects->push_back(this) ;
	map->place(this->loc, this, defaultCheck, true) ;
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
        this->loc = new Position<long>(*(rhs.loc), defaultCheck) ;
		vectDir = vectorHeading<long>(loc) ;
		map->place(this->loc, this, defaultCheck, true) ;
		vectDir.updateAndNormalize() ;
		
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
		this->vectDir = std::move(rhs.vectDir) ;
		loc->checkBounds(defaultCheck) ;
		vectDir.updateAndNormalize() ;
		
		this->ID = rhs.ID ;
		rhs.ID = 0 ;
		rhs.loc = nullptr ;

	}
	return *this ;
}

void GameObject::operator()() {
	//todo
}

void GameObject::operator()(GameObject & sentObject) {

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

void GameObject::move(const Position<long> & moveTo) {
	Position<long> mt = Position<long>(moveTo, defaultCheck) ;
	map->erase(*(this->getPosition())) ;
	*(this->loc) = mt ;
	map->place(this->loc, this, defaultCheck, true) ;
	vectDir.updateAndNormalize() ;
}

void GameObject::moveSameDirection() {
	vectDir.normalize() ;
	auto next = vectDir.calculateNextPosition(defaultCheck) ;
	move(next) ;
}

void GameObject::moveNewDirection(vectorHeading<long> & newDirection) {
	newDirection.normalize() ;
	auto next = vectorHeading<long>::calculateNextPosition(newDirection, loc, defaultCheck) ;
	move(next) ;
}

void GameObject::runOnThread() {
	void (GameObject::*behaviorsPtr)() = &GameObject::defaultBehaviors ;
	this->currentlyThreading = new bool{true} ;
	goThread = new std::thread(behaviorsPtr, std::move(this)) ;
	startThreading(this->goThread, false) ;
}

void GameObject::defaultBehaviors() {
	
	int i = 0 ;
	vector<GameObject *> * nearby = nullptr ;
	auto rand = fastRand<unsigned int>(8, 40) ;
	
	while (GLOBAL_CONTINUE_SIGNAL) {
		unsigned speedChange = rand.nextValue() ;
		wander(1, (speedChange * eight_milliseconds), 5, 0) ;
		nearby = map->findNearby(loc, (long)5, (long)5) ;
		if ((nearby != nullptr) && (nearby->size() > 0)) {
			this->ally = nearby->at(0) ;
		}
		i++ ;
	}
}

void GameObject::attack(GameObject * enemy) {
	
}

void GameObject::allyWith(GameObject * ) {
	
}

void GameObject::wander(long xyOffset, unsigned timeInterval, long time) {
	Time timer ;
	timer.startTimer() ;
	while (timer.checkTimeElapsed() < time) {
		if (ally == nullptr) {
			float nX = randSignFlip(xyOffset) ;
			if (((loc->getX() + nX) > GameObject::MAX_X) || ((loc->getX() + nX) < GameObject::MIN_X)) {
				nX = (nX * -1) ;
			}
		
			//repeat for y coord
			float nY = randSignFlip(xyOffset) ;
			if (((loc->getY() + nY) > GameObject::MAX_Y) || ((loc->getY() + nY) < GameObject::MIN_Y)) {
				nY = (nY * -1) ;
			}
			auto vecdir = vectorHeading<long>(nX, nY, 0, this->loc) ;
			moveNewDirection(vecdir) ;
		}
		else if (ally != nullptr) {
			move(*ally->getPosition()) ;
		}
		usleep(timeInterval) ; //i.e. 8.33 milliseconds x 25
	}
}

void GameObject::wander(long xyOffset, unsigned timeInterval, bool * run) {
	while (*run) {
		if (ally == nullptr) {
			float nX = randSignFlip(xyOffset) ;
			if (((loc->getX() + nX) > GameObject::MAX_X) || ((loc->getX() + nX) < GameObject::MIN_X)) {
				nX = (nX * -1) ;
			}
			//repeat for y coord
			float nY = randSignFlip(xyOffset) ;
			if (((loc->getY() + nY) > GameObject::MAX_Y) || ((loc->getY() + nY) < GameObject::MIN_Y)) {
				nY = (nY * -1) ;
			}
			auto vecdir = vectorHeading<long>(nX, nY, 0, this->loc) ;
			moveNewDirection(vecdir) ;
		}
		else if (ally != nullptr) {
			move(*ally->getPosition()) ;
		}
		usleep(timeInterval) ; //i.e. 8.33 milliseconds x 25
	}
}

void GameObject::wander(long xyOffset, unsigned int timeInterval, int loops, int ignored) {
	for (long i = 0 ; i < loops ; i++) {
		if (ally == nullptr) {
			float nX = randSignFlip(xyOffset) ;
			if (((loc->getX() + nX) > GameObject::MAX_X) || ((loc->getX() + nX) < GameObject::MIN_X)) {
				nX = (nX * -1) ;
			}
			//repeat for y coord
			float nY = randSignFlip(xyOffset) ;
			if (((loc->getY() + nY) > GameObject::MAX_Y) || ((loc->getY() + nY) < GameObject::MIN_Y)) {
				nY = (nY * -1) ;
			}
			auto vecdir = vectorHeading<long>(nX, nY, 0, this->loc) ;
			moveNewDirection(vecdir) ;
		}
		else if (ally != nullptr) {
			move(*ally->getPosition()) ;
		}
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





