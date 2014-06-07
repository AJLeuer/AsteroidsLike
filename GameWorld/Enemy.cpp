//
//  Enemy.cpp
//  GameWorld
//
//  Created by aleuer2 on 6/3/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Enemy.h"


void Enemy::defaultBehaviors() {
  
    if (this->getPosition()->getX() > GLOBAL_MIN_X) {
        moveLeft(defaultOffset<float>) ;
    }
    else if ((this->getPosition()->getX()) <= (GLOBAL_MIN_X - 1)) {
        
    }

	
}