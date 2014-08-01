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


class Weapon {
	
protected:
	
    /**
     * A simple sprite that will only be drawn onscreen immediately after the
     * weapon fires
     */
	GameObject projectile ;
	
public:
	
	Weapon(const AssetFile & file, const float sizeModifier) :
		projectile(file, sizeModifier, Position<float>(0, 0), {0.0}, false, SafeBoolean::f) {}
    
	Weapon(FastRand<unsigned long> & randm, const float sizeModifier, PositionType type) :
        projectile(AssetFile::projectileImageFilenames->at(randm(0, AssetFile::projectileImageFilenames->size()-1)) , new Position<float>(0, 0), 0.0, sizeModifier, type, false) /* ie not visible, don't monitor velocity */ {}
	
    Weapon(const Weapon & other) :
		projectile(other.projectile) {}
    
    Weapon(Weapon && other) :
        projectile(std::move(other.projectile)) {}
    
    ~Weapon() { delete (projectile.getRawMutablePosition()) ;}
    
	//Weapon & operator=(const Weapon & rhs) ;
    
	//Weapon & operator=(Weapon && rhs) ;
	
	void fire(const Position<float> startingPos, const Angle & orientation) ;
	
};

#endif /* defined(__SpriteFight__Weapon__) */
