//
//  Weapon.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/10/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Weapon.h"

list<Projectile> Weapon::firedProjectiles ;

void Weapon::trackFiredProjectiles() {
	
	while (GLOBAL_CONTINUE_FLAG) {
		
		while (firedProjectiles.empty() == false) {
			
			for (auto i = firedProjectiles.begin() ; i != firedProjectiles.end() ; ) {
				
				Projectile * projectile = & (* i) ;
				
				projectile->move(defaultMoveDistance<float> / 4) ;
				
				if (projectile->overBounds() == true) {
					
					projectile->setVisibility(false) ;
					
					i++ ;
					auto next = i ;
					i-- ;

					firedProjectiles.erase(i) ;

					if (next == firedProjectiles.end()) {
						break ;
					}
					else {
						i = next ;
					}
				}
				else {
					i++ ;
				}
			}
			this_thread::sleep_for(chrono::microseconds(256)) ;
		}
		
		this_thread::sleep_for(std::chrono::milliseconds(32)) ;
	}
}

void Weapon::init() {
	static std::thread projectileTrackingThread(Weapon::trackFiredProjectiles) ;
	projectileTrackingThread.detach() ;
}

void Projectile::doDefaultBehavior(bool initialCall) {
    /* do nothing */ ;
}

void Weapon::fire(const Position<float> & startingPos, const Angle & orientation) {
    
	firedProjectiles.emplace_back(textureFile, sizeModifier, startingPos, orientation, false, SafeBoolean::f, false) ;
	
	Projectile & projectile = firedProjectiles.back() ;
	
	/* rotate our vector by the given angle */
    projectile.getRawMutableVector()->rotateVector(orientation) ;
    
    projectile.graphicsData->setBoundsChecking(false) ;
	
	projectile.setVisibility(true) ;
}


