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

AdapterInterface::AdapterInterface(AdapterInterface * current) {
	if ((extentMembers + 1) > 1) {
		cout << "Warning: there are currently " << extentMembers <<
			" instances of AdapterInterface or instances of classes deriving from AdapterInterface in existence" << endl ;
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

void AdapterInterface::show() {}

void AdapterInterface::show_threaded() {}

void AdapterInterface::operator()() {}

void AdapterInterface::close() { WorldObjects = nullptr ; }

