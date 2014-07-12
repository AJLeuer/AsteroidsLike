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


class Weapon {
	
protected:
	
    /**
     * A simple GameObject that will only be drawn onscreen immediately after the
     * weapon fires
     */
	GameObject projectile ;
	
public:
	
	Weapon(Colors color, const AssetFile & projectileImageFile, float sizeModifier, const Pos2<float> & loc_) :
		projectile(color, projectileImageFile, sizeModifier, loc_, false) {}
    
    Weapon(FastRand<int> & randm) ;
    
    Weapon(const Weapon & other) :
        projectile(other.projectile) {}
    
    Weapon(Weapon && other) :
        projectile(std::move(other.projectile)) {}
    
    Weapon & operator=(const Weapon & rhs) ;
    
    Weapon & operator=(Weapon && rhs) ;
	
	void fire() ;
	
};

#endif /* defined(__SpriteFight__Weapon__) */
