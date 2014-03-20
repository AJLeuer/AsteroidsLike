//
//  AdapterInterface.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/19/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "AdapterInterface.h"

unsigned AdapterInterface::extentMembers = 0 ;

AdapterInterface * AdapterInterface::current = nullptr ;

vector<GameObject*> ** AdapterInterface::WorldObjects = nullptr ;

AdapterInterface::AdapterInterface(AdapterInterface * current, int n) {
	if ((extentMembers + 1) > 1) {
		cout << "No more than one instance of any class derived from AdapterInterface can exist at one time" << endl ;
		throw new exception() ;
	}
	extentMembers++ ;
	AdapterInterface::current = current ;
}

AdapterInterface::~AdapterInterface() {
	if (this == current) {
		extentMembers-- ;
		current = nullptr ;
	}
}

void AdapterInterface::init() {
	if (World::isRunning()) {
		WorldObjects = World::getGameObjects() ;
	}
}

void AdapterInterface::show() { /*TODO add */ }

void AdapterInterface::close() { WorldObjects = nullptr ; }

