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
	graphicsData(new GraphicsData<float, int>), /* can't be properly initialized yet */
	vec(graphicsData->getRawMutableVector())
{
	IDs++ ;
    
	graphicsData->reinitializeMembers(FastRand<int>::defaultRandom, new Pos2<float>(0.0, 0.0, BoundsCheck<float>::defaultCheck), SafeBoolean::t, AssetType::asteroid, PositionType::worldPosition, true) ;
    
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}

	allGameObjects->push_back(this) ;
    
    graphicsData->getRawMutablePosition()->checkBounds(BoundsCheck<float>::defaultCheck, getSize()->getWidth(), getSize()->getHeight()) ;
    placeOnMap() ;
	graphicsData->updateAndNormalizeVector() ;
	/* No graphics data initialization here */
}

GameObject::GameObject(const GameObject & other) :
	ID(IDs),
    graphicsData(new GraphicsData<float, int>(*other.graphicsData)),
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
	
	vec = graphicsData->getRawMutableVector() ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
    graphicsData->getRawMutablePosition()->checkBounds(BoundsCheck<float>::defaultCheck, getSize()->getWidth(), getSize()->getHeight()) ;
	placeOnMap() ;
	graphicsData->updateAndNormalizeVector() ;
	
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
    graphicsData(other.graphicsData), /* No initGraphicsData() for move operations, just steal from other */
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
	
    other.graphicsData = nullptr ;
	other.vec = nullptr ;
	other.ID = -1 ;
}


GameObject::GameObject(const AssetFile & imageFile, float sizeModifier, const Position<float> & loc_, const Angle rotation, bool visible, SafeBoolean monitorVelocity) :
	ID(IDs),
	graphicsData(new GraphicsData<float, int>()), /* can't be properly initialized yet */
	vec(graphicsData->getRawMutableVector())
{
	IDs++ ;
    
    graphicsData->reinitializeMembers(imageFile, new Pos2<float>(loc_, BoundsCheck<float>::defaultCheck), monitorVelocity, rotation, sizeModifier, PositionType::worldPosition, visible) ;
    
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
    
    graphicsData->getRawMutablePosition()->checkBounds(BoundsCheck<float>::defaultCheck, getSize()->getWidth(), getSize()->getHeight()) ;
	placeOnMap() ;
	graphicsData->updateAndNormalizeVector() ;
	setVisibility(visible) ;
}

GameObject::GameObject(FastRand<int> & rand, AssetType type, bool visible) :
	ID(IDs),
	graphicsData(new GraphicsData<float, int>()),
	vec(graphicsData->getRawMutableVector())
{
	IDs++ ;
	
	graphicsData->reinitializeMembers(rand, new Pos2<float>(rand, BoundsCheck<float>::defaultCheck), SafeBoolean::t, type, PositionType::worldPosition, visible) ;

	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
    
    graphicsData->getRawMutablePosition()->checkBounds(BoundsCheck<float>::defaultCheck, getSize()->getWidth(), getSize()->getHeight()) ;
	placeOnMap() ;
	graphicsData->updateAndNormalizeVector() ;
	
	FastRand<float> randSizeMod(0.5, 1.0) ;
}


