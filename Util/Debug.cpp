//
//  Debug.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/6/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Debug.h"

using namespace std ;

ofstream * Debug::debugFile { new ofstream("debug.txt") } ;

unsigned Debug::debugCounter = 0 ;