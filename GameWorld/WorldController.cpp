//
//  WorldController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#define eight_milliseconds 8333 //i.e. 8 millis in microseconds

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

	FastRand<int> posModifier(-100, 100) ;

	float startingXArea = (GLOBAL_MAX_X * 0.75) ;
	float startingYArea = (GLOBAL_MAX_Y * 0.5) ;
	

	for (unsigned i = 0 ; i < 2 ; i++) {
		new GameObject(AssetType::character, AssetFileIO::getRandomImageFilename(AssetType::character),
					   0.50, Pos2<float>((startingXArea + posModifier()), (startingYArea + posModifier()), 0, defaultCheck<float>)) ;
	}


	SharedGameData::initData(GameObject::getAllGameObjects(), GameObject::getMap()) ;
}

void WorldController::exec() {
	worldSimulationThread = thread(&WorldController::runWorldSimulation) ;  //runWorldSimulation()
	checkDelThread = thread(&GameObject::checkForMarkedDeletions) ;
}

void WorldController::runWorldSimulation() {

	DirectionVector<float> rightUp(2.0, 1.0, 0, nullptr) ;
	DirectionVector<float> leftUp(-2.0, 1.0, 0, nullptr) ;

	for (auto i = 0 ; i < gameObjects->size() ; i++) {
		//
	}

	gameObjects->at(0)->moveNewDirection(rightUp) ;
	gameObjects->at(1)->moveNewDirection(leftUp) ;


	while (GLOBAL_CONTINUE_SIGNAL) {
		for (auto i = 0 ; i < gameObjects->size() ; i++) {
			gameObjects->at(i)->moveSameDirection() ;
		}
        usleep(eight_milliseconds) ;
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
