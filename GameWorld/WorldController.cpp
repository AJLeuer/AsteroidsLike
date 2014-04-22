//
//  WorldController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#define eight_milliseconds 8333

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
	for (unsigned i = 0 ; i < 5 ; i++) {
		new GameObject(1) ;
	}
	/* debug end */
	
	//we also assigned all the MAX constants in both GameObject and
	//WorldController so that they sync together (see above)
	
}

void WorldController::runWorldSimulation() {
	/* debug code */
	void (*rws_ptr)() = &WorldController::runWorldSimulation ;
	new std::thread(rws_ptr) ;
	/* end debug code */
}

void WorldController::runWorldSimulation_threaded() {
	/* debug code */
	unsigned wanderOffset = 1 ;
	
	for (auto i = 0 ; i < gameObjects->size() ; i++) {
		auto rand = fastRand<unsigned int>(8, 40) ;
		unsigned speedChange = rand.nextValue() ; //use this to differentiate the rate at which each
													  //wandering object will update its speed
		gameObjects->at(i)->wander(wanderOffset, (speedChange * eight_milliseconds), GLOBAL_CONTINUE_SIGNAL) ;
	}

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
