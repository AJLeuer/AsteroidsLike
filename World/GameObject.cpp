//
//  GameObject.cpp
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#include "GameObject.h"

/* starts at 1 (0 is a special case */
unsigned GameObject::IDs = 0 ;

bool GameObject::map_is_init = false ;

vector<GameObject *> * GameObject::allGameObjects = new vector<GameObject*>() ;


GameMap<GameObject> * GameObject::map = new GameMap<GameObject>(globalMaxX(), globalMaxY()) ;

FastRand<int> GameObject::goRand(FastRand<int>(0, INT_MAX));


GameObject::GameObject() :
	ID(IDs),
	size(Size<int>()),
	loc(0.0, 0.0, 0.0, BoundsCheck<float>::defaultCheck)
{
	IDs++ ;
    
    outputData = OutputData<float, int>(FastRand<int>::defaultRandom, & loc, 1.0, PositionType::worldPosition) ;
    vectr = Vectr<float>(& loc, false) ;
    
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}

	allGameObjects->push_back(this) ;
    
    loc.checkBounds(BoundsCheck<float>::defaultCheck, size.getWidth(), size.getHeight()) ;
	map->place<float>( & loc, this, BoundsCheck<float>::defaultCheck) ;
	vectr.updateAndNormalize() ;
	/* No graphics data initialization here */
}

GameObject::GameObject(const GameObject & other) :
	ID(IDs),
    outputData(other.outputData),
	size(Size<int>()),
	loc(other.loc),
	vectr(other.vectr)
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
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
    loc.checkBounds(BoundsCheck<float>::defaultCheck, size.getWidth(), size.getHeight()) ;
	map->place<float>(& loc, this, BoundsCheck<float>::defaultCheck) ;
	vectr.updateAndNormalize() ;
	
	allGameObjects->push_back(this) ;

	
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
	ID(other.ID),
    outputData(std::move(other.outputData)), /* No initGraphicsData() for move operations, just steal from other */
	size(std::move(other.size)),
    loc(std::move(other.loc)),
	vectr(std::move(other.vectr))
{
	{
	/* debug */
	*(Debug::debugOutput) << "Warning: move constructor called. \n" ;
	/*end debug */
	}
	
	/* don't need to incr IDs */
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
	/* There's already references to us on the map and in 
	 allGameObjects, don't need to add us again */
	
	other.ID = -1 ;
}


GameObject::GameObject(const AssetFile & imageFile, float sizeModifier, const Position<float> & loc_, bool visible, bool monitorVelocity) :
	ID(IDs),
	size(Size<int>()),
	loc(loc_, BoundsCheck<float>::defaultCheck)
{
	IDs++ ;
    
    outputData = OutputData<float, int>(imageFile, &loc, sizeModifier, PositionType::worldPosition) ;
	vectr = Vectr<float>(& loc, monitorVelocity) ;
    
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
    
    loc.checkBounds(BoundsCheck<float>::defaultCheck, size.getWidth(), size.getHeight()) ;
	map->place(& loc, this, BoundsCheck<float>::defaultCheck) ;
	vectr.updateAndNormalize() ;
	setVisibility(visible) ;
}

GameObject::GameObject(FastRand<int> rand) :
	ID(IDs),
	outputData(rand, &loc, 1.0, PositionType::worldPosition),
	size(Size<int>()),
	loc(rand, BoundsCheck<float>::defaultCheck),
	vectr(& loc, true)
{
	IDs++ ;

	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	allGameObjects->push_back(this) ;
    
    loc.checkBounds(BoundsCheck<float>::defaultCheck, size.getWidth(), size.getHeight()) ;
	map->place<float>(& loc, this, BoundsCheck<float>::defaultCheck) ;
	vectr.updateAndNormalize() ;
	
	FastRand<float> randSizeMod(0.5, 1.0) ;
}


GameObject::~GameObject() {
	
	eraseByID(this->ID) ;
	
	map->erase(& loc, this) ;

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

		/* Keep ID the same */
        this->outputData = rhs.outputData ;

        map->erase(& loc, this) ;

        loc = rhs.loc ;
		vectr = Vectr<float>(rhs.vectr.getX(), rhs.vectr.getY(), rhs.vectr.getZ(), &loc) ;
        
		map->place(& loc, this, BoundsCheck<float>::defaultCheck) ;
		vectr.updateAndNormalize() ;
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
		
		this->ID = rhs.ID ;
		this->outputData = std::move(rhs.outputData) ; /* No initGraphicsData() for move operations, just steal from other */
		this->size = std::move(rhs.size) ;
		
        this->loc = std::move(rhs.loc) ;
		this->vectr = std::move(rhs.vectr) ;
		
		rhs.ID = -1 ;
	}
	return *this ;
}

void GameObject::operator()() {
	defaultBehaviors() ;
}

void GameObject::operator()(GameObject * sentObject) {

}

bool GameObject::operator==(GameObject & other) const {
	if (this->ID == other.ID) {
		return true ;
	}
	else {
		return false ;
	}
}


