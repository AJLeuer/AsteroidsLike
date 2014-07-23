//
//  Weapon.cpp
//  SpriteFight
//
//  Created by Adam James Leuer on 7/10/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Weapon.h"

Weapon::Weapon(FastRand<int> & randm, const Position<float> * ownerPosition) {
    
}

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
	projectile.setVisibility(true) ;
}


