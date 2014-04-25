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
const GameMap<GameObject> * WorldController::map = nullptr ;



const long WorldController::MAX_X { GLOBAL_MAX_X } ;
const long WorldController::MIN_X { GLOBAL_MIN_X } ;
const long WorldController::MAX_Y { GLOBAL_MAX_Y } ;
const long WorldController::MIN_Y { GLOBAL_MIN_Y } ;

WorldController::WorldController() {}

void WorldController::init() {

	WorldController::gameObjects = GameObject::getAllGameObjects() ;
	WorldController::map = GameObject::getMap() ;
	
	/* debug code */
	fastRand<long> rand(returnSmaller(GLOBAL_MIN_X, GLOBAL_MIN_Y), returnLarger(GLOBAL_MAX_X, GLOBAL_MAX_Y)) ;
	
	for (unsigned i = 0 ; i < 15 ; i++) {
		new NPC(rand) ;
	}
	/* debug end */
	
	//we also assigned all the MAX constants in both GameObject and
	//WorldController so that they sync together (see above)
	
}

void WorldController::runWorldSimulation() {
	void (*rws_ptr)() = &WorldController::runWorldSimulation_threaded ;
	thread * thr  = new std::thread(rws_ptr) ;
	thr->join() ;
	void (*chdel_ptr)() = &WorldController::checkForMarkedDeletions ;
	new std::thread(chdel_ptr) ;
}

void WorldController::runWorldSimulation_threaded() {
	for (auto i = 0 ; i < gameObjects->size() ; i++) {
		gameObjects->at(i)->runOnThread() ;
	}
}

void WorldController::checkForMarkedDeletions() {
	while (GLOBAL_CONTINUE_SIGNAL) {
		for (auto i = 0 ; i < gameObjects->size() ; i++) {
			if (gameObjects->at(i)->markedForDeletion == true) {
				*gameObjects->at(i)->currentlyThreading = false ;
				delete gameObjects->at(i) ;
				gameObjects->at(i) = nullptr ;
			}
		}
	}
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
	delete map ; 
	gameObjects = nullptr ;
	
	Locking::sharedMutex.unlock() ;
}