GameObject::~GameObject() {
    
	eraseByID(this->ID) ;
	
	eraseFromMap() ;
    
    graphicsData->markForDeletion() ;
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

		graphicsData->copy(*rhs.graphicsData) ; //give outputdata our new position as well
		
		vec = graphicsData->getRawMutableVector() ;
        
        onMap = rhs.onMap ;
		
        placeOnMap() ;
		graphicsData->updateAndNormalizeVector() ;
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
		
		graphicsData = rhs.graphicsData ;
        
        onMap = rhs.onMap ;
		
		vec = graphicsData->getRawMutableVector() ;
		
        rhs.graphicsData = nullptr ;
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
    unsigned objectsDeleted = 0 ; //debug var, remove this
	while (GLOBAL_CONTINUE_FLAG) {
		for (auto i = 0 ; i < allGameObjects->size() ; i++) {
			if ((allGameObjects->at(i) != nullptr) && (allGameObjects->at(i)->isMarkedForDeletion())) {
				delete allGameObjects->at(i) ;
				allGameObjects->at(i) = nullptr ;
                objectsDeleted++ ;
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

    ss << "Current Position: " << getPosition()->toString() << endl ;

	*writeTo << ss.rdbuf() ;
}

void GameObject::placeOnMap() {
    map->place<float>(getPosition(), this) ;
    onMap = true ;
}

void GameObject::moveOnMap(const Position<float> * toNewLoc) {
    if ((onMap == true) && (toNewLoc->overBounds(map->mapBounds<float>(), graphicsData->getSize().getWidth(), graphicsData->getSize().getHeight()) == false)) {
        map->map_move(getPosition(), toNewLoc, this, onMap) ;
        onMap = true ;
    }
    else if (onMap == false) {
        //do nothing
    }
    else { //if it's moved off the map, mark it as such
        eraseFromMap() ;
    }
}

void GameObject::eraseFromMap() {
    if (onMap == true) {
        map->erase(getPosition(), this) ;
        onMap = false ;
    }
}

void GameObject::move() {
    float distanceModifier = defaultMoveDistance<float> ;
    move(distanceModifier) ;
}

void GameObject::move(float distanceModifier) {
    vec->normalize() ;
    /* Automatically find our next move based on our current vector */
    Position<float> next = Vectr<float>::calculateNextPosition(*vec, distanceModifier) ;
    moveTo(next) ;
}


void GameObject::moveTo(Position<float> to) {
    moveTo(&to) ;
}

void GameObject::moveTo(Position<float> * to) {
    
    /* If this object is bounds checked, do bounds checking */
    const BoundsCheck<float> * bc = graphicsData->getBoundsCheck() ;
    
    if (graphicsData->isBoundsChecked()) { //isBoundsChecked() will also check to make sure bc isn't null
        if (to->overXBounds(bc)) {
            to->setX(getPosition()->getX()) ;
        }
        if (to->overYBounds(bc)) {
            to->setY(getPosition()->getY()) ;
        }
    }
    
    /* then move the map representation of us */
    moveOnMap(to) ;

    /* next update our current position */
    graphicsData->getRawMutablePosition()->setAll(*to) ;
    
    /* Finally update and normalize the vector */
	graphicsData->updateAndNormalizeVector() ;
	
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
    if ((graphicsData->getOrientation()->val_const() >= 270) || (graphicsData->getOrientation()->val_const() <= 90)) {
        moveLeft() ;
    }
    else { /* if > 90 and  < 270... */
        moveRight() ;
    }
}

void GameObject::orientationDependentRightLeftMove() {
    if ((graphicsData->getOrientation()->val_const() >= 270) || (graphicsData->getOrientation()->val_const() <= 90)) {
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

PastPositionAndTimeDifferential GameObject::getReverseMove() {
    Pos2<float> * pos_hist = (Pos2<float> *)graphicsData->getRawMutablePosition() ;
    PastPositionAndTimeDifferential lastPos = pos_hist->popLastArchivedPosition();
    return lastPos ;
	/* else do nothing, just stay frozen in place */
}

void GameObject::moveNewDirection(Vectr<float> & newDirection, float distanceModifier) {

	newDirection.normalize() ;
	
	newDirection.rotateAbs(*vec->getOrientation()) ; /* rotate new direction to match our own orientation */

    *vec += newDirection ;
	
	move(distanceModifier) ;
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
    
	vector<const GameObject *> * nearby = map->findNearby<float>(getPosition(), searchDistanceX, searchDistanceY) ;
	
	if ((nearby != nullptr) && (nearby->size() > 0)) {
		allyWith(nearby->at(0)) ;
	}
}

void GameObject::allyWith(const GameObject * other) {
	this->ally = other ;
}

void GameObject::setImageFile(string imageFileName) {
	graphicsData->setAssetFile(imageFileName) ;
}

const AssetFile * GameObject::getImageFile() const {
	return graphicsData->getAssetFile() ;
}

Texture * GameObject::getTexture() const {
	return graphicsData->getTexture() ;
}

bool GameObject::overBounds(const BoundsCheck<float> & bc) {
    return getPosition()->overBounds(bc, getSize()->getWidth(), getSize()->getHeight()) ;
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





