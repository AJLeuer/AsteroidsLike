//
//  Weapon.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/10/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Weapon.h"

void Projectile::doDefaultBehavior(bool initialCall) {
    /* do nothing */ ;
}

void Weapon::fire(const Position<float> & startingPos, const Angle & orientation) {
    
    projectile = new Projectile(textureFile, sizeModifier, startingPos, orientation, false, SafeBoolean::f, false) ; //current plan is to not actually create the projectile until it's fired
	
	/* rotate our vector by the given angle */
    projectile->getRawMutableVector()->rotateVector(orientation) ;
    
    projectile->graphicsData->setBoundsChecking(false) ;

	auto fireL = [this] () mutable -> void { /* copies variables by value */
		
		projectile->move(defaultMoveDistance<float> / 4) ;
		
		projectile->setVisibility(true) ; //set to visible only after we've moved it into the correct position

        do {
            projectile->move(defaultMoveDistance<float> / 4) ;
            
            this_thread::sleep_for(std::chrono::microseconds(250)) ;
            
        } while (projectile->overBounds() == false) ;
        
        projectile->setVisibility(false) ;
        
        projectile->markForDeletion() ;
		
	} ;
	
	thread thr(fireL) ;
	thr.detach() ;
}


