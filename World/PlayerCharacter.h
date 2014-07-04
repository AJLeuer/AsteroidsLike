//
//  PlayerCharacter.h
//  SpriteFight
//
//  Created by aleuer2 on 6/30/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__PlayerCharacter__
#define __SpriteFight__PlayerCharacter__

#include <iostream>
#include <sstream>

#include "../Util/AssetFileIO.h"
#include "Character.h"
#include "../Control/Input.hpp"

/**
 * A class which serves as the basic template for just about any person in the game world, whether player
 * or npc.
 */
class PlayerCharacter : public Character  {
    
protected:
	

    
public:
	
	/**
	 * Constructs a default PlayerCharacter.
	 */
	PlayerCharacter() ;
	
	
	/**
	 * Copy constructor for PlayerCharacter
	 *
	 * @param The PlayerCharacter to be copied
	 */
	PlayerCharacter(const PlayerCharacter & other) ;
	
	/**
	 * Move constructor for PlayerCharacter
	 *
	 * @param The PlayerCharacter to be moved
	 */
	PlayerCharacter(PlayerCharacter && other) ;
	
	
	/**
	 * Constructs a PlayerCharacter based on the arguments given
	 *
	 * @param name The name of this PlayerCharacter
	 * @param alive Whether this PlayerCharacter is dead or alive
	 * @param state The CharacterState of this PlayerCharacter
	 * @param health The Health of this PlayerCharacter
	 * @param damage The Damage capability of this PlayerCharacter
	 */
	PlayerCharacter(Color color, AssetType type, const AssetFile & imageFile, float size, const Pos2<float> & loc, string name, Reaction reaction, DoA alive, CharacterState state, unsigned health, unsigned damage) ;
	
	
	/**
	 * Constructs a randomized PlayerCharacter. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param randSeed A seed to initialize the random number generator
	 */
	PlayerCharacter(FastRand<int> rand) ;
	
	
	/**
	 * Destructor for PlayerCharacter
	 */
	~PlayerCharacter() ;
	
	
	/**
	 * Assignment operator overload (copy)
	 *
	 * @param rhs The right hand side argument (which will be copied)
	 */
	PlayerCharacter & operator=(const PlayerCharacter & rhs) ;
	
	/**
	 * Assignment operator overload (move)
	 *
	 * @param rhs The right hand side argument (which will be moved)
	 */
	PlayerCharacter & operator=(PlayerCharacter && rhs) ;
	
	
	/**
	 * Overloads operator() for PlayerCharacter. Possibly will be used to
	 * call notify(). TBD.
	 */
	void operator()() ;
	
	
	/**
	 * Overloads the overload of operator(). The actual implementation
	 * and uses for this are still undecided.
	 *
	 * @param otherCharacter A reference to another PlayerCharacter
	 */
	void operator()(GameObject * other) ;
	

	/**
	 * Writes a formatted text description of this PlayerCharacter into the desired output stream
	 */
	void textDescription(ostream * writeTo) const ;
	
	//using GameObject's implementation for now, may change later
	void defaultBehaviors() ;
	
	/**
	 * Attacks a hostile PlayerCharacter
	 *
	 * @param enemy The enemy to attack
	 */
	void attack(PlayerCharacter * enemy) ;
    
    void moveUp() override { GameObject::moveUp(defaultOffset<float>) ; /* Debug */ printPositition() ; /* end debug */ }
	void moveDown() override { GameObject::moveDown(defaultOffset<float>) ; /* Debug */ printPositition() ; /* end debug */ }
	void moveRight() override { GameObject::moveRight(defaultOffset<float>) ; /* Debug */ printPositition() ; /* end debug */ }
	void moveLeft() override { GameObject::moveLeft(defaultOffset<float>) ; /* Debug */ printPositition() ; /* end debug */ }
	
	void moveUp(float offset) override { GameObject::moveUp(offset) ; /* Debug */ printPositition() ; /* end debug */ }
	void moveDown(float offset) override { GameObject::moveDown(offset) ; /* Debug */ printPositition() ; /* end debug */ }
	void moveRight(float offset) override { GameObject::moveRight(offset) ; /* Debug */ printPositition() ; /* end debug */ }
	void moveLeft(float offset) override { GameObject::moveLeft(offset) ; /* Debug */ printPositition() ; /* end debug */ }
    
    void jump() ;
    
    void printPositition() ;
	
	
} ;

#endif /* defined(__SpriteFight__PlayerCharacter__) */
