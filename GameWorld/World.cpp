//
//  World.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "World.h"

bool World::running = false ;

vector<GameObject*> * World::allObjects  = nullptr ;

World::World() {}

void World::init() {
	running = true ;
	allObjects  = new vector<GameObject*>() ;
	
	 //testing code
	for (vector<GameObject*>::size_type i = 0 ; i < 15 ; i++) {
		allObjects->push_back(new NPC(rand())) ;
	}
	
}

void World::playGameInRealTime() {
	//todo
}

void World::playGameRecorded(std::ostream * writeTo) {
	//testing code
	for (vector<GameObject*>::size_type i = 0 ; i < 15 ; i++) {
		allObjects->at(i)->textDescription(writeTo) ;
		*writeTo << endl << endl ;
	}
}

void World::close() {
	running = false ;
	for (vector<GameObject*>::size_type i = 0 ; i < allObjects->size() ; i++) {
		delete allObjects->at(i) ;
	}
	delete allObjects ;
}
