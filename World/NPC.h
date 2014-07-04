//
//  NPC.h
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__NPC__
#define __GameWorld__NPC__

#include <iostream>

#include "Character.h"

#include "../Control/Input.hpp"


class NPC : public Character {
	
  
protected:
	
    
	//todo
	
	
public:
	/**
	 * Constructs a default NPC.
	 */
	NPC() ;
	
	
	/**
	 * Copy constructor for NPC
	 *
	 * @param The NPC to be copied
	 */
	NPC(const NPC & other) ;
	
	/**
	 * Move constructor for NPC
	 *
	 * @param The NPC to be moved
	 */
	NPC(NPC && other) ;
	
	
	/**
	 * Constructs an NPC based on the arguments given
	 *
	 * @param color The color of this NPC
	 * @param symbol The icon to be used by this NPC
	 * @param name The name of this NPC
	 * @param alive Whether this NPC is dead or alive
	 * @param state The CharacterState of this NPC
	 * @param health The Health of this NPC
	 * @param damage The Damage capability of this NPC
	 * @param reaction The reaction of this NPC to the player
	 */
	NPC(Color color, AssetType type, const AssetFile & imageFile, float size, const Pos2<float> & loc,
		string name, DoA alive, CharacterState state, unsigned health, unsigned damage, Reaction reaction) ;
	
	
	/**
	 * Constructs a randomized NPC. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param randSeed A seed to initialize the random number generator
	 */
	NPC(FastRand<int> rand) ;
	
	
	/**
	 * Destructor for NPC
	 */
	~NPC() ;
	
	
	/**
	 * Assignment operator overload (copy)
	 *
	 * @param rhs The right hand side argument (which will be copied)
	 */
	NPC & operator=(const NPC & rhs) ;
	
	/**
	 * Assignment operator overload (move)
	 *
	 * @param rhs The right hand side argument (which will be moved)
	 */
	NPC & operator=(NPC && rhs) ;
	
	
	/**
	 * Overloads operator() for NPC. Possibly will be used to
	 * call notify(). TBD.
	 */
	void operator()() ;
	
	
	/**
	 * Overloads the overload of operator(). The actual implementation
	 * and uses for this are still undecided.
	 *
	 * @param otherNPC A reference to another NPC
	 */
	void operator()(Character & other) ;
	
	
	/**
	 * Another class with a reference to this NPC can call this to have the NPC perform some
	 * function, as yet undecided. TBI.
	 */
	void notify() ;
	
	
	/**
	 * A NPC can use this function to pass messages to another.
	 *
	 * @param message The Message sent by this
	 * @param recipient The object receiving the Message
	 */
	void passMessage(Message * message, Character & recipient) ;
	
	/**
	 * Writes a formatted text description of this NPC into the desired output stream
	 */
	virtual void textDescription(ostream * writeTo) const ;
	
	/**
	 * Attacks a hostile NPC
	 *
	 * @param enemy The enemy to attack
	 */
	void attack(Character * enemy) ;
	
	
	/**
	 * Returns this NPC's reaction (attitude) toward the player
	 */
	Reaction getReaction() { return  this->reaction ; }
	
	
  
} ;

#endif /* defined(__GameWorld__NPC__) */
