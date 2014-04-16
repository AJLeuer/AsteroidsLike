//
//  WorldController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "WorldController.h"



bool WorldController::running = false ;

list<GameObject*> * WorldController::gameObjects  = nullptr ;
GameMap<GameObject> * WorldController::map = nullptr ;

std::mutex WorldController::runningMtx ;

const long WorldController::MAX_X = GameObject::MAX_X ;
const long WorldController::MIN_X = GameObject::MIN_X ;
const long WorldController::MAX_Y = GameObject::MAX_Y ;
const long WorldController::MIN_Y = GameObject::MIN_Y ;

WorldController::WorldController() {}

void WorldController::init() {
	running = true ;
	
	/* debug code */
	for (unsigned i = 0 ; i < 50 ; i++) {
		new GameObject(1) ;
	}
	
	WorldController::gameObjects = GameObject::allGameObjects ;
	WorldController::map = GameObject::map ;
	
	//we also assigned all the MAX constants in both GameObject and
	//WorldController so that they sync together (see above)
	
	
}

//testing code
void WorldController::foo(double xyOffs, unsigned long time, bool * b) {
	
	for (auto i = gameObjects->begin() ; i != gameObjects->end() ; i++) {
		(*i)->wander(xyOffs, b) ;
	}
	
	
	vector< const GameObject*> * found ;
	
	const Location<long> * loc = new Location<long>(95, 15, 0) ;
	
	found = GameObject::map->findNearby<long>(loc, 5, 5) ;
	
	Drawing drawing ;
	drawing.draw2DRepresentation(*(Debug::debugOutput), GameObject::map->getMapVect(), ' ') ;

}

void WorldController::playGameInRealTime() {
	//todo
}

void WorldController::playGameRecorded(std::ostream * writeTo) {
	//testing code
	for (auto i = gameObjects->begin() ; i != gameObjects->end() ; i++) {
		(*i)->textDescription(writeTo) ;
		*writeTo << endl << endl ;
	}
}

void WorldController::close() {
	
	runningMtx.lock() ; //we don't want our Adapter thinking its safe to read our GameObjects any more
	running = false ;
	GameObject::joinThreads() ;
	//delete GameObject::map ; //deletes internal vectors
	
	unsigned n = 0 ; //debug symbol
	for (auto i = gameObjects->begin() ; i != gameObjects->end() ; i++) {
		delete (*i) ;
		n++ ;
	}
	
	delete gameObjects ;
	gameObjects = nullptr ;
	runningMtx.unlock() ;
	
}
