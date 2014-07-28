//
//  Character.h
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
class Character : public GameObject {

protected:
	
	string name ;
	
	Reaction reaction ;
	
	DoA alive ;
	
	/**
	 * The Character's CharacterState
	 */
	CharacterState state ;
	
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
	
	void attack_helper(Character * enemy) ;

public:
	
	/**
	 * @brief Constructs a default Character.
	 */
	Character() ;
	
	
	/**
	 * Copy constructor for Character
	 *
	 * @param other Character to be copied
	 */
	Character(const Character & other) ;
	
	/**
	 * Move constructor for Character
	 *
	 * @param other Character to be moved
	 */
	Character(Character && other) ;
	
	
	/**
	 * Constructs a Character based on the arguments given
	 *
	 * @param name The name of this Character
	 * @param alive Whether this Character is dead or alive
	 * @param state The CharacterState of this Character
	 * @param health The Health of this Character
	 * @param damage The Damage capability of this Character
	 */
	Character(const AssetFile & imageFile, float sizeModifier,
			  const Position<float> & loc, const Angle rotation, string name, Reaction reaction, DoA alive, CharacterState state, unsigned health, unsigned damage, bool monitorVelocity) ;
	
	
	/**
	 * Constructs a randomized Character. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param rand A seed to initialize the random number generator
	 */
	Character(FastRand<int> rand, AssetType type) ;
	
	
	/**
	 * Destructor for Character
	 */
	~Character() ;
	
	
	/**
	 * Assignment operator overload (copy)
	 *
	 * @param rhs The right hand side argument (which will be copied)
	 */
	virtual Character & operator=(const Character & rhs) ;
	
	/**
	 * Assignment operator overload (move)
	 *
	 * @param rhs The right hand side argument (which will be moved)
	 */
	virtual Character & operator=(Character && rhs) ;
	
	
	/**
	 * Overloads operator() for Character. Possibly will be used to
	 * call notify(). TBD.
	 */
	virtual void operator()() ;
	
	
	/**
	 * Overloads the overload of operator(). The actual implementation
	 * and uses for this are still undecided.
	 *
	 * @param other A reference to another Character
	 */
	virtual void operator()(GameObject * other) ;
	
	
	/**
	 * Another class with a reference to this Character can call this to have the Character perform some
	 * function, as yet undecided. TBI.
	 */
	virtual void notify() ;
	
	
	/**
	 * A Character can use this function to pass messages to another.
	 *
	 * @param message The Message sent by this
	 * @param recipient The object receiving the Message
	 */
	virtual void passMessage(Message * message, GameObject & recipient) ;
	
	/**
	 * Writes a formatted text description of this Character into the desired output stream
	 */
	virtual void textDescription(ostream * writeTo) const ;
	
	//using GameObject's implementation for now, may change later
	virtual void defaultBehaviors() override ;
	
	virtual void aiBehaviors() override ;
	
	/**
	 * Attacks a hostile Character
	 *
	 * @param enemy The enemy to attack
	 */
	virtual void attack(Character * enemy) ;
	
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
	 * @return This character's current state
	 */
	CharacterState * getState() ;
	
	/**
	 * Set's this Character's state
	 *
	 * @param state The Character's new state
	 */
	void setState(CharacterState & state) { this->state = state ; }
	
	
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
