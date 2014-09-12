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
	outputData(), /* can't be properly initialized yet */
	pos(0.0, 0.0, BoundsCheck<float>::defaultCheck),
	vec(outputData.getRawMutableVector())
{
	IDs++ ;
    
	outputData.reinitializeMembers(FastRand<int>::defaultRandom, & this->pos, SafeBoolean::t, AssetType::asteroid, PositionType::worldPosition, true) ;
    
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}

	allGameObjects->push_back(this) ;
    
    pos.checkBounds(BoundsCheck<float>::defaultCheck, getSize()->getWidth(), getSize()->getHeight()) ;
    placeOnMap() ;
	outputData.updateAndNormalizeVector() ;
	/* No graphics data initialization here */
}

GameObject::GameObject(const GameObject & other) :
	ID(IDs),
    outputData(other.outputData),
	pos(other.pos),
    onMap(other.onMap)
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
	
	vec = outputData.getRawMutableVector() ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
    pos.checkBounds(BoundsCheck<float>::defaultCheck, getSize()->getWidth(), getSize()->getHeight()) ;
	placeOnMap() ;
	outputData.updateAndNormalizeVector() ;
	
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
    pos(std::move(other.pos)),
	vec(other.vec),
    onMap(other.onMap)
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
	
	other.vec = nullptr ;
	other.ID = -1 ;
}


GameObject::GameObject(const AssetFile & imageFile, float sizeModifier, const Position<float> & loc_, const Angle rotation, bool visible, SafeBoolean monitorVelocity) :
	ID(IDs),
	outputData(), /* can't be properly initialized yet */
	pos(loc_, BoundsCheck<float>::defaultCheck),
	vec(outputData.getRawMutableVector())
{
	IDs++ ;
    
    outputData.reinitializeMembers(imageFile, &pos, monitorVelocity, rotation, sizeModifier, PositionType::worldPosition, visible) ;
    
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
    
    pos.checkBounds(BoundsCheck<float>::defaultCheck, getSize()->getWidth(), getSize()->getHeight()) ;
	placeOnMap() ;
	outputData.updateAndNormalizeVector() ;
	setVisibility(visible) ;
}

GameObject::GameObject(FastRand<int> & rand, AssetType type, bool visible) :
	ID(IDs),
	outputData(),
	pos(rand, BoundsCheck<float>::defaultCheck),
	vec(outputData.getRawMutableVector())
{
	IDs++ ;
	
	outputData.reinitializeMembers(rand, &pos, SafeBoolean::t, type, PositionType::worldPosition, visible) ;

	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
    
    pos.checkBounds(BoundsCheck<float>::defaultCheck, getSize()->getWidth(), getSize()->getHeight()) ;
	placeOnMap() ;
	outputData.updateAndNormalizeVector() ;
	
	FastRand<float> randSizeMod(0.5, 1.0) ;
}


