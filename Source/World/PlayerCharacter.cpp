//
//  PlayerCharacter.cpp
//  SpriteFight
//
//  Created by aleuer2 on 6/30/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "PlayerCharacter.h"

/**
 * Constructs a default PlayerCharacter.
 */
PlayerCharacter::PlayerCharacter() :
    Character(),
    weapon(AssetFile::projectileImageFilenames->at(2), getSize()->getModifier()){}


/**
 * Copy constructor for PlayerCharacter
 *
 * @param other The PlayerCharacter to be copied
 */
PlayerCharacter::PlayerCharacter(const PlayerCharacter & other) :
    Character(other), weapon(other.weapon),
    moveFlag(other.moveFlag) {}

/**
 * Move constructor for PlayerCharacter
 *
 * @param other The PlayerCharacter to be moved
 */
PlayerCharacter::PlayerCharacter(PlayerCharacter && other) :
    Character(other), weapon(std::move(other.weapon)),
    moveFlag(other.moveFlag) {}

/**
 * Constructs a PlayerCharacter based on the arguments given
 *
 * @param name The name of this PlayerCharacter
 * @param alive Whether this PlayerCharacter is dead or alive
 * @param state The CharacterState of this PlayerCharacter
 * @param health The Health of this PlayerCharacter
 * @param damage The Damage capability of this PlayerCharacter
 */
PlayerCharacter::PlayerCharacter(const AssetFile & imageFile, float size, const Vect<float> & loc,
								 const Angle rotation, string name, Reaction reaction, DoA alive, CharacterState state,
								 unsigned health, unsigned damage, SafeBoolean monitorVelocity, bool boundsChecking, const AssetFile & projectileImageFile) :

								Character(imageFile, size, loc, rotation, name, reaction, alive, state, health, damage, monitorVelocity, boundsChecking),
    							weapon(projectileImageFile, getSize()->getModifier())
{
	
}


/**
 * Constructs a randomized PlayerCharacter. The client has to option to simply leave the argument randSeed as
 * 0, in which case the constructor will generate its own random number.
 *
 * @param randSeed A seed to initialize the random number generator
 */
PlayerCharacter::PlayerCharacter(Randm<int> rand, AssetType type) :
    Character(rand, type),
    weapon(AssetFile::projectileImageFilenames->at(2), this->getSize()->getModifier())
{
	
}


/**
 * Destructor for PlayerCharacter
 */
PlayerCharacter::~PlayerCharacter() {}


/**
 * Overloads operator() for PlayerCharacter. Possibly will be used to
 * call notify(). TBD.
 */
void PlayerCharacter::operator()() {
    //todo
}


/**
 * Overloads the overload of operator(). The actual implementation
 * and uses for this are still undecided.
 *
 * @param otherCharacter A reference to another PlayerCharacter
 */
void PlayerCharacter::operator()(GameObject * other) {
    //todo
}

void PlayerCharacter::move(VectorAndVelocity<float> & direction, float distanceModifier) {
	
	direction.normalize() ;
	
	direction.rotateVectorAndOrientation(getRawMutableVector()->copyOrientation()) ; /* rotate new direction to match our own orientation */
	
	*getRawMutableVector() += direction ;
	
	printPositition() ; /* Debug code */
	
	/* have to use this until we find a better way to stop moving more than once per loop */
	/* if moveFlag == true, then we've already been instructed to move this loop, so we don't need to
		do anything */
	if (moveFlag == false) {
		deferredCallbacks.push_back(std::pair<void (PlayerCharacter::*)(), PlayerCharacter *>(&GameObject::move, this)) ;
		moveFlag = true ;
	}
}

//using GameObject's implementation for now, may change later
void PlayerCharacter::doDefaultBehavior(bool initialCall) {
   /* Overriding this with nothing, since we are player-controlled, not AI-controlled */
}


void PlayerCharacter::fire() {
	auto gunX = copyPosition().getX() + (getSize()->getWidth() / 2) ;
	auto gunY = copyPosition().getY() + (getSize()->getWidth() / 2) ;
	
	Vect<float> gunPos { gunX, gunY} ;
	
    weapon.fire(gunPos, * getOrientation()) ;
}


void PlayerCharacter::fire(const Character * atEnemy) {}

void PlayerCharacter::jump() {
    GameObject::jump() ;
    #ifdef DEBUG_MODE
    printPositition() ;
    #endif
}

/* PlayerCharacter relies heavily on 
	update() to make sure that the player's
	character updates smoothly, not sporadically */
void PlayerCharacter::update() {
    for (long i = (deferredCallbacks.size() - 1) ; i >= 0 ; i--) {
        PlayerCharacter * pc = deferredCallbacks.at(i).second ;
        void (PlayerCharacter::*callBack)() = deferredCallbacks.at(i).first ;
        (pc->*callBack)() ;
		deferredCallbacks.pop_back() ;
    }
	
	/* move() is a special case in that it can only be called once per loop,
	 so we have an extra measure that we use in order to prevent that happenening (moveFlag)
	 and update() is in charge of resetting it. Note however that update doesn't actually
	 check to make sure move() wasn't added to defferredCallbacks more than once, PlayerCharacter's
	 movement functions are responsible for doing that*/
	moveFlag = false ;
}

/**
 * Writes a formatted text description of this PlayerCharacter into the desired output stream
 */
void PlayerCharacter::textDescription(ostream * writeTo) const {
	this->Character::textDescription(writeTo) ;
}

void PlayerCharacter::printPositition() {
	VectorAndVelocity<float> * vec = getRawMutableVector() ;
	if (*vec->getCurrent() != vec->getLast()) {
		stringstream ss ;
		ss << this->name << "'s current world position is: " << getPosition() ;
		auto screenPosition = translateToWindowCoords(*getPosition()) ;
		ss << this->name << "'s current screen position is: " << screenPosition << '\n' ;
		DebugOutput << ss.rdbuf() ;
	}
}



