//
//  World.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "World.h"



bool World::running = false ;

vector<GameObject*> * World::gameObjects  = nullptr ;

std::mutex World::runningMtx ;

const double World::GLOBAL_MAX_X = GameObject::GLOBAL_MAX_X ;
//const double World::GLOBAL_MIN_X = GameObject::GLOBAL_MIN_X ;
const double World::GLOBAL_MAX_Y = GameObject::GLOBAL_MAX_Y ;
//const double World::GLOBAL_MIN_Y = GameObject::GLOBAL_MIN_Y ;

World::World() {}

void World::init() {
	running = true ;
	gameObjects  = new vector<GameObject*>() ;
	
	 //testing code
	for (vector<GameObject*>::size_type i = 0 ; i < 5 ; i++) {
		gameObjects->push_back(new NPC(rand())) ;
	}
	
}

//testing code
void World::foo(unsigned long time, bool * b) {
	for (vector<GameObject*>::size_type i = 0 ; i < gameObjects->size() ; i++) {
		gameObjects->at(i)->wander(15, b) ;
	}
}

void World::playGameInRealTime() {
	//todo
}

void World::playGameRecorded(std::ostream * writeTo) {
	//testing code
	for (vector<GameObject*>::size_type i = 0 ; i < 15 ; i++) {
		gameObjects->at(i)->textDescription(writeTo) ;
		*writeTo << endl << endl ;
	}
}

void World::close() {
	runningMtx.lock() ; //we don't want our Adapter thinking its safe to read our GameObjects any more
	running = false ;
	GameObject::joinThreads() ;
	for (vector<GameObject*>::size_type i = 0 ; i < gameObjects->size() ; i++) {
		delete gameObjects->at(i) ;
	}
	delete gameObjects ;
	gameObjects = nullptr ;
	runningMtx.unlock() ;
}
