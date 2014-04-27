//
//  GameData.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/14/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <iostream>

#include "GameData.h"


using namespace std ;

mutex Locking::sharedMutex ;

extern bool GLOBAL_CONTINUE_SIGNAL = true ;


extern const long GLOBAL_MAX_X { GLOBAL_MAX_X_ } ;
extern const long GLOBAL_MIN_X { GLOBAL_MIN_X_ } ;
extern const long GLOBAL_MAX_Y { GLOBAL_MAX_Y_ } ;
extern const long GLOBAL_MIN_Y { GLOBAL_MIN_Y_ } ;





