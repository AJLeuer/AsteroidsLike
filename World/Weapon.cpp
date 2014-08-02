//
//  Weapon.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/10/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Weapon.h"



void Weapon::fire(const Position<float> & startingPos, const Angle & orientation) {
    
    projectile = new GameObject(textureFile, sizeModifier, startingPos, orientation, true, SafeBoolean::f) ; //current plan is to not actually create the projectile until it's fired
    
    Position<float> * pos = projectile->getRawMutablePosition() ;
    Vectr<float> * vectr = projectile->getRawMutableVector() ;

	auto fireL = [this, pos, vectr, &orientation] () -> void { /* copies variables by value */
		
		/* copy projectile to make a new projectile */
		/* projectile will start out in a completely wrong spot. We need to move it before drawing it onscreen.
		 Move projectile to our current spot */
		projectile->setVisibility(true) ;
		
		/* rotate our vector by the given angle */
		vectr->rotateDiff(orientation) ;
		
		while ((pos->overBounds(BoundsCheck<float>::defaultCheck)) == false) {
			*pos += *vectr ;
			this_thread::sleep_for(std::chrono::microseconds(250)) ;
		}
	} ;
	
	thread thr(fireL) ;
	thr.detach() ;
}


