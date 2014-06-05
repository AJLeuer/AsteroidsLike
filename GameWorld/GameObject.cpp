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
	textureImageFile(AssetFileIO::getRandomImageFilename(AssetType::block)),
	size(Size<int>()),
	type(AssetType::block),
	visible(true),
	loc(new Pos2<float>(0.0, 0.0, 0.0, defaultCheck<float>)),
	vectr(DirectionVector<float>(loc))
{
	IDs++ ;

	FastRand<float> randSize(0.5, 1.0) ; //set sizeModifier to something small, since these are just randomly generated (likely enemies)
	size.setModifier(randSize()) ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}

	allGameObjects->push_back(this) ;
	map->place<float>(loc, this, defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	initGraphicsData(false) ;
	//already set: hasThread = new bool(false) ;
}

GameObject::GameObject(const GameObject & other) :
	hasThread(new bool(false)),
	goThread(nullptr),
	ID(IDs),
	textureImageFile(string(other.textureImageFile)),
	texture(nullptr), //this GameObject willfigure out what it's own texture and size via initGraphicsData()
	size(Size<int>()),
	type(other.type),
	visible(other.visible),
	loc(new Pos2<float>(*(other.loc), defaultCheck<float>)),
	vectr(DirectionVector<float>(other.vectr.getX(), other.vectr.getY(), other.vectr.getZ(), loc))
{
	{
	/* debug */
	#ifdef DEBUG_MODE
	stringstream ss ;
	ss << "Warning: Copy constructor called on GameObject ID# " << other.ID
		<< endl << "Dumping description of GameObject to be copied from: " << endl << other << endl ;
	*(Debug::debugOutput) << ss.rdbuf() ;
	#endif
	/* end debug */
	}
	
	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	
	/* places and updates to our new (nearby) Position if place unsuccessful at given Loc */
	map->place<float>(loc, this, defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	
	allGameObjects->push_back(this) ;

	initGraphicsData(true) ;
	
	/* Don't want to copy goThread or goIterator */

	{
	/* debug */
	#ifdef DEBUG_MODE
	stringstream st ;
	st << "Warning: Copy constructor finished copying GameObject ID# " << other.ID
		<< " to GameObject ID# " << this->ID << '\n' << "Dumping desciption of GameObject copied to: " << '\n' << this << '\n' ;
	*(Debug::debugOutput) << st.rdbuf() ;
	#endif
	/* end debug */
	}

}

GameObject::GameObject(GameObject && other) :
	hasThread(other.hasThread),
	goThread(other.goThread),
	ID(other.ID),
	textureImageFile(std::move(other.textureImageFile)),
	texture(other.texture),
	size(std::move(other.size)),
	type(other.type),
	visible(other.visible),
	loc(other.loc),
	vectr(std::move(other.vectr))
{
	{
	/* debug */
	#ifdef DEBUG_MODE
	*(Debug::debugOutput) << "Warning: move constructor called. \n" ;
	#endif
	/*end debug */
	}
	
	//don't need to incr IDs
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	
	/* There's already references to us on the map and in 
	 allGameObjects, don't need to add us again */
	

	other.hasThread = nullptr ;
	other.goThread = nullptr ;

	other.ID = 0 ;
	other.textureImageFile = "" ;
	other.texture = nullptr ;
	other.loc = nullptr ;
}

GameObject::GameObject(AssetType type, const string & imageFilename, float sizeModifier, const Pos2<float> & loc_) :
	goThread(nullptr),
	ID(IDs),
	textureImageFile(imageFilename),
	size(Size<int>()),
	type(type),
	visible(true),
	loc(new Pos2<float>(loc_, defaultCheck<float>)),
	vectr(DirectionVector<float>(loc))
{
	IDs++ ;

	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
	map->place<float>(loc, this, defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	initGraphicsData(false) ;
	size.setModifier(sizeModifier) ;
	//already set: hasThread = new bool(false) ;
}

GameObject::GameObject(FastRand<int> rand) :
	goThread(nullptr),
	ID(IDs),
	type(AssetType::block), //TODO randomly select other AssetTypes if we add them later
	visible(true),
	textureImageFile(AssetFileIO::getRandomImageFilename(AssetType::block)),
	size(Size<int>()),
	loc(new Pos2<float>(rand, defaultCheck<float>)),
	vectr(DirectionVector<float>(loc))
{
	IDs++ ;

	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	allGameObjects->push_back(this) ;
	map->place<float>(loc, this, defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	initGraphicsData(false) ;
	FastRand<float> randSize(0.5, 1.0) ; //set sizeModifier to something small, since these are just randomly generated (likely enemies)
	size.setModifier(randSize()) ;
	//already set: hasThread = new bool(false) ;
}


GameObject::~GameObject() {
	
	eraseByID(this->ID) ;

	if ((hasThread != nullptr) && (*hasThread == false)) {
		delete hasThread ;
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

	{
	/* Debug code */
	#ifdef DEBUG_MODE
	stringstream ss ;
	ss << "Warning: GameObject assignment operator overload (copy) called. This will cause performance issues." << '\n' ;
	DebugOutput << ss.rdbuf() ;
	#endif
	/* End Debug code */
	}

	if (this != &rhs) {
		*(this->hasThread) = false ;
		if (this->goThread != nullptr) {
			delete this->goThread ;
			this->goThread = nullptr ;
		}
		this->ID = IDs ;
		this->textureImageFile = rhs.textureImageFile ;
		this->type = rhs.type ;
		this->visible = rhs.visible ;
		initGraphicsData(true) ;

		if (this->loc != nullptr) {
			map->erase<float>(loc) ;
			delete loc ;
		}

        loc = std::move(new Pos2<float>(*rhs.loc)) ;
		vectr = DirectionVector<float>(rhs.vectr.getX(), rhs.vectr.getY(), rhs.vectr.getZ(), loc) ;
		map->place<float>(loc, this, defaultCheck<float>, true) ;
		vectr.updateAndNormalize() ;

		allGameObjects->push_back(this) ;

		IDs++ ;
	}
	return *this ;
}

GameObject & GameObject::operator=(GameObject && rhs) {

	{
	/* Debug code */
	#ifdef DEBUG_MODE
	stringstream ss ;
	ss << "Warning: GameObject assignment operator overload (move) called." << '\n' ;
	DebugOutput << ss.rdbuf() ;
	#endif
	/* End Debug code */
	}

	if (this != &rhs) {
		if (this->goThread != nullptr) { //should never be null
			delete this->goThread ;
		}
		delete this->hasThread ;
		
		this->hasThread = rhs.hasThread ;
		this->goThread = rhs.goThread ;
		
		this->textureImageFile = std::move(rhs.textureImageFile) ;
		this->texture = rhs.texture ;
		this->size = std::move(rhs.size) ;
		this->type = rhs.type ;
		this->visible = rhs.visible ;
		if (this->loc != nullptr) {
			delete this->loc ;
		}
        this->loc = rhs.loc ;
		this->vectr = std::move(rhs.vectr) ;
		
		this->ID = rhs.ID ;
		
		rhs.hasThread = nullptr ;
		rhs.goThread = nullptr ;
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
		tex = AssetFileIO::getTextureFromFilename(GameState::getMainRenderer(), this->getImageFile(), this->getType()) ;

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

void GameObject::checkForMarkedDeletions() { //will run on own thread
	while (GLOBAL_CONTINUE_SIGNAL) {
		for (auto i = 0 ; i < allGameObjects->size() ; i++) {
			if (allGameObjects->at(i)->markedForDeletion) {
				*allGameObjects->at(i)->hasThread = false ;
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
		*(get<1>(*i)->hasThread) = false ;
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
	*(this->hasThread) = true ;
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
	*(this->hasThread) = false ;
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

void GameObject::moveTo(Position<float> * to) {
	map->erase<float>(getPosition()) ;
	loc->setAll(*to) ;
	map->place<float>(loc, this, defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	
	{
	/* Debug code */
	/*
	#ifdef DEBUG_MODE
	stringstream ss ;
	ss << "Current size of loc archive: " << loc->getHistory()->size() << '\n' ;
	DebugOutput << ss.rdbuf() ;
	#endif
	*/
	/* end debug */
	}
}

void GameObject::moveTo(Position<float> to) {
	map->erase<float>(getPosition()) ;
	loc->setAll(to) ;
	map->place<float>(loc, this, defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	
	{
	/* Debug code */
	/*
	#ifdef DEBUG_MODE
	stringstream ss ;
	ss << "Current size of loc archive: " << loc->getHistory()->size() << '\n' ;
	DebugOutput << ss.rdbuf() ;
	#endif
	*/
	/* end debug */
	}
}

void GameObject::moveSameDirection() {

	vectr.normalize() ;
	Position<float> next = DirectionVector<float>::calculateNextPosition(vectr) ;

	if (next.overXBounds(defaultCheck<float>)) {
		next = DirectionVector<float>::calculateReverseXPosition(vectr, defaultCheck<float>) ;
	}
	if (next.overYBounds(defaultCheck<float>)) {
		next = DirectionVector<float>::calculateReverseYPosition(vectr, defaultCheck<float>) ;
	}

	moveTo(std::move(next)) ;
}

void GameObject::moveNewDirection(DirectionVector<float> & newDirection) {

	newDirection.normalize() ;
	auto next = DirectionVector<float>::calculateNextPosition(newDirection, loc, defaultCheck<float>) ;
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
	wander() ;
}

void GameObject::attack(GameObject * enemy) {
	
}

void GameObject::findNearbyAlly(int searchDistanceX, int searchDistanceY) {
    
	vector<GameObject *> * nearby = map->findNearby<float>(loc, searchDistanceX, searchDistanceY) ;
	
	if ((nearby != nullptr) && (nearby->size() > 0)) {
		allyWith(nearby->at(0)) ;
	}
}

void GameObject::allyWith(const GameObject * other) {
	this->ally = other ;
}

void GameObject::wander() {

	FastRand<int> randm(0, 5) ;
	int cases = randm() ;

	switch (cases) {
		case 0 :
		{
			moveUp() ;
			break ;
		}

		case 1 :
		{
			moveDown() ;
			break;
		}

		case 2 :
		{
			moveUpRight() ;
			break ;
		}

		case 3 :
		{
			moveUpLeft() ;
			break ;
		}

		case 4 :
		{
			moveDownRight() ;
			break ;
		}
		case 5 :
		{
			moveDownLeft() ;
			break ;
		}
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





