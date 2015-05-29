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

#include "../Util/Timer.hpp"

#include "GameObject.h"


class Projectile : public GameObject {
    using GameObject::GameObject ;
    
    virtual void doDefaultBehavior(bool initialCall = false) override ;
};


class Weapon {
	
protected:
	
	static list<Projectile> firedProjectiles ;
	
	static void trackFiredProjectiles() ;
	
	
    AssetFile textureFile ;
    float sizeModifier ;
    
    /**
     * A simple sprite that will only be drawn onscreen immediately after the
     * weapon fires
     */
	Projectile * projectile ;
	
public:
	
	static void init() ;
	
	Weapon(const AssetFile & file, float sizeMod) :
        textureFile(file),
        sizeModifier(sizeMod),
		projectile(nullptr) {}
    
	
    Weapon(const Weapon & other) :
        textureFile(other.textureFile),
        sizeModifier(other.sizeModifier),
		projectile(other.projectile) {}
    
    Weapon(Weapon && other) :
        textureFile(std::move(other.textureFile)),
        sizeModifier(other.sizeModifier),
        projectile(other.projectile)
    {
        other.projectile = nullptr ;
    }
    
    ~Weapon() { /* projectile deleted at end of fire(), don't call it here */ }
    
	//Weapon & operator=(const Weapon & rhs) ;
    
	//Weapon & operator=(Weapon && rhs) ;
	
	void fire(const Vect<float> & startingPos, const Angle & orientation) ;
	
};

#endif /* defined(__SpriteFight__Weapon__) */
