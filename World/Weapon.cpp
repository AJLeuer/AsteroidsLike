//
//  Weapon.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/10/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Weapon.h"


/*
Weapon & Weapon::operator=(const Weapon & rhs) {
    if (this != &rhs) {
        projectile = rhs.projectile ;
    }
    return *this ;
}

Weapon & Weapon::operator=(Weapon && rhs) {
    if (this != &rhs) {
        projectile = std::move(rhs.projectile) ;
    }
    return *this ;
} */

void Weapon::fire(const Position<float> startingPos, const Angle & orientation) {
    
    Position<float> * pos = projectile.getRawMutablePosition() ; /* dangerous */
    
    Vectr<float> * vectr = projectile.getRawMutableVector() ;
	
	pos->setAll(startingPos) ; //should update projectile's position as well

	auto fireL = [this, orientation, vectr, pos] () -> void { /* copies variables by value */
		
		/* copy projectile to make a new projectile */
		/* projectile will start out in a completely wrong spot. We need to move it before drawing it onscreen.
		 Move projectile to our current spot */
		projectile.setVisibility(true) ;
		
		/* rotate our vector by the given angle */
		vectr->rotateDiff(orientation) ;
		
		while ((projectile.getPosition().overBounds(&BoundsCheck<float>::defaultCheck)) == false) {
			*pos += *vectr ;
			this_thread::sleep_for(std::chrono::microseconds(250)) ;
		}
	} ;
	
	thread thr(fireL) ;
	thr.detach() ;
}


