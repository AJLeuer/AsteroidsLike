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
	
	//void setPosition(Position<float> * pos) ;
	
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
	
	const Vectr<float> * ownerVector ;
	
public:
	
	Weapon(const AssetFile & projectileImageFile, float sizeModifier, const Position<float> * ownerPosition, const Vectr<float> * ownerVector ) :
		projectile(projectileImageFile, sizeModifier, *ownerPosition, false, false),
		ownerPosition(ownerPosition),
		ownerVector(ownerVector) {}
    
	Weapon(FastRand<unsigned long> & randm, const Position<float> * ownerPosition, const Vectr<float> * ownerVector) :
		projectile(AssetFile::projectileImageFilenames->at(randm(0, AssetFile::projectileImageFilenames->size()-1)), 1.0, *ownerPosition, false, false), /* ie not visible, don't monitor velocity */
		ownerPosition(ownerPosition),
		ownerVector(ownerVector) {}
	
    Weapon(const Weapon & other) :
		projectile(other.projectile) {}
    
    Weapon(Weapon && other) :
        projectile(std::move(other.projectile)) {}
    
    Weapon & operator=(const Weapon & rhs) ;
    
    Weapon & operator=(Weapon && rhs) ;
	
	void fire() ;
	
};

#endif /* defined(__SpriteFight__Weapon__) */
