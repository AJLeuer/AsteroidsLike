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
    weapon(Colors::blue, AssetFile::projectileImageFilenames->at(3), size.getModifier(), {loc->getX()/2, loc->getY(), loc->getZ()}){}


/**
 * Copy constructor for PlayerCharacter
 *
 * @param The PlayerCharacter to be copied
 */
PlayerCharacter::PlayerCharacter(const PlayerCharacter & other) :
    Character(other), weapon(other.weapon),
    moveInfo(other.moveInfo) {}

/**
 * Move constructor for PlayerCharacter
 *
 * @param The PlayerCharacter to be moved
 */
PlayerCharacter::PlayerCharacter(PlayerCharacter && other) :
    Character(other), weapon(std::move(other.weapon)),
    moveInfo(std::move(other.moveInfo)) {}

/**
 * Constructs a PlayerCharacter based on the arguments given
 *
 * @param name The name of this PlayerCharacter
 * @param alive Whether this PlayerCharacter is dead or alive
 * @param state The CharacterState of this PlayerCharacter
 * @param health The Health of this PlayerCharacter
 * @param damage The Damage capability of this PlayerCharacter
 */
PlayerCharacter::PlayerCharacter(Colors color, const AssetFile & imageFile, float size, const Pos2<float> & loc, string name, Reaction reaction, DoA alive, CharacterState state, unsigned health, unsigned damage, const AssetFile & projectileImageFile) :
    Character(color, imageFile, size, loc, name, reaction, alive, state, health, damage),
    weapon(color, projectileImageFile, size, {loc.getX()/2, loc.getY(), loc.getZ()}) {}


/**
 * Constructs a randomized PlayerCharacter. The client has to option to simply leave the argument randSeed as
 * 0, in which case the constructor will generate its own random number.
 *
 * @param randSeed A seed to initialize the random number generator
 */
PlayerCharacter::PlayerCharacter(FastRand<int> rand) :
    Character(rand),
    weapon(Colors::blue, AssetFile::projectileImageFilenames->at(2), 1.0, {0, 0, 0}){}


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
		this->moveInfo = rhs.moveInfo ;
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
		this->moveInfo = std::move(rhs.moveInfo) ;
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
	
	moveInfo.first = true ;
}

//using GameObject's implementation for now, may change later
void PlayerCharacter::defaultBehaviors() {
   /* Overriding this with nothing, since we are player-controlled, not AI-controlled */
}

/**
 * Attacks a hostile PlayerCharacter
 *
 * @param enemy The enemy to attack
 */
void PlayerCharacter::attack(PlayerCharacter * enemy) {
    //todo
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
	if (moveInfo.first) {
		move(moveInfo.second) ;
		moveInfo = {false, defaultMoveDistance<float>} ;
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



