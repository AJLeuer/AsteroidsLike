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
	
	 //testing code
	for (vector<GameObject*>::size_type i = 0 ; i < 75 ; i++) {
		gameObjects->push_back(new NPC(rand())) ;
	}
	
}

//testing code
void WorldController::foo(double xyOffs, unsigned long time, bool * b) {
	for (vector<GameObject*>::size_type i = 0 ; i < gameObjects->size() ; i++) {
		//gameObjects->at(i)->wander(xyOffs, b) ;
	}
	GameObject * go_temp = (gameObjects->at(rand() % gameObjects->size())) ;
	Location<long> loc_temp = *(go_temp->getLocation()) ;
	Location<long> loc = Location<long>(2, 3, 0) ;
	Location<long> loc2 = Location<long>(2, 3, 0) ;
	GameObject::map->move(loc, loc2) ;
	const Location<long> locs = Location<long>(75, 75, 0) ;
	vector<GameObject*> * found ;
	found = GameObject::map->findNearby<long>(locs, 50, 50) ;
	
	
	cout << *(found->at(10)) ;
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
