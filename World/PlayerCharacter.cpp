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
    Character() {}


/**
 * Copy constructor for PlayerCharacter
 *
 * @param The PlayerCharacter to be copied
 */
PlayerCharacter::PlayerCharacter(const PlayerCharacter & other) :
    Character(other) {}

/**
 * Move constructor for PlayerCharacter
 *
 * @param The PlayerCharacter to be moved
 */
PlayerCharacter::PlayerCharacter(PlayerCharacter && other) :
    Character(other) {}

/**
 * Constructs a PlayerCharacter based on the arguments given
 *
 * @param name The name of this PlayerCharacter
 * @param alive Whether this PlayerCharacter is dead or alive
 * @param state The CharacterState of this PlayerCharacter
 * @param health The Health of this PlayerCharacter
 * @param damage The Damage capability of this PlayerCharacter
 */
PlayerCharacter::PlayerCharacter(Color color, AssetType type, const string & imageFilename, float size, const Pos2<float> & loc, string name, Reaction reaction, DoA alive, CharacterState state, unsigned health, unsigned damage) :
    Character(color, type, imageFilename, size, loc, name, reaction, alive, state, health, damage) {}


/**
 * Constructs a randomized PlayerCharacter. The client has to option to simply leave the argument randSeed as
 * 0, in which case the constructor will generate its own random number.
 *
 * @param randSeed A seed to initialize the random number generator
 */
PlayerCharacter::PlayerCharacter(FastRand<int> rand) :
    Character(rand) {}


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


/**
 * Writes a formatted text description of this PlayerCharacter into the desired output stream
 */
void PlayerCharacter::textDescription(ostream * writeTo) const {
    this->Character::textDescription(writeTo) ;
}

//using GameObject's implementation for now, may change later
void PlayerCharacter::defaultBehaviors() {
    //todo
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
	jump() ;
	timedTurnInvisible(std::chrono::nanoseconds(64000000)) ;
	
    #ifdef DEBUG_MODE
    printPositition() ;
    #endif
}

void PlayerCharacter::printPositition() {
	stringstream ss ;
	ss << this->name << "'s current world position is: " << getPosition() ;
	auto screenPosition = translateToWindowCoords(*getPosition()) ;
	ss << this->name << "'s current screen position is: " << screenPosition << '\n' ;
	DebugOutput << ss.rdbuf() ;
}



