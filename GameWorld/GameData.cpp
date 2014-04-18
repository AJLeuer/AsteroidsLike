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

extern const long GLOBAL_MAX_X { 179 } ;
extern const long GLOBAL_MIN_X { 0 } ;
extern const long GLOBAL_MAX_Y { 37 } ;
extern const long GLOBAL_MIN_Y { 0 } ;



