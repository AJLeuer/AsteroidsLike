//
//  WorldController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "WorldController.h"



//bool WorldController::running = false ;

vector<GameObject*> * WorldController::gameObjects  = nullptr ;
GameMap<GameObject> * WorldController::map = nullptr ;



const long WorldController::MAX_X { GLOBAL_MAX_X } ;
const long WorldController::MIN_X { GLOBAL_MIN_X } ;
const long WorldController::MAX_Y { GLOBAL_MAX_Y } ;
const long WorldController::MIN_Y { GLOBAL_MIN_Y } ;

WorldController::WorldController() {}

void WorldController::init() {

	WorldController::gameObjects = GameObject::allGameObjects ;
	WorldController::map = GameObject::map ;
	
	/* debug code */
	for (unsigned i = 0 ; i < 250 ; i++) {
		new GameObject(1) ;
	}
	/* debug end */
	
	//we also assigned all the MAX constants in both GameObject and
	//WorldController so that they sync together (see above)
	
}


void WorldController::runWorldSimulation(unsigned long time) {
/* debug code */
	auto wanderOffset = 1 ;
	
	for (auto i = 0 ; i < gameObjects->size() ; i++) {
		gameObjects->at(i)->wander(wanderOffset, time) ;
	}
	
	vector< const GameObject*> * found ;
	
	const Location<long> * loc = new Location<long>(95, 15, 0) ;
	
	found = GameObject::map->findNearby<long>(loc, 5, 5) ;
/* end debug code */
}


void WorldController::playGameInRealTime() {
	//todo
}

void WorldController::playGameRecorded(std::ostream * writeTo) {
	
}

void WorldController::close() {
	
	Locking::sharedMutex.lock() ; //we don't want our Adapter thinking its safe to read our GameObjects any more
	GameObject::joinThreads() ;
	
	

	for (auto i = 0 ; i < gameObjects->size() ; i++) {
		delete gameObjects->at(i) ;
	}
	
	delete gameObjects ;
	delete GameObject::map ; 
	gameObjects = nullptr ;
	Locking::sharedMutex.unlock() ;
	
}
