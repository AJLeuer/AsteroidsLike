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
	double worldXSize = GameObject::GLOBAL_MAX_X - GameObject::GLOBAL_MIN_X ;
	double worldYSize = GameObject::GLOBAL_MAX_Y - GameObject::GLOBAL_MIN_Y ;
	
	double tempX = inGameWorld.x + (worldXSize - GameObject::GLOBAL_MAX_X) ;
	double tempY = inGameWorld.y + (worldYSize - GameObject::GLOBAL_MAX_Y) ;
	
	unsigned tw = termWidth() ;
	unsigned th = termHeight() ;
	
	int x = (tw * tempX)/worldXSize ;
	int y = (th * tempY)/worldYSize ;
	
	return Location(x, y, 0) ;
}
