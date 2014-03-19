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

Location AdapterUtil::transLocation(const Location &inGameWorld) {
	int worldXSize = GameObject::GLOBAL_MAX_LOCATION->x - GameObject::GLOBAL_MIN_LOCATION->x ;
	int worldYSize = GameObject::GLOBAL_MAX_LOCATION->y - GameObject::GLOBAL_MAX_LOCATION->y ;
	int x = (termWidth() * inGameWorld.x)/worldXSize ;
	int y = (termHeight() * inGameWorld.y)/worldYSize ;
	return Location(x, y, 0) ;
}
