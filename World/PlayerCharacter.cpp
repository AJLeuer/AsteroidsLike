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
    weapon(AssetFile::projectileImageFilenames->at(2), size.getModifier(), & loc){}


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
PlayerCharacter::PlayerCharacter(const AssetFile & imageFile, float size, const Position<float> & loc, string name, Reaction reaction, DoA alive, CharacterState state, unsigned health, unsigned damage, bool monitorVelocity, const AssetFile & projectileImageFile) :
    Character(imageFile, size, loc, name, reaction, alive, state, health, damage, monitorVelocity),
    weapon(projectileImageFile, size, &loc) {}


/**
 * Constructs a randomized PlayerCharacter. The client has to option to simply leave the argument randSeed as
 * 0, in which case the constructor will generate its own random number.
 *
 * @param randSeed A seed to initialize the random number generator
 */
PlayerCharacter::PlayerCharacter(FastRand<int> rand) :
    Character(rand),
    weapon(AssetFile::projectileImageFilenames->at(2), 1.0, & loc){}


/**
 * Destructor for PlayerCharacter
 */
PlayerCharacter::~PlayerCharacter() {}


/**
 * Assignment operator overload (copy)
 *
 * @param rhs The right hand side argument (which will be copied)
 */
PlayerCharacter & PlayerCharacter::operator=(const PlayerCharacter & rhs) {
    if (this != &rhs) {
        this->Character::operator=(rhs) ;
		this->moveFlag = rhs.moveFlag ;
    }
    return *this ;
    
}

/**
 * Assignment operator overload (move)
 *
 * @param rhs The right hand side argument (which will be moved)
 */
PlayerCharacter & PlayerCharacter::operator=(PlayerCharacter && rhs) {
    if (this != &rhs) {
        this->Character::operator=(std::move(rhs)) ;
		this->moveFlag = rhs.moveFlag ;
    }
    return *this ;
}


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

void PlayerCharacter::moveNewDirection(Vectr<float> & newDirection) {
	
	newDirection.normalize() ;
	vectr += newDirection ;
	
	printPositition() ;
	
	moveFlag = true ;
}

//using GameObject's implementation for now, may change later
void PlayerCharacter::defaultBehaviors() {
   /* Overriding this with nothing, since we are player-controlled, not AI-controlled */
}


void PlayerCharacter::fire() {
    weapon.fire() ;
}


void PlayerCharacter::fire(const Character * atEnemy) {
    
}

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
	if (moveFlag) {
		move() ;
		moveFlag = false ;
	}
}

/**
 * Writes a formatted text description of this PlayerCharacter into the desired output stream
 */
void PlayerCharacter::textDescription(ostream * writeTo) const {
	this->Character::textDescription(writeTo) ;
}

void PlayerCharacter::printPositition() {
	if (*vectr.getCurrent() != vectr.getLast()) {
		stringstream ss ;
		ss << this->name << "'s current world position is: " << getPosition() ;
		auto screenPosition = translateToWindowCoords(*getPosition()) ;
		ss << this->name << "'s current screen position is: " << screenPosition << '\n' ;
		DebugOutput << ss.rdbuf() ;
	}
}



