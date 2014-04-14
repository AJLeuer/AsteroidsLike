//
//  WorldController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "WorldController.h"



bool WorldController::running = false ;

vector<GameObject*> * WorldController::gameObjects  = nullptr ;
GameMap<GameObject> * WorldController::map = nullptr ;

std::mutex WorldController::runningMtx ;

const double WorldController::GLOBAL_MAX_X = GameObject::GLOBAL_MAX_X ;
const double WorldController::GLOBAL_MIN_X = GameObject::GLOBAL_MIN_X ;
const double WorldController::GLOBAL_MAX_Y = GameObject::GLOBAL_MAX_Y ;
const double WorldController::GLOBAL_MIN_Y = GameObject::GLOBAL_MIN_Y ;

WorldController::WorldController() {}

void WorldController::init() {
	running = true ;
	
	gameObjects  = new vector<GameObject*>() ;
	map = new GameMap<GameObject>(GLOBAL_MAX_X, GLOBAL_MAX_Y) ;
	
	GameObject::allGameObjects = WorldController::gameObjects ;
	GameObject::map = WorldController::map ;
	
	//we also assigned all the GLOBAL_MAX constants in both GameObject and
	//WorldController so that they sync together (see above)
	
	/* uncomment this! *//*
	for (vector<GameObject*>::size_type i = 0 ; i < 75 ; i++) {
		gameObjects->push_back(new NPC(1)) ; //<-uncomment
	}
	*//* uncomment this! */
	
	/* debug code */
	GameObject * searchTest0 = new GameObject("s0", new Location<long>(100, 19, 0)) ;
	GameObject * searchTest1 = new GameObject("s1", new Location<long>(97, 14, 0)) ; //was found x2 the first time
	GameObject * notFound = new GameObject("nf", new Location<long>(94, 18, 0)) ;
	
	gameObjects->push_back(searchTest0) ;
	gameObjects->push_back(searchTest1) ;
	gameObjects->push_back(notFound) ;
	/* end debug */
	
}

//testing code
void WorldController::foo(double xyOffs, unsigned long time, bool * b) {
	/*
	for (vector<GameObject*>::size_type i = 0 ; i < gameObjects->size() ; i++) {
		gameObjects->at(i)->wander(xyOffs, b) ;
	}
	*/
	
	vector<GameObject*> * found ;
	
	const Location<long> * loc = new Location<long>(95, 15, 0) ;
	
	found = GameObject::map->findNearby<long>(loc, 5, 5) ;
	
	
	Debug::draw2DRepresentation(*(Debug::debugFile), GameObject::map->getMapVect(), ' ') ;

	bool bo = true ;//temp debug
}

void WorldController::playGameInRealTime() {
	//todo
}

void WorldController::playGameRecorded(std::ostream * writeTo) {
	//testing code
	for (vector<GameObject*>::size_type i = 0 ; i < 15 ; i++) {
		gameObjects->at(i)->textDescription(writeTo) ;
		*writeTo << endl << endl ;
	}
}

void WorldController::close() {
	
	runningMtx.lock() ; //we don't want our Adapter thinking its safe to read our GameObjects any more
	running = false ;
	GameObject::joinThreads() ;
	GameObject::map->eraseAll() ;
	for (vector<GameObject*>::size_type i = 0 ; i < gameObjects->size() ; i++) {
		delete gameObjects->at(i) ;
	}
	delete gameObjects ;
	gameObjects = nullptr ;
	runningMtx.unlock() ;
	
}
