//
//  Weapon.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/10/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Weapon.h"



void Weapon::fire(const Position<float> & startingPos, const Angle & orientation) {
    
    projectile = new Projectile(textureFile, sizeModifier, startingPos, orientation, true, SafeBoolean::f) ; //current plan is to not actually create the projectile until it's fired
	
	Vectr<float> vectr = *(projectile->getRawMutableVector()) ; //copy vector
	
	/* rotate our vector by the given angle */
	vectr.rotateDiff(orientation) ;

	auto fireL = [this, vectr, orientation] () mutable -> void { /* copies variables by value */
        
        projectile->graphicsData->setBoundsChecking(false) ;
        
        projectile->graphicsData->overrideCurrentOrientation(orientation) ;
		
		projectile->moveNewDirection(vectr) ;
		
		projectile->setVisibility(true) ; //set to visible only after we've moved it into the correct position

        do {
            projectile->move() ;
            
            this_thread::sleep_for(std::chrono::microseconds(250)) ;
            
        } while (projectile->overBounds() == false) ;
        
        projectile->setVisibility(false) ;
        
        projectile->markForDeletion() ;
		
	} ;
	
	thread thr(fireL) ;
	thr.detach() ;
}


