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
		//gameObjects->push_back(new NPC(1)) ;
	}
	
}

//testing code
void WorldController::foo(double xyOffs, unsigned long time, bool * b) {
	/*
	for (vector<GameObject*>::size_type i = 0 ; i < gameObjects->size() ; i++) {
		gameObjects->at(i)->wander(xyOffs, b) ;
	}
	*/
	//temp code
	GameObject searcher(0) ;
	searcher.move(Location<long>(65, 29, 0)) ;
	
	GameObject test1(0) ;
	test1.move(Location<long>(69, 27, 0)) ;
	
	GameObject test2(0) ;
	test2.move(Location<long>(55, 1, 0)) ;
	
	GameObject test3(0) ;
	test3.move(Location<long>(55, 1, 0)) ;
	
	GameObject test4(0) ;
	test4.move(Location<long>(75, 21, 0)) ;
	
	GameObject notFound(0) ;
	notFound.move(Location<long>(62, 23, 0)) ;
	
	GameObject out_of_search_range(0) ; //shoudln't be found
	out_of_search_range.move(Location<long>(79, 10, 0)) ;
	
	
	
	GameObject::map->drawSimpleGraphic(' ', *Debug::debugFile, " ") ;
	
	
	vector<GameObject*> * found ;
	found = GameObject::map->findNearby<long>(*searcher.getLocation(), 15, 15) ;
	
	
	
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
