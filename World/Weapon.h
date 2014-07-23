//
//  Weapon.h
//  SpriteFight
//
//  Created by Adam James Leuer on 7/10/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__Weapon__
#define __SpriteFight__Weapon__

#include <iostream>

#include "GameObject.h"

class Projectile : public GameObject {
	
protected:
	
public:
	
	using GameObject::GameObject ;
	
	/**
	 * Overidden to ensure this has no functionality
	 */
	void moveRandomDirection() override {
		; //NOP
	}
	
	/**
	 * Overidden to ensure this has no functionality
	 */
	void defaultBehaviors() override {
		//NOP
	}
	
	
};


class Weapon {
	
protected:
	
    /**
     * A simple GameObject that will only be drawn onscreen immediately after the
     * weapon fires
     */
	Projectile projectile ;
    
    const Position<float> * ownerPosition ;
	
public:
	
	Weapon(const AssetFile & projectileImageFile, float sizeModifier, const Position<float> * ownerPosition) :
		projectile(projectileImageFile, sizeModifier, *ownerPosition, false, false) {}
    
    Weapon(FastRand<int> & randm, const Position<float> * ownerPosition) ;
    
    Weapon(const Weapon & other) :
        projectile(other.projectile) {}
    
    Weapon(Weapon && other) :
        projectile(std::move(other.projectile)) {}
    
    Weapon & operator=(const Weapon & rhs) ;
    
    Weapon & operator=(Weapon && rhs) ;
	
	void fire() ;
	
};

#endif /* defined(__SpriteFight__Weapon__) */
