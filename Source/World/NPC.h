//
//  NPC.h
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__NPC__
#define __SpriteFight__NPC__

#include <iostream>

#include "Pawn.h"

#include "../Control/Input.hpp"


class NPC : public Pawn {
	
  
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
	 * @param other The NPC to be copied
	 */
	NPC(const NPC & other) ;
	
	/**
	 * Move constructor for NPC
	 *
	 * @param other The NPC to be moved
	 */
	NPC(NPC && other) ;
	
	
	/**
	 * Constructs an NPC based on the arguments given
	 */
	NPC(const AssetFile & imageFile, float size, const Vect<float> & loc, const Angle rotation,
		string name, unsigned health, unsigned damage, Util::SafeBoolean monitorVelocity, bool boundsChecking) ;
	
	
	/**
	 * Constructs a randomized NPC. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param rand A seed to initialize the random number generator
	 */
	NPC(Randm<int> rand, AssetType type) ;
	
	
	/**
	 * Destructor for NPC
	 */
	~NPC() ;

	
	/**
	 * Overloads operator() for NPC. Possibly will be used to
	 * call notify(). TBD.
	 */
	void operator()() ;
	
	
	/**
	 * Overloads the overload of operator(). The actual implementation
	 * and uses for this are still undecided.
	 *
	 * @param other A reference to another NPC
	 */
	void operator()(Pawn & other) ;
	
	/**
	 * Writes a formatted text description of this NPC into the desired output stream
	 */
	virtual void textDescription(ostream * writeTo) const ;
	
	/**
	 * Attacks a hostile NPC
	 *
	 * @param enemy The enemy to attack
	 */
	void attack(GameObject * enemy) ;

	
	
  
} ;

#endif /* defined(__SpriteFight__NPC__) */
