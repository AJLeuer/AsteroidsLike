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
    
    Position<float> * pos = projectile->getRawMutablePosition() ;
	
	Vectr<float> vectr = *(projectile->getRawMutableVector()) ; //copy vector
	
	/* rotate our vector by the given angle */
	vectr.rotateDiff(orientation) ;

	auto fireL = [this, pos, vectr, orientation] () mutable -> void { /* copies variables by value */
        
        //projectile->outputData.setBoundsChecking(false) ;
        
        projectile->outputData.overrideCurrentOrientation(orientation) ;
		
		projectile->moveTo(pos) ;
		
		projectile->moveNewDirection(vectr) ;
		
		projectile->setVisibility(true) ; //set to visible only after we've moved it into the correct position
		
		while ((pos->overBounds(BoundsCheck<float>::defaultCheck)) == false) {
			projectile->move() ;
			this_thread::sleep_for(std::chrono::microseconds(250)) ;
		}
		
	projectile->markForDeletion() ;
		
	} ;
	
	thread thr(fireL) ;
	thr.detach() ;
}


