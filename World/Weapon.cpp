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
		
		projectile.moveTo(*ownerPosition) ; //projectile will start out in a completely wrong spot. We need to move it before drawing it onscreen
		projectile.setVisibility(true) ;
		Vectr<float> tempvec = copyVect(*this->ownerVector, false) ;
		
		while ((projectile.getPosition()->overBounds(&BoundsCheck<float>::defaultCheck)) == false) {
			projectile.moveNewDirection(tempvec) ;
			this_thread::sleep_for(std::chrono::milliseconds(24)) ;
		}
	} ;
	
	thread thr(fireL) ;
	thr.detach() ;
}


