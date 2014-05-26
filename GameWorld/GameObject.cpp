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


bool GameObject::map_is_init = false ;

vector<pair <thread *, GameObject*> > * GameObject::allThreads  = new vector<pair <thread *, GameObject*> >() ;

vector<GameObject *> * GameObject::allGameObjects = new vector<GameObject*>() ;


GameMap<GameObject> * GameObject::map = new GameMap<GameObject>(GLOBAL_MAX_X, GLOBAL_MAX_Y) ;

FastRand<int> GameObject::goRand(FastRand<int>(0, INT_MAX));


GameObject::GameObject() :
	ID(IDs),
	textureImageFile(AssetFileIO::getRandomImageFilename(AssetType::character)),
	size(Size<int>()),
	type(AssetType::character),
	loc(new Pos2<long>(0, 0, 0, defaultCheck)),
	vectDir(VectorHeading<long>(loc))
{
	IDs++ ;

	FastRand<float> randSize(0.5, 1.0) ; //set sizeModifier to something small, since these are just randomly generated (likely enemies)
	size.setModifier(randSize()) ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}

	allGameObjects->push_back(this) ;
	map->place(this->loc, this, defaultCheck, true) ;
	initGraphicsData(false) ;
	//already set: currentlyThreading = new bool(false) ;
}

GameObject::GameObject(const GameObject & other) :
	currentlyThreading(new bool(*(other.currentlyThreading))),
	goThread(nullptr),
	ID(IDs),
	textureImageFile(string(other.textureImageFile)),
	texture(nullptr), //this GameObject willfigure out what it's own texture and size via initGraphicsData()
	size(Size<int>()),
	type(other.type),
	loc(new Pos2<long>(*(other.loc), defaultCheck)),
	vectDir(VectorHeading<long>(other.vectDir))
{
	/* debug */
	stringstream ss ;
	ss << "Warning: Copy constructor called on GameObject ID# " << other.ID
		<< endl << "Dumping description of GameObject to be copied from: " << endl << other << endl ;
	*(Debug::debugOutput) << ss.rdbuf() ;
	/* end debug */
	
	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	
	/* places and updates to our new (nearby) Position if place unsuccessful at given Loc */
	map->place(this->loc, this, defaultCheck, true) ;
	
	allGameObjects->push_back(this) ;

	initGraphicsData(true) ;
	
	/* Don't want to copy goThread or goIterator */
	
	/* debug */
	stringstream st ;
	st << "Warning: Copy constructor finished copying GameObject ID# " << other.ID
		<< " to GameObject ID# " << this->ID << '\n' << "Dumping desciption of GameObject copied to: " << '\n' << this << '\n' ;
	*(Debug::debugOutput) << st.rdbuf() ;
	/* end debug */
}

GameObject::GameObject(GameObject && other) :
	currentlyThreading(other.currentlyThreading),
	goThread(other.goThread),
	ID(other.ID),
	textureImageFile(std::move(other.textureImageFile)),
	texture(other.texture),
	size(std::move(other.size)),
	type(other.type),
	loc(other.loc),
	vectDir(std::move(other.vectDir))
{
	/* debug */
	*(Debug::debugOutput) << "Warning: move constructor called. \n" ;
	/*end debug */
	
	//don't need to incr IDs
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	
	/* There's already references to us on the map and in 
	 allGameObjects, don't need to add us again */
	
	if (!(*(other.currentlyThreading))) {
		other.currentlyThreading = nullptr ;
		other.goThread = nullptr ;
	}
	other.ID = 0 ;
	other.textureImageFile = "" ;
	other.texture = nullptr ;
	other.loc = nullptr ;
}

GameObject::GameObject(AssetType type, const string & imageFileName, float modifier, const Pos2<long> & loc_) :
	goThread(nullptr),
	ID(IDs),
	textureImageFile(imageFileName),
	size(Size<int>()),
	type(type),
	loc(new Pos2<long>(loc_, defaultCheck)),
	vectDir(VectorHeading<long>(loc))
{
	IDs++ ;

	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
	map->place(this->loc, this, defaultCheck, true) ;
	initGraphicsData(false) ;
	size.setModifier(modifier) ;
	//already set: currentlyThreading = new bool(false) ;
}

