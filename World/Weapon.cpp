//
//  Weapon.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/10/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Weapon.h"



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
}

void Weapon::fire() {

	auto fireL = [this] () -> void {
		
		/* copy projectile to make a new projectile */
		/* projectile will start out in a completely wrong spot. We need to move it before drawing it onscreen.
		 Move projectile to our current spot */
		
		auto projectileFired = this->projectile ;
		
		projectileFired.moveTo(*ownerPosition) ;
		
		projectileFired.setVisibility(true) ;
		
		Vectr<float> tempvec = ownerVector->copyVect(false) ;
		
		while ((projectileFired.getPosition()->overBounds(&BoundsCheck<float>::defaultCheck)) == false) {
			projectileFired.moveNewDirection(tempvec, defaultMoveDistance<float>, nullptr) ;
			this_thread::sleep_for(std::chrono::milliseconds(2)) ;
		}
	} ;
	
	thread thr(fireL) ;
	thr.detach() ;
}


