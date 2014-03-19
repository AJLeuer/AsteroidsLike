//
//  World.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "World.h"

bool World::running = false ;

vector<GameObject*> World::allObjects  = vector<GameObject*>() ;

World::World() {}

void World::init() {
	running = true ;
	
	GameObject::GLOBAL_MAX_LOCATION = new struct Location(500, 500, 500) ;
	GameObject::GLOBAL_MIN_LOCATION = new struct Location(-500, -500, -500) ;
	
	 //testing code
	for (vector<NPC>::size_type i = 0 ; i < 15 ; i++) {
		allObjects.push_back(new NPC(rand())) ;
	}
	
}

void World::playGameInRealTime() {
	//todo
}

void World::playGameRecorded(std::ostream *writeTO) {
	//testing code
	for (vector<NPC>::size_type i = 0 ; i < 15 ; i++) {
		allObjects.at(i)->textDescription(writeTO) ;
		*writeTO << endl << endl ;
	}
	//end testing
}