GameObject::GameObject(FastRand<long> rand) :
	goThread(nullptr),
	ID(IDs),
	type(AssetType::character), //TODO randomly select other AssetTypes if we add them later
	textureImageFile(AssetFileIO::getRandomImageFilename(AssetType::character)),
	size(Size<int>()),
	loc(new Pos2<long>(rand, defaultCheck)),
	vectDir(VectorHeading<long>(loc))
{
	IDs++ ;

	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}

	allGameObjects->push_back(this) ;
	map->place(this->loc, this, defaultCheck, true) ;
	initGraphicsData(false) ;
	FastRand<float> randSize(0.5, 1.0) ; //set sizeModifier to something small, since these are just randomly generated (likely enemies)
	size.setModifier(randSize()) ;
	//already set: currentlyThreading = new bool(false) ;
}


GameObject::~GameObject() {
	
	eraseByID(this->ID) ;

	if ((currentlyThreading != nullptr) && (*currentlyThreading == false)) {
		delete currentlyThreading ;
		SDL_DestroyTexture(texture) ;
		if (goThread != nullptr) {
			delete this->goThread ;
		}
		if (loc != nullptr) {
			map->erase(loc) ;
			delete loc ;
		}
	}
}

GameObject & GameObject::operator=(const GameObject & rhs) {
	/* Debug code */
	stringstream ss ;
	ss << "Warning: GameObject assignment operator overload (copy) called. This will cause performance issues." << '\n' ;
	DebugOutput << ss.rdbuf() ;
	/* End Debug code */
	if (this != &rhs) {
		*(this->currentlyThreading) = false ;
		this->goThread = nullptr ;
		this->ID = IDs ;
		this->textureImageFile = rhs.textureImageFile ;
		this->type = rhs.type ;
		initGraphicsData(true) ;

		if (this->loc != nullptr) {
			map->erase(this->loc) ;
			delete loc ;
		}

        loc = new Pos2<long>(*rhs.loc) ;
		vectDir = VectorHeading<long>(loc) ;
		map->place(this->loc, this, defaultCheck, true) ;
		vectDir.updateAndNormalize() ;
		
		allGameObjects->push_back(this) ;

		IDs++ ;
	}
	return *this ;
}

GameObject & GameObject::operator=(GameObject && rhs) {
	/* Debug code */
	stringstream ss ;
	ss << "Warning: GameObject assignment operator overload (move) called." << '\n' ;
	DebugOutput << ss.rdbuf() ;
	/* End Debug code */
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
		
		this->textureImageFile = std::move(rhs.textureImageFile) ;
		this->texture = rhs.texture ;
		this->size = std::move(rhs.size) ;
		this->type = rhs.type ;
		if (this->loc != nullptr) {
			delete this->loc ;
		}
        this->loc = rhs.loc ;
		this->vectDir = std::move(rhs.vectDir) ;
		loc->checkBounds(defaultCheck) ;
		vectDir.updateAndNormalize() ;
		
		this->ID = rhs.ID ;
		rhs.ID = 0 ;
		rhs.textureImageFile = "" ;
		rhs.texture = nullptr ;
		rhs.loc = nullptr ;
	}
	return *this ;
}

