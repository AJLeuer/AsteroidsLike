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
#include "../Output/OutputData.hpp"


class Weapon {
	
protected:
	
    /**
     * A simple sprite that will only be drawn onscreen immediately after the
     * weapon fires
     */
	OutputData<float, int> projectile ;
	
	Position<float> pos ; /* we won't use this for much, just init'ing projectile mainly.
						   fire() takes arguments to to tell use where we are, so we don't rely on pos */

	
public:
	
	Weapon(const AssetFile & file, const float sizeModifier, PositionType type) :
		pos(0, 0),
		projectile(file, &pos, ZERO_DEGREES, sizeModifier, type, false) {}
    
	Weapon(FastRand<unsigned long> & randm, const float sizeModifier, PositionType type) :
		pos(0, 0),
        projectile(AssetFile::projectileImageFilenames->at(randm(0, AssetFile::projectileImageFilenames->size()-1)) , &pos, ZERO_DEGREES, sizeModifier, type, false) /* ie not visible, don't monitor velocity */ {}
	
    Weapon(const Weapon & other) :
		projectile(other.projectile) {}
    
    Weapon(Weapon && other) :
        projectile(std::move(other.projectile)) {}
    
	//Weapon & operator=(const Weapon & rhs) ;
    
	//Weapon & operator=(Weapon && rhs) ;
	
	void fire(const Position<float> startingPos, const Vectr<float> & direction, Angle<float> * orientation) ;
	
};

#endif /* defined(__SpriteFight__Weapon__) */
