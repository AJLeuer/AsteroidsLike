//
//  Pawn.h
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __Util__Character__
#define __Util__Character__

#include <iostream>
#include <sstream>

#include "../Util/AssetFileIO.h"
#include "GameObject.h"
#include "../Control/Input.hpp"

/**
 * @brief A class which serves as the basic template for just about any person in the game world, whether player
 *        or npc.
 */
class Pawn : public GameObject {

protected:
	
	string name ;

	
	/**
	 * The Character's current Health (a data structure inheriting from GameState)
	 */
	Health * health ;
	
	/**
	 * The Character's current Damage (a data structure inheriting from GameState)
	 */
	Damage * damage ;

	/**
	 * Used by attack()
	 */
	bool turn = false ;
	
	float hitPercentage ;
	
	void attack_helper(Pawn * enemy) ;

public:
	
	/**
	 * @brief Constructs a default Pawn.
	 */
	Pawn() ;
	
	
	/**
	 * Copy constructor for Pawn
	 *
	 * @param other Pawn to be copied
	 */
	Pawn(const Pawn & other) ;
	
	/**
	 * Move constructor for Pawn
	 *
	 * @param other Pawn to be moved
	 */
	Pawn(Pawn && other) ;
	
	
	/**
	 * Constructs a Pawn based on the arguments given
	 *
	 * @param name The name of this Pawn
	 * @param alive Whether this Pawn is dead or alive
	 * @param state The CharacterState of this Pawn
	 * @param health The Health of this Pawn
	 * @param damage The Damage capability of this Pawn
	 */
	Pawn(const AssetFile & imageFile, float sizeModifier,
			  const Vect<float> & loc, const Angle rotation, string name, unsigned health, unsigned damage, SafeBoolean monitorVelocity, bool boundsChecking) ;
	
	
	/**
	 * Constructs a randomized Pawn. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param rand A seed to initialize the random number generator
	 */
	Pawn(Randm<int> rand, AssetType type) ;
	
	
	/**
	 * Destructor for Pawn
	 */
	~Pawn() ;

	/**
	 * Overloads operator() for Pawn. Possibly will be used to
	 * call notify(). TBD.
	 */
	virtual void operator()() override ;
	
	
	/**
	 * Overloads the overload of operator(). The actual implementation
	 * and uses for this are still undecided.
	 *
	 * @param other A reference to another Pawn
	 */
	virtual void operator()(GameObject * other) override ;
	
	/**
	 * Writes a formatted text description of this Pawn into the desired output stream
	 */
	virtual void textDescription(ostream * writeTo) const  override ;
	
	//using GameObject's implementation for now, may change later
	virtual void doDefaultBehavior(bool initialCall = false) override ;
	
	virtual void aiBehaviors() override ;
	
	/**
	 * Attacks a hostile Pawn
	 *
	 * @param enemy The enemy to attack
	 */
	virtual void attack(GameObject * enemy) override ;
	
	/**
	 * Getter for name
	 */
	const string & getName() const { return this->name ; }
	
	/**
	 * @brief Setter for name
	 *
	 * @param s The Character's new name
	 */
	void setName(string s) { this->name = s ; }

	
	/**
	 * @return a value representing Character's health
	 */
	Health * checkHealth() ;
	
	
	/**
	 * Sets this Character's health
	 */
	void modHealth(Health *) ;
	
	
	/**
	 * @return a read-only value representing Character's damage
	 */
	Damage * getDamage() ;

	
	/**
	 * Sets this Character's damage
	 */
	void modDamage(Damage *) ;
	
	/*
	 * Fire's the Character's weapon
	 */
	
	
} ;

#endif /* defined(__Util__Character__) */
