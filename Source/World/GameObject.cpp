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

Randm<int> GameObject::goRand(Randm<int>(0, INT_MAX));


GameObject::GameObject() :
	ID(IDs),
	graphicsData(new GraphicsData<float, int>(Randm<int>::defaultRandom, new Vect<float>(0.0, 0.0, BoundsCheck<float>::defaultCheck), randomEnumeration<AssetType>(9), PositionType::worldPosition, SafeBoolean::t, SafeBoolean::f, SafeBoolean::t, SafeBoolean::t))
{
	IDs++ ;
    
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}

	allGameObjects->push_back(this) ;
    placeOnMap() ;
    
	graphicsData->updateAndNormalizeVector() ;
	/* No graphics data initialization here */
	
}

GameObject::GameObject(const GameObject & other) :
	ID(IDs),
    graphicsData(new GraphicsData<float, int>(*other.graphicsData)),
    /* Don't copy other's onMap */
	previousActionUndoFunctions(new stack<function<void()>>(* other.previousActionUndoFunctions))
{

	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
	placeOnMap() ;
 }

GameObject::GameObject(GameObject && other) :
	ID(other.ID),
    graphicsData(other.graphicsData), /* No initGraphicsData() for move operations, just steal from other */
    /* Don't copy other's onMap */
	previousActionUndoFunctions(other.previousActionUndoFunctions)
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
	
	allGameObjects->push_back(this) ;
	placeOnMap() ;
	
	/* Cannot erase by id, since this and other share the same ids. Instead, we have to call
	   eraseByReference */
	eraseByReference(other) ;
	other.eraseFromMap() ;
    other.graphicsData = nullptr ;
	other.ID = -1 ;
	other.previousActionUndoFunctions = nullptr ;
	other.wasMoved = true ;
}


GameObject::GameObject(const AssetFile & imageFile, float sizeModifier, const Vect<float> & loc_, const Angle rotation, bool visible, SafeBoolean monitorVelocity,
					   SafeBoolean boundsChecking, SafeBoolean collisionDetection) :

	ID(IDs),
	graphicsData(new GraphicsData<float, int>(imageFile, new Vect<float>(loc_, BoundsCheck<float>::defaultCheck), rotation, sizeModifier, PositionType::worldPosition, (SafeBoolean)visible, monitorVelocity, boundsChecking, collisionDetection)) /* can't be properly initialized yet */
{
	IDs++ ;
    
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
    
	placeOnMap() ;

}

GameObject::GameObject(Randm<int> & rand, AssetType type, bool visible) :
	ID(IDs),
	graphicsData(new GraphicsData<float, int>(rand, new Vect<float>(rand, BoundsCheck<float>::defaultCheck), randomEnumeration<AssetType>(9), PositionType::worldPosition, (SafeBoolean)visible, SafeBoolean::f, SafeBoolean::t, SafeBoolean::t))
{
	IDs++ ;
	
	if (!map_is_init) {
		map = new GameMap<GameObject>(globalMaxX()+1, globalMaxY()+1) ;
		map_is_init = true ;
	}
	
	allGameObjects->push_back(this) ;
    
	placeOnMap() ;
	
	Randm<float> randSizeMod(0.5, 1.0) ;
}


GameObject::~GameObject() {
	
	mtx.lock() ;
	
	if (wasMoved == false) { //no need to destruct if this was the argument to a move operation
		eraseByID(this->ID) ;
		
		eraseFromMap() ;
		
		graphicsData->markForDeletion() ;
		
		delete previousActionUndoFunctions ;
	}
	
	mtx.unlock() ;
}

