//
//  WorldController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#define eight_milliseconds 8333

#include "WorldController.h"

using namespace std ;

//bool WorldController::running = false ;

vector<GameObject*> * WorldController::gameObjects  = nullptr ;
thread WorldController::worldSimulationThread ;
thread WorldController::checkDelThread ;
const GameMap<GameObject> * WorldController::map = nullptr ;

WorldController::WorldController() {}

void WorldController::init() {

	WorldController::gameObjects = GameObject::getAllGameObjects() ; 
	WorldController::map = GameObject::getMap() ;
	
	/* debug code */
	fastRand<long> rand(floor(GLOBAL_MIN_X, GLOBAL_MIN_Y), ceilling(GLOBAL_MAX_X, GLOBAL_MAX_Y)) ;
	Position<long> pos(200, 200, 0) ;
	for (unsigned i = 0 ; i < 30 ; i++) {
		new GameObject(fastRand<long>(0, GLOBAL_MAX_X)) ;
	}
	/* debug end */
	
	//we also assigned all the MAX constants in both GameObject and
	//WorldController so that they sync together (see above)

	SharedGameData::initData(GameObject::getAllGameObjects(), GameObject::getMap()) ;
}

void WorldController::exec() {
	worldSimulationThread = thread(&WorldController::runWorldSimulation) ;  //runWorldSimulation()
	checkDelThread = thread(&GameObject::checkForMarkedDeletions) ;
}

void WorldController::runWorldSimulation() {

	fastRand<unsigned> speedVariance = fastRand<unsigned>(1, 3) ;

	while (GLOBAL_CONTINUE_SIGNAL) {
		for (auto i = 0 ; i < gameObjects->size() ; i++) {
			gameObjects->at(i)->wanderVariedSpeed(speedVariance) ;
			//we can add any other default behaviors here
		}
	}
}



void WorldController::exit() {
	
	sharedMutex.lock() ; //we don't want our Adapter thinking its safe to read our GameObjects any more
	
	checkDelThread.join() ;
	
	GameObject::joinThreads() ;
	
	for (auto i = 0 ; i < gameObjects->size() ; i++) {
		delete gameObjects->at(i) ;
	}

	delete gameObjects ;
	delete map ; 
	gameObjects = nullptr ;
	
	sharedMutex.unlock() ;
}
