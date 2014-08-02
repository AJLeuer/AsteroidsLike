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
    
    AssetFile textureFile ;
    float sizeModifier ;
    
    /**
     * A simple sprite that will only be drawn onscreen immediately after the
     * weapon fires
     */
	GameObject * projectile ;
	
public:
	
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
    
    ~Weapon() { delete projectile ;}
    
	//Weapon & operator=(const Weapon & rhs) ;
    
	//Weapon & operator=(Weapon && rhs) ;
	
	void fire(const Position<float> & startingPos, const Angle & orientation) ;
	
};

#endif /* defined(__SpriteFight__Weapon__) */
