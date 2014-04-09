//
//  AdapterUtil.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/18/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "AdapterUtil.h"

using namespace std	;

unsigned AdapterUtil::termWidth() {
	unsigned r = atoi(getenv("COLUMNS")) ;
	return r ;
}

unsigned AdapterUtil::termHeight() {
	unsigned r = atoi(getenv("LINES")) ;
	return r ;
}