GameObject::~GameObject() {
    
	
	eraseByID(this->ID) ;
	
	eraseFromMap() ;

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
        eraseFromMap() ;

        pos = rhs.pos ;
		
		outputData.copy(rhs.outputData) ; //give outputdata our new position as well
		
		vec = outputData.getRawMutableVector() ;
        
        onMap = rhs.onMap ;
		
        placeOnMap() ;
		outputData.updateAndNormalizeVector() ;
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
		
        this->pos = std::move(rhs.pos) ;
		
		outputData.moveCopy(std::move(rhs.outputData)) ;
        
        onMap = rhs.onMap ;
		
		vec = outputData.getRawMutableVector() ;
		
		rhs.vec = nullptr ;
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

bool GameObject::operator==(const GameObject & other) const {
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
	else { //we have to do it the hard way...
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

    ss << "Current Position: " << pos.toString() << endl ;

	*writeTo << ss.rdbuf() ;
}

void GameObject::placeOnMap() {
    map->place<float>(& (pos), this) ;
    onMap = true ;
}

void GameObject::moveOnMap(const Position<float> * toNewLoc) {
    if ((onMap == true) && (toNewLoc->overBounds(map->mapBounds<float>(), outputData.getSize().getWidth(), outputData.getSize().getHeight()) == false)) {
        map->map_move(& pos, toNewLoc, this, onMap) ;
        onMap = true ;
    }
    else if (onMap == false) {
        //do nothing
    }
    else { //if it's moved off the map, mark it as such
        eraseFromMap() ;
        markForDeletion() ;
    }
}

void GameObject::eraseFromMap() {
    if (onMap == true) {
        map->erase(& pos, this) ;
        onMap = false ;
    }
}

void GameObject::moveTo(const Position<float> * to) {
    
    moveOnMap(to) ;

    pos.setAll(*to) ;

	outputData.updateAndNormalizeVector() ;
	
	{
	/* Debug code */
	stringstream ss ;
	ss << "Current size of pos archive: " << pos.getHistory()->size() << '\n' ;
	DebugOutput << ss.rdbuf() ;
	/* end debug */
	}
}

void GameObject::moveTo(const Position<float> to) {
	moveTo(&to) ;
}

void GameObject::moveUp() {
	Vectr<float> up(UP, SafeBoolean::f) ;
    moveNewDirection(up) ;
}

 void GameObject::moveDown() {
    Vectr<float> down(DOWN, SafeBoolean::f) ;
    moveNewDirection(down) ;
}

 void GameObject::moveRight() {
     Vectr<float> right(RIGHT, SafeBoolean::f) ;
     moveNewDirection(right) ;
}

 void GameObject::moveLeft() {
     Vectr<float> left(LEFT, SafeBoolean::f) ;
     moveNewDirection(left) ;
}

void GameObject::orientationDependentLeftRightMove() {
    if ((outputData.getOrientation()->val_const() >= 270) || (outputData.getOrientation()->val_const() <= 90)) {
        moveLeft() ;
    }
    else { /* if > 90 and  < 270... */
        moveRight() ;
    }
}

void GameObject::orientationDependentRightLeftMove() {
    if ((outputData.getOrientation()->val_const() >= 270) || (outputData.getOrientation()->val_const() <= 90)) {
        moveRight() ;
    }
    else { /* if > 90 and  < 270... */
        moveLeft() ;
    }
}


void GameObject::moveRandomDirection() {
	
	float x = chooseAtRand(1.0, -1.0) ;
	
	float y = chooseAtRand(1.0, -1.0) ;
	
	Vectr<float> newVector(x, y, 0, SafeBoolean::f) ;
	
	moveNewDirection(newVector) ;
}

void GameObject::jump() {
	vec->normalize() ;
	Position<float> next = Vectr<float>::calculateNextPosition(*vec, 15.0) ;
    timedTurnInvisible(std::chrono::nanoseconds(64000000)) ;
	moveTo(std::move(next)) ;
}

void GameObject::move() {
    float distanceModifier = defaultMoveDistance<float> ;
    const BoundsCheck<float> * bc = &(BoundsCheck<float>::defaultCheck) ;
	
    move(distanceModifier, bc) ;
}

void GameObject::move(float distanceModifier, const BoundsCheck<float> * bc) {
	vec->normalize() ;
	Position<float> next = Vectr<float>::calculateNextPosition(*vec, distanceModifier) ;
	
	if ((bc != nullptr) && (outputData.isBoundsChecked())) {
		if (next.overXBounds(bc)) {
			next.setX(pos.getX()) ;
		}
		if (next.overYBounds(bc)) {
			next.setY(pos.getY()) ;
		}
	}
	moveTo(next) ;
}

PastPositionAndTimeDifferential GameObject::getReverseMove() {
    PastPositionAndTimeDifferential lastPos = pos.popLastArchivedPosition();
    return lastPos ;
	/* else do nothing, just stay frozen in place */
}

void GameObject::moveNewDirection(Vectr<float> & newDirection, float distanceModifier, const BoundsCheck<float> * bc) {

	newDirection.normalize() ;
	
	newDirection.rotateAbs(*vec->getOrientation()) ; /* rotate new direction to match our own orientation */

    *vec += newDirection ;
	
	move(distanceModifier, bc) ;
}

void GameObject::wander() {
	
	vec->normalize() ;
	
	auto dist = vec->getLastMoveDistance() ;
	
	Position<float> next = Vectr<float>::calculateNextPosition(*vec, defaultMoveDistance<float>) ;
	
	if (next.overXBounds(&BoundsCheck<float>::defaultCheck)) {
		next = Vectr<float>::calculateReverseXPosition(*vec, 1.0, BoundsCheck<float>::defaultCheck) ;
	}
	if (next.overYBounds(&BoundsCheck<float>::defaultCheck)) {
		next = Vectr<float>::calculateReverseYPosition(*vec, 1.0, BoundsCheck<float>::defaultCheck) ;
	}
	
	moveTo(std::move(next)) ;
}

/* This function will really only apply to generic GameObject (like Asteroids and debris, etc.)
 All other inheriting classes will override this (usually with no-ops) */
void GameObject::defaultBehaviors() {
    
    static unsigned calls = 0 ;
    
    if (calls == 0) {
        moveRandomDirection() ;
        update() ;
    }
    else if (calls > 0) {
        aiBehaviors() ;
    }
    
    calls++ ;
}

void GameObject::aiBehaviors() {
	wander() ;
	
	//todo add more behaviors
}


void GameObject::attack(GameObject * enemy) {
	
}

void GameObject::findNearbyAlly(int searchDistanceX, int searchDistanceY) {
    
	vector<const GameObject *> * nearby = map->findNearby<float>(& pos, searchDistanceX, searchDistanceY) ;
	
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

void GameObject::fire() {
	//no-op
}





