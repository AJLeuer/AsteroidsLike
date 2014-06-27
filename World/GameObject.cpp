//
//  GameObject.cpp
//  World
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
	textureImageFile(""),
	texture(nullptr),
	size(Size<int>()),
	type(),
	visible(true),
	loc(new Pos2<float>(0.0, 0.0, 0.0, *defaultCheck<float>)),
	vectr(DirectionVector<float>(loc))
{
	IDs++ ;

	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}

	allGameObjects->push_back(this) ;
    
    loc->checkBounds(*defaultCheck<float>, size.getWidth(), size.getHeight()) ;
	map->place<float>(loc, this, *defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	/* No graphics data initialization here */
}

GameObject::GameObject(const GameObject & other) :
	ID(IDs),
	textureImageFile(string(other.textureImageFile)),
	texture(nullptr), //this GameObject willfigure out what it's own texture and size via initGraphicsData()
	size(Size<int>()),
	type(other.type),
	visible(other.visible),
	loc(new Pos2<float>(*(other.loc), *defaultCheck<float>)),
	vectr(DirectionVector<float>(other.vectr.getX(), other.vectr.getY(), other.vectr.getZ(), loc))
{
	{
	/* debug */
	stringstream ss ;
	ss << "Warning: Copy constructor called on GameObject ID# " << other.ID
		<< endl << "Dumping description of GameObject to be copied from: " << endl << other << endl ;
	*(Debug::debugOutput) << ss.rdbuf() ;
	/* end debug */
	}
	
	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	
    loc->checkBounds(*defaultCheck<float>, size.getWidth(), size.getHeight()) ;
	map->place<float>(loc, this, *defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	
	allGameObjects->push_back(this) ;

	initGraphicsData(true, other.getSize()->getModifier()) ;
	
	/* Don't copy gthread or goIterator */

	{
	/* debug */
	stringstream st ;
	st << "Warning: Copy constructor finished copying GameObject ID# " << other.ID
		<< " to GameObject ID# " << this->ID << '\n' << "Dumping desciption of GameObject copied to: " << '\n' << this << '\n' ;
	*(Debug::debugOutput) << st.rdbuf() ;
	/* end debug */
	}
}

GameObject::GameObject(GameObject && other) :
	hasThread(other.hasThread),
	gthread(other.gthread),
	ID(other.ID),
	textureImageFile(std::move(other.textureImageFile)),
	texture(other.texture), /* No initGraphicsData() for move operations, just steal from other */
	size(std::move(other.size)),
	type(other.type),
	visible(other.visible),
	loc(other.loc),
	vectr(std::move(other.vectr))
{
	{
	/* debug */
	*(Debug::debugOutput) << "Warning: move constructor called. \n" ;
	/*end debug */
	}
	
	/* don't need to incr IDs */
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	
	/* There's already references to us on the map and in 
	 allGameObjects, don't need to add us again */
	
	other.hasThread = nullptr ;
	other.gthread = nullptr ;
	other.ID = 0 ;
	other.textureImageFile = "" ;
	other.texture = nullptr ;
	other.loc = nullptr ;
}

GameObject::GameObject(AssetType type, const string & imageFilename, float sizeModifier, const Pos2<float> & loc_) :
	ID(IDs),
	textureImageFile(imageFilename),
	size(Size<int>()),
	type(type),
	visible(true),
	loc(new Pos2<float>(loc_, *defaultCheck<float>)),
	vectr(DirectionVector<float>(loc))
{
	IDs++ ;

	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
    
    loc->checkBounds(*defaultCheck<float>, size.getWidth(), size.getHeight()) ;
	map->place<float>(loc, this, *defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	initGraphicsData(false, sizeModifier) ;
}

GameObject::GameObject(FastRand<int> rand) :
	ID(IDs),
	type(randomEnumeration<AssetType>(2)), /* TODO change 2 to the maximum value within AssetType if more are added */
	visible(true),
	size(Size<int>()),
	loc(new Pos2<float>(rand, *defaultCheck<float>)),
	vectr(DirectionVector<float>(loc))
{
	IDs++ ;

	if (!map_is_init) {
		map = new GameMap<GameObject>(GLOBAL_MAX_X+1, GLOBAL_MAX_Y+1) ;
		map_is_init = true ;
	}
	allGameObjects->push_back(this) ;
    
    loc->checkBounds(*defaultCheck<float>, size.getWidth(), size.getHeight()) ;
	map->place<float>(loc, this, *defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	
	textureImageFile = AssetFileIO::getRandomImageFilename(type) ;
	FastRand<float> randSizeMod(0.5, 1.0) ;
	initGraphicsData(false, randSizeMod()) ;
}


GameObject::~GameObject() {
	
	eraseByID(this->ID) ;

	if ((hasThread == nullptr) || (*hasThread == false)) {
		delete hasThread ;
		SDL_DestroyTexture(texture) ;
		if (gthread != nullptr) {
			delete this->gthread ;
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
	stringstream ss ;
	ss << "Warning: GameObject assignment operator overload (copy) called. This will cause performance issues." << '\n' ;
	DebugOutput << ss.rdbuf() ;
	/* End Debug code */
	}

	if (this != &rhs) {
		*(this->hasThread) = false ;
		if (this->gthread != nullptr) {
			delete this->gthread ;
			this->gthread = nullptr ;
		}
		this->ID = IDs ;
		this->textureImageFile = rhs.textureImageFile ;
		this->type = rhs.type ;
		this->visible = rhs.visible ;
		initGraphicsData(true, rhs.getSize()->getModifier()) ;

		if (this->loc != nullptr) {
			map->erase<float>(loc) ;
			delete loc ;
		}

        loc = std::move(new Pos2<float>(*rhs.loc)) ;
		vectr = DirectionVector<float>(rhs.vectr.getX(), rhs.vectr.getY(), rhs.vectr.getZ(), loc) ;
		map->place<float>(loc, this, *defaultCheck<float>, true) ;
		vectr.updateAndNormalize() ;

		allGameObjects->push_back(this) ;

		IDs++ ;
	}
	return *this ;
}

GameObject & GameObject::operator=(GameObject && rhs) {

	{
	/* Debug code */
	stringstream ss ;
	ss << "Warning: GameObject assignment operator overload (move) called." << '\n' ;
	DebugOutput << ss.rdbuf() ;
	/* End Debug code */
	}

	if (this != &rhs) {
		if (this->gthread != nullptr) { 
			delete this->gthread ;
		}
		delete this->hasThread ;
		
		this->hasThread = rhs.hasThread ;
		this->gthread = rhs.gthread ;
		
		this->textureImageFile = std::move(rhs.textureImageFile) ;
		this->texture = rhs.texture ; /* No initGraphicsData() for move operations, just steal from other */
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
		rhs.gthread = nullptr ;
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

void GameObject::initGraphicsData(bool overrideCurrentTexture, float sizeModifier) {

	//set texture
	if ((texture == nullptr) || (overrideCurrentTexture)) {
		SDL_Texture * tex = nullptr ;
		tex = AssetFileIO::getTextureFromFilename(GameState::getMainRenderer(), getImageFile(), getType()) ;

		if (tex == nullptr) {
			stringstream ss ;
			ss << "Load texture failed." << '\n' ;
			ss << SDL_GetError() << '\n' ;
			cerr << ss.rdbuf() ;
			throw exception() ;
		}

		this->setTexture(tex) ;
	}

	//set size
	int tempW  ; 
	int tempH  ;
	
	SDL_QueryTexture(texture, NULL, NULL, &tempW, &tempH) ; //init local size with size of texture
	size.setSize(tempW, tempH, sizeModifier) ; //assign new size to this GameObject
}

void GameObject::checkForMarkedDeletions() { //will run on own thread
	while (GLOBAL_CONTINUE_SIGNAL) {
		for (auto i = 0 ; i < allGameObjects->size() ; i++) {
			if ((allGameObjects->at(i) != nullptr) && (allGameObjects->at(i)->markedForDeletion)) {
				allGameObjects->at(i)->endThreading(false) ;
				delete allGameObjects->at(i) ;
				allGameObjects->at(i) = nullptr ;
			}
		}
	}
}

void GameObject::eraseByID(unsigned ID) {
	/* Try a shortcut first (this will only work if the Game o's were pushed onto the vector in the order of their creation) */
	if (allGameObjects->at(ID)->getID() == ID) {
		allGameObjects->at(ID) = nullptr ;
	}
	else {
		for (auto i = 0 ; i < allGameObjects->size() ; i++) {
			if (((allGameObjects->at(i)) != nullptr) && ((allGameObjects->at(i)->ID) == ID)) {
				allGameObjects->at(i) = nullptr ;
				break ;
			}
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
	this->gthread = new thread(functionPointer, this) ;
	*(this->hasThread) = true ;
	pair<thread *, GameObject *> threadPair = pair<thread *, GameObject *>(this->gthread, this) ;
	GameObject::allThreads->push_back(threadPair) ;
	if (wait) {
		gthread->join() ;
		allThreads->pop_back() ;
		delete this->gthread ;
		this->gthread = nullptr ;
	}
}

void GameObject::endThreading(bool join) {
	*(this->hasThread) = false ;
	if (join) {
		gthread->join() ;
	}
	for (auto i = 0 ; i < allThreads->size() ; i++) {
		if (get<0>(allThreads->at(i)) == this->gthread) {
			get<0>(allThreads->at(i)) = nullptr ;
		}
	}
	//delete this->gthread ;
	//this->gthread = nullptr ;
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
    
    to->checkBounds(*defaultCheck<float>, size.getWidth(), size.getHeight()) ;
	loc->setAll(*to) ;
	map->place<float>(loc, this, *defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	
	{
	/* Debug code */
	/*
	stringstream ss ;
	ss << "Current size of loc archive: " << loc->getHistory()->size() << '\n' ;
	DebugOutput << ss.rdbuf() ;
	*/
	/* end debug */
	}
}

void GameObject::moveTo(Position<float> to) {
    
	map->erase<float>(getPosition()) ;
    
    to.checkBounds(*defaultCheck<float>, size.getWidth(), size.getHeight()) ;
	loc->setAll(to) ;
	map->place<float>(loc, this, *defaultCheck<float>, true) ;
	vectr.updateAndNormalize() ;
	
	{
	/* Debug code */
	/*
	stringstream ss ;
	ss << "Current size of loc archive: " << loc->getHistory()->size() << '\n' ;
	DebugOutput << ss.rdbuf() ;
	*/
	/* end debug */
	}
}

void GameObject::jump() {
	vectr.normalize() ;
	Position<float> next = DirectionVector<float>::calculateNextPosition(vectr, 10.0) ;
	moveTo(std::move(next)) ;
}

void GameObject::moveSameDirection() {

	vectr.normalize() ;
	Position<float> next = DirectionVector<float>::calculateNextPosition(vectr, 1.0) ;

	if (next.overXBounds(*defaultCheck<float>)) {
		next = DirectionVector<float>::calculateReverseXPosition(vectr, 1.0, *defaultCheck<float>) ;
	}
	if (next.overYBounds(*defaultCheck<float>)) {
		next = DirectionVector<float>::calculateReverseYPosition(vectr, 1.0, *defaultCheck<float>) ;
	}

	moveTo(std::move(next)) ;
}

void GameObject::moveNewDirection(DirectionVector<float> & newDirection) {

	newDirection.normalize() ;
	auto next = DirectionVector<float>::calculateNextPosition(newDirection, loc, 1.0, *defaultCheck<float>) ;
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

SDL_Texture * GameObject::getTexture() const {
	if (visible) {
		return texture ;
	}
	else {
		return nullptr ;
	}
}

void GameObject::timedTurnInvisible(std::chrono::nanoseconds nano) {
	
	visible = false ;
	
	auto invisTimer = [&] {
		this_thread::sleep_for(nano) ;
		this->setVisible() ;
	} ;
	
	std::thread thr(invisTimer) ;
	thr.detach() ;
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





