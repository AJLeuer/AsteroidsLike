//
//  Enemy.cpp
//  GameWorld
//
//  Created by aleuer2 on 6/3/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Enemy.h"


Enemy::Enemy() {
    
}

void Enemy::defaultBehaviors() {
    /* So we can alter our behavior each time through the loop */
	bool even = ((worldControllerLoopCount % 2) == 0) ;
	
	FastRand<float> randOffsetter(-defaultOffset<float>, defaultOffset<float>) ;
	auto randOffsetVal = (randOffsetter() * 30) ;
	
	
    if (even) {
        moveUp(randOffsetVal) ; /* will actually move us up or down (randomly) */
        moveUp(randOffsetVal * (-1)) ; /* will put us back where we started longtitudinally, though
																	laterally we've still moved to the left  */
    }
    else {
        moveLeft(defaultOffset<float> * 2) ;
    }
	
}