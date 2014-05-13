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
thread * WorldController::checkDelThread = nullptr ;
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
	fastRand<long> rand(floor(GLOBAL_MIN_X, GLOBAL_MIN_Y), ceilling(GLOBAL_MAX_X, GLOBAL_MAX_Y)) ;
	Position<long> pos(200, 200, 0) ;
	for (unsigned i = 0 ; i < 3 ; i++) {
		new GameObject(AssetType::character, "/Assets/Blocks/Blocks_01_256x256_Alt_01_001.png", 1, pos) ;
		new GameObject(rand) ;
	}
	/* debug end */
	
	//we also assigned all the MAX constants in both GameObject and
	//WorldController so that they sync together (see above)
	
}

void WorldController::exec() {
	runWorldSimulation() ;
	checkDelThread = new std::thread(&GameObject::checkForMarkedDeletions) ;
}

void WorldController::runWorldSimulation() {
	for (auto i = 0 ; i < gameObjects->size() ; i++) {
		gameObjects->at(i)->runOnThread() ;
	}
}



void WorldController::exit() {
	
	sharedMutex.lock() ; //we don't want our Adapter thinking its safe to read our GameObjects any more
	
	checkDelThread->join() ;
	
	GameObject::joinThreads() ;
	
	for (auto i = 0 ; i < gameObjects->size() ; i++) {
		delete gameObjects->at(i) ;
	}
	
	delete checkDelThread ;
	delete gameObjects ;
	delete map ; 
	gameObjects = nullptr ;
	
	sharedMutex.unlock() ;
}