void GameObject::checkForMarkedDeletions() { //will run on own thread
	while (GLOBAL_CONTINUE_FLAG) {
		for (auto i = 0 ; i < allGameObjects->size() ; i++) {
			if ((allGameObjects->at(i) != nullptr) && (allGameObjects->at(i)->markedForDeletion)) {
				delete allGameObjects->at(i) ;
				allGameObjects->at(i) = nullptr ;
			}
		}
		this_thread::sleep_for(defaultSleepTime) ;
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

void GameObject::notify() {
	//todo
}

void GameObject::passMessage(Message * message, GameObject & recipient) {
	//todo
}

void GameObject::update() {
	//mainly used by inheriting classes, see their implementations
}

void GameObject::textDescription(ostream * writeTo) const {
	stringstream ss ;
	ss << "GameObject ID#: " << this->ID << endl ;

    ss << "Current Position: " << loc.toString() << endl ;

	*writeTo << ss.rdbuf() ;
}

void GameObject::moveTo(Position<float> * to) {
	
    map->erase(& loc, this) ;
        
    to->checkBounds(BoundsCheck<float>::defaultCheck, size.getWidth(), size.getHeight()) ;
    loc.setAll(*to) ;
    map->place(& loc, this, BoundsCheck<float>::defaultCheck) ;
    vectr.updateAndNormalize() ;

	
	{
	/* Debug code */
	//stringstream ss ;
	//ss << "Current size of loc archive: " << loc.getHistory()->size() << '\n' ;
	//DebugOutput << ss.rdbuf() ;
	/* end debug */
	}
}

void GameObject::moveTo(Position<float> to) {
	
    map->erase(& loc, this) ;
        
    to.checkBounds(BoundsCheck<float>::defaultCheck, size.getWidth(), size.getHeight()) ;
    loc.setAll(to) ;
    map->place(& loc, this, BoundsCheck<float>::defaultCheck) ;
    vectr.updateAndNormalize() ;


	/* Debug code */
	//stringstream ss ;
	//ss << "Current size of loc archive: " << loc.getHistory()->size() << '\n' ;
	//DebugOutput << ss.rdbuf() ;
	/* end debug */
}

void GameObject::moveUp() {
    Vectr<float> up(UP, false) ;
    moveNewDirection(up) ;
}

 void GameObject::moveDown() {
    Vectr<float> down(DOWN, false) ;
    moveNewDirection(down) ;
}

 void GameObject::moveRight() {
     Vectr<float> right(RIGHT, false) ;
     moveNewDirection(right) ;
}

 void GameObject::moveLeft() {
     Vectr<float> left(LEFT, false) ;
     moveNewDirection(left) ;
}


void GameObject::moveRandomDirection() {
	
	float x = chooseAtRand(1.0, -1.0) ;
	
	float y = chooseAtRand(1.0, -1.0) ;
	
	Vectr<float> newVector(x, y, 0, false) ;
	
	moveNewDirection(newVector) ;
}

void GameObject::jump() {
	vectr.normalize() ;
	Position<float> next = Vectr<float>::calculateNextPosition(vectr, 10.0) ;
    timedTurnInvisible(std::chrono::nanoseconds(64000000)) ;
	moveTo(std::move(next)) ;
}

void GameObject::move(float distanceModifier) {

	vectr.normalize() ;
	Position<float> next = Vectr<float>::calculateNextPosition(vectr, distanceModifier) ;

	if (next.overXBounds(&BoundsCheck<float>::defaultCheck)) {
		next.setX(loc.getX()) ;
	}
	if (next.overYBounds(&BoundsCheck<float>::defaultCheck)) {
		next.setY(loc.getY()) ;
	}

	moveTo(next) ;
}

void GameObject::moveNewDirection(Vectr<float> & newDirection) {

	newDirection.normalize() ;
    vectr += newDirection ;
	
	move() ;
}

void GameObject::wander() {
	
	vectr.normalize() ;
	
	auto dist = vectr.getLastMoveDistance() ;
	
	Position<float> next = Vectr<float>::calculateNextPosition(vectr, defaultMoveDistance<float>) ;
	
	if (next.overXBounds(&BoundsCheck<float>::defaultCheck)) {
		next = Vectr<float>::calculateReverseXPosition(vectr, 1.0, BoundsCheck<float>::defaultCheck) ;
	}
	if (next.overYBounds(&BoundsCheck<float>::defaultCheck)) {
		next = Vectr<float>::calculateReverseYPosition(vectr, 1.0, BoundsCheck<float>::defaultCheck) ;
	}
	
	moveTo(std::move(next)) ;
}

void GameObject::defaultBehaviors() {
	aiBehaviors() ;
}

void GameObject::aiBehaviors() {
	wander() ;
	//todo add more behaviors
}


void GameObject::attack(GameObject * enemy) {
	
}

void GameObject::findNearbyAlly(int searchDistanceX, int searchDistanceY) {
    
	vector<GameObject *> * nearby = map->findNearby<float>(& loc, searchDistanceX, searchDistanceY) ;
	
	if ((nearby != nullptr) && (nearby->size() > 0)) {
		allyWith(nearby->at(0)) ;
	}
}

void GameObject::allyWith(const GameObject * other) {
	this->ally = other ;
}

void GameObject::setImageFile(string imageFileName) {
	outputData.setAssetFile(imageFileName) ;
}

const AssetFile * GameObject::getImageFile() const {
	return outputData.getAssetFile() ;
}

Texture * GameObject::getTexture() const {
	return outputData.getTexture() ;
}

void GameObject::timedTurnInvisible(std::chrono::nanoseconds nano) {
	
	this->setVisibility(false) ;
	
	auto invisTimer = [=] {
		this_thread::sleep_for(nano) ;
		this->setVisibility(true) ;
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





