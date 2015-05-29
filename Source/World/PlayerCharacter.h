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
#include "Weapon.h"
#include "../Control/Input.hpp"

/**
 * A class which serves as the basic template for just about any person in the game world, whether player
 * or npc.
 */
class PlayerCharacter : public Character  {
    
protected:
	
	/**
	 * Any state change that cannot be done to PlayerCharacter in realtime, and only once per global loop,
	 * should be stored the form of std::pair consisting of a void PlayerCharacter member function and pointer to the relevant
	 * PlayerCharacter object, and pushed back onto defferedCallbacks. The function will be called once during update(),
	 * then removed from defferedCallbacks. Each time the the function is needed it will need to be readded.
	 */
    vector<std::pair<void (PlayerCharacter::*)(), PlayerCharacter *>> deferredCallbacks ;
    
	Weapon weapon ;
    
    
	/**
	 * @brief A boolean flag which, when set to true, indicated that this PlayerCharacter should move()
	 *		  on the next call to update
	 */
	bool moveFlag = false ;
	
	
public:
	
	/**
	 * Constructs a default PlayerCharacter.
	 */
	PlayerCharacter() ;
	
	
	/**
	 * Copy constructor for PlayerCharacter
	 *
	 * @param other PlayerCharacter to be copied
	 */
	PlayerCharacter(const PlayerCharacter & other) ;
	
	/**
	 * Move constructor for PlayerCharacter
	 *
	 * @param other The PlayerCharacter to be moved
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
	PlayerCharacter(const AssetFile & imageFile, float size, const Vect<float> & loc,
					const Angle rotation, string name, Reaction reaction, DoA alive, CharacterState state,
					unsigned health, unsigned damage, SafeBoolean monitorVelocity, bool boundsChecking, const AssetFile & projectileImageFile) ;
	
	
	/**
	 * Constructs a randomized PlayerCharacter. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param rand A seed to initialize the random number generator
	 */
	PlayerCharacter(Randm<int> rand, AssetType type) ;
	
	
	/**
	 * Destructor for PlayerCharacter
	 */
	~PlayerCharacter() ;

	
	/**
	 * Overloads operator() for PlayerCharacter. Possibly will be used to
	 * call notify(). TBD.
	 */
	void operator()() ;
	
	
	/**
	 * Overloads the overload of operator(). The actual implementation
	 * and uses for this are still undecided.
	 *
	 * @param other A reference to another PlayerCharacter
	 */
	void operator()(GameObject * other) ;
	
	/**
	 * Similar in function to @GameObject::moveNewDirection(),
	 * but overridden to ensure the player cannot move more than once
	 * each time through the game loop
	 */
	void move(VectorAndVelocity<float> & direction, float distanceModifier = defaultMoveDistance<float>) override ;
	
	/**
	 * Overidden to ensure this has no functionality
	 */
	void moveRandomDirection() override {
		;//NOP
	}
	
	//using GameObject's implementation for now, may change later
	virtual void doDefaultBehavior(bool initialCall = false) override ;
	
	/* no AI behaviors, for obvious reasons */
	void aiBehaviors() override { /* nothing */ }
	
	/**
	 * Fires the PlayerCharacter's weapon
     *
	 */
	void fire() ;
    
    /**
	 * Fires the PlayerCharacter's weapon
	 *
	 * @param atEnemy The enemy to attack
	 */
	void fire(const Character * atEnemy) ;
	
    
    void jump() ;
	
    /**
     * @brief Some PlayerCharacter functions cannot be called in realtime (i.e. not at the time the Player reqests them). There
     * may be multiple reasons for this. For example, movement functions cannot be called in realtime, as that would result in the Player's avatar
     * moving unevenly across the screen. For all such functions, instead of making a state change directly, they should push_back a reference to another
     * function that *will* perform the updating onto the updateCallbacks vector
     */
	void update() override ;
	
	/**
	 * Writes a formatted text description of this PlayerCharacter into the desired output stream
	 */
	void textDescription(ostream * writeTo) const ;
    
    void printPositition() ;
	
} ;

#endif /* defined(__SpriteFight__PlayerCharacter__) */