void GameObject::operator()() {
	defaultBehaviors() ;
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

void GameObject::initGraphicsData(bool overrideCurrentTexture) {

	//set texture
	if ((texture == nullptr) || (overrideCurrentTexture)) {
		SDL_Texture * tex = nullptr ;
		tex = AssetFileIO::getTextureFromFilename(SharedGameData::getMainRenderer(), this->getImageFile(), this->getType()) ;

		if (tex == nullptr) {
			stringstream ss ;
			ss << "Load texture failed." << '\n' ;
			ss << SDL_GetError() << '\n' ;
			DebugOutput << ss.rdbuf() ;
			throw exception() ;
		}

		this->setTexture(tex) ;
	}

	//set size
	int tempW = this->size.getWidth() ; //don't ever assign or change size directly
	int tempH = this->size.getHeight() ;
	SDL_QueryTexture(texture, NULL, NULL, &tempW, &tempH) ; //init local size with size of texture
	this->setSize(tempW, tempH) ; //assign new size to this GameObject
}

void GameObject::checkForMarkedDeletions() {
	while (GLOBAL_CONTINUE_SIGNAL) {
		for (auto i = 0 ; i < allGameObjects->size() ; i++) {
			if (allGameObjects->at(i)->markedForDeletion) {
				*allGameObjects->at(i)->currentlyThreading = false ;
				delete allGameObjects->at(i) ;
				allGameObjects->at(i) = nullptr ;
			}
		}
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

void GameObject::joinThreads() {
	for (auto i = allThreads->begin() ; i != allThreads->end() ; i++) {
		get<0>(*i)->join() ;
		*(get<1>(*i)->currentlyThreading) = false ;
	}
}

void GameObject::notify() {
	//todo
}

void GameObject::passMessage(Message * message, GameObject & recipient) {
	//todo
}

void GameObject::startThreading(void (GameObject::*functionPointer)(), bool wait) {
	this->goThread = new thread(functionPointer, this) ;
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

void GameObject::textDescription(ostream * writeTo) const {
	stringstream ss ;
	ss << "GameObject ID#: " << this->ID << endl ;
	if (loc != nullptr) {
		ss << "Current Position: " << loc->toString() << endl ;
	}
	*writeTo << ss.rdbuf() ;
}

void GameObject::moveTo(Position<long> * to) {
	to->checkBounds(defaultCheck) ;
	map->erase(this->getPosition()) ;
	this->loc->setAll(*to) ;
	map->place(this->loc, this, defaultCheck, true) ;
	vectDir.updateAndNormalize() ;
}

void GameObject::moveTo(Position<long> to) {
	to.checkBounds(defaultCheck) ;
	map->erase(this->getPosition()) ;
	this->loc->setAll(to) ;
	map->place(this->loc, this, defaultCheck, true) ;
	vectDir.updateAndNormalize() ;
}

void GameObject::moveSameDirection() {
	vectDir.normalize() ;
	auto next = vectDir.calculateNextPosition(defaultCheck) ;
	moveTo(std::move(next)) ;
}

void GameObject::moveNewDirection(VectorHeading<long> & newDirection) {
	newDirection.normalize() ;
	auto next = VectorHeading<long>::calculateNextPosition(newDirection, loc, defaultCheck) ;
	moveTo(std::move(next)) ;
}

void GameObject::defaultBehaviors_threaded() {
	void (GameObject::*behaviorsPtr)() = &GameObject::defaultBehaviors ;
	startThreading(behaviorsPtr, false) ;
}

void GameObject::defaultBehaviors() {
	//we can change this to whatever we want
}

void GameObject::wanderVariedSpeed(FastRand<unsigned> speedVariance) {
	unsigned speedChange = speedVariance.nextValue() ; //smaller values are faster
	wander(speedChange, false) ;
}

void GameObject::attack(GameObject * enemy) {
	
}

void GameObject::findNearbyAlly(long searchDistanceX, long searchDistanceY) {
	vector<GameObject *> * nearby = map->findNearby(loc, searchDistanceX, searchDistanceY) ;
	
	if ((nearby != nullptr) && (nearby->size() > 0)) {
		allyWith(nearby->at(0)) ;
	}
}

void GameObject::allyWith(const GameObject * other) {
	this->ally = other ;
}

void GameObject::wander(long xyOffset, bool followAlly) {

	if ((!followAlly) || (ally == nullptr)) {

		float nX = randSignFlip(xyOffset) ;
		if (((loc->getX() + nX) > GLOBAL_MAX_X) || ((loc->getX() + nX) < GLOBAL_MIN_X)) {
			nX = (nX * -1) ;
		}

		//repeat for y coord
		float nY = randSignFlip(xyOffset) ;
		if (((loc->getY() + nY) > GLOBAL_MAX_Y) || ((loc->getY() + nY) < GLOBAL_MIN_Y)) {
			nY = (nY * -1) ;
		}
		auto vecdir = VectorHeading<long>(nX, nY, 0, this->loc) ;
		moveNewDirection(vecdir) ;
	}
	else if ((ally != nullptr) && (followAlly)) {
		this->moveTo(*ally->getPosition()) ;
	}
}

void GameObject::setImageFile(string imageFileName) {
	this->textureImageFile = imageFileName ;
}

string GameObject::getImageFile() const {
	return this->textureImageFile ;
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