void GameObject::operator()() {
	doDefaultBehavior() ;
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

bool GameObject::operator == (const GameObject & other) const {
	if (this->ID == other.ID) {
		return true ;
	}
	else {
		return false ;
	}
}

void GameObject::eraseByID(int ID) {
	/* Try a shortcut first (this will only work if the Game o's were pushed onto the vector in the order of their creation) */
	if (ID == -1) {
		//then the GameObject was already erased by ID during a move op, so just return immediately
		return ;
	}

	//else...
	if (ID <= (allGameObjects->size() - 1)) {
		if ((allGameObjects->at(ID) != nullptr) && (allGameObjects->at(ID)->ID == ID)) {
			allGameObjects->at(ID) = nullptr ;
		}
	}
	else { //we have to do it the hard way...
		for (auto i = 0 ; i < allGameObjects->size() ; i++) {
			if ((allGameObjects->at(i) != nullptr) && (allGameObjects->at(i)->ID == ID)) {
				allGameObjects->at(i) = nullptr ;
				break ;
			}
		}
	}

	
}

void GameObject::eraseByReference(GameObject & reference) {
	
	for (auto i = 0 ; i < allGameObjects->size() ; i++) {
		
		/* No real need for null pointer checks here, since we're just comparing references, or assigning to null */
		if ( (*allGameObjects)[i] == & reference) {
			(*allGameObjects)[i] = nullptr ;
			break ;
		}
		
	}
}

void GameObject::reversePreviousAction() {
	if (previousActionUndoFunctions->size() > 0) {
		
		function<void()> undoMostRecentAction = previousActionUndoFunctions->top() ;
		previousActionUndoFunctions->pop() ; /* pop the undo counterpart to the most recent recent action,
											  removing it from the stack and ensuring it won't be called again */
		
		undoMostRecentAction() ; //call the reversal function, undoing the most recent action
	}
}


void GameObject::update() {
	//mainly used by inheriting classes, see their implementations
}

void GameObject::textDescription(ostream * writeTo) const {
	stringstream ss ;
    
	ss << "GameObject ID#: " << this->ID << endl ;

    ss << "Current Vect: " << getPosition()->toString() << endl ;

	*writeTo << ss.rdbuf() ;
}

void GameObject::placeOnMap() {
    map->place<float>(getPosition(), this) ;
    onMap = true ;
}

void GameObject::moveOnMap(const Vect<float> * toNewLoc) {
    if ((onMap == true) && (toNewLoc->overBounds(map->mapBounds<float>()) == false)) {
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

void GameObject::move(VectorAndVelocity<float> & direction, float distanceModifier) {
    
    direction.normalize() ;
    
    direction.rotateVectorAndOrientation(*graphicsData->getVector()->getOrientation()) ; /* rotate new direction to match our own orientation */
    
    *graphicsData->getRawMutableVector() += direction ;
    
    move(distanceModifier) ;
}

void GameObject::move(float distanceModifier) {
    graphicsData->getRawMutableVector()->normalize() ;
    /* Automatically find our next move based on our current vector */
    Vect<float> next = graphicsData->getRawMutableVector()->calculateNextPosition(distanceModifier) ;
    moveTo(next) ;
}


void GameObject::moveTo(Vect<float> to) {
    moveTo(&to) ;
}

void GameObject::moveTo(Vect<float> * to) {
	
	/* copy current position for undoing later */
	const Vect<float> oldPosition = * this->getPosition() ;
    
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
	
	/* create the undo action for resetting position to it's previous state */
	auto undo = [this, oldPosition] () { //copy variables by value
		auto thisPointerCopy = this ; //debug var
		this->moveOnMap(& oldPosition) ;
		graphicsData->getRawMutablePosition()->setAll(oldPosition) ;
		graphicsData->updateAndNormalizeVector() ;
	} ;
	
	previousActionUndoFunctions->push(undo) ;
}

void GameObject::moveUp() {
	VectorAndVelocity<float> up(UP, SafeBoolean::f) ;
    move(up) ;
}

 void GameObject::moveDown() {
    VectorAndVelocity<float> down(DOWN, SafeBoolean::f) ;
    move(down) ;
}

 void GameObject::moveRight() {
     VectorAndVelocity<float> right(RIGHT, SafeBoolean::f) ;
     move(right) ;
}

 void GameObject::moveLeft() {
     VectorAndVelocity<float> left(LEFT, SafeBoolean::f) ;
     move(left) ;
}

void GameObject::orientationDependentLeftRightMove() {
    if ((graphicsData->copyOrientation() >= 270.0f) || (graphicsData->copyOrientation() <= 90.0f)) {
        moveLeft() ;
    }
    else { /* if > 90 and  < 270... */
        moveRight() ;
    }
}

void GameObject::orientationDependentRightLeftMove() {
    if ((graphicsData->copyOrientation() >= 270.0f) || (graphicsData->copyOrientation() <= 90.0f)) {
        moveRight() ;
    }
    else { /* if > 90 and  < 270... */
        moveLeft() ;
    }
}

void GameObject::rotateClockwise() {
	
	graphicsData->rotateClockwise() ;
	
	/* create the undo action for resetting position to it's previous state */
	auto undo = [this] () { //copy variables by value
		graphicsData->rotateCounterClockwise() ;
	} ;
	
	previousActionUndoFunctions->push(undo) ;
}

void GameObject::rotateCounterClockwise() {
	
	graphicsData->rotateCounterClockwise() ;
	
	/* create the undo action for resetting position to it's previous state */
	auto undo = [this] () { //copy variables by value
		graphicsData->rotateClockwise() ;
	} ;
	
	previousActionUndoFunctions->push(undo) ;
} ;

void GameObject::moveX(float x) {
    this->moveTo({x, graphicsData->getPosition().getY()}) ;
}

void GameObject::moveY(float y) {
    this->moveTo({graphicsData->getPosition().getX(), y}) ;
}


void GameObject::moveRandomDirection() {
    
    Randm<float> randVectorIniter(-10.0, 10.0) ;
	
	float x = randVectorIniter.nextValue() ;
	
	float y = randVectorIniter.nextValue() ;
	
	VectorAndVelocity<float> newVector(x, y, 0, SafeBoolean::f) ;
	
	move(newVector) ;
}

void GameObject::jump() {
	graphicsData->getRawMutableVector()->normalize() ;
	Vect<float> next = graphicsData->getRawMutableVector()->calculateNextPosition(15.0) ;
    timedTurnInvisible(std::chrono::nanoseconds(64000000)) ;
	moveTo(std::move(next)) ;
}

void GameObject::wander() {
	
	graphicsData->getRawMutableVector()->normalize() ;
	
	auto dist = graphicsData->getVector()->getLastMoveDistance() ;
	
	Vect<float> next = graphicsData->getRawMutableVector()->calculateNextPosition(defaultMoveDistance<float>) ;
	
	if (next.overXBounds(&BoundsCheck<float>::defaultCheck)) {
		next = graphicsData->getRawMutableVector()->calculateReverseXPosition(1.0, BoundsCheck<float>::defaultCheck) ;
	}
	if (next.overYBounds(&BoundsCheck<float>::defaultCheck)) {
		next = graphicsData->getRawMutableVector()->calculateReverseYPosition(1.0, BoundsCheck<float>::defaultCheck) ;
	}
	
	moveTo(std::move(next)) ;
}

/* This function will really only apply to generic GameObject (like Asteroids and debris, etc.)
 All other inheriting classes will override this (usually with no-ops) */
void GameObject::doDefaultBehavior(bool initialCall) {

    if (initialCall) {
        moveRandomDirection() ;
    }
    else {
        wander() ;
    }
}

void GameObject::aiBehaviors() {
	wander() ;
	
	//todo add ai
}


void GameObject::attack(GameObject * enemy) {
	
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

bool GameObject::overBounds() {
    return graphicsData->overBounds() ;
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





