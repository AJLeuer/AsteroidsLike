//
//  WorldController.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#include "WorldController.h"

using namespace std ;

//bool WorldController::running = false ;

vector<GameObject*> * WorldController::gameObjects  = nullptr ;
vector<GameObject*> * WorldController::secondaryGameObjects = new vector<GameObject *>() ;
thread WorldController::worldSimulationThread ;
thread WorldController::checkDelThread ;
const GameMap<GameObject> * WorldController::map = nullptr ;

WorldController::WorldController() {}

void WorldController::update() {
	for (auto i = 0 ; i < secondaryGameObjects->size() ; i++) {
		secondaryGameObjects->at(i)->moveSameDirection() ;
	}
}

void WorldController::init() {

	WorldController::gameObjects = GameObject::getAllGameObjects() ;
	WorldController::map = GameObject::getMap() ;

	FastRand<int> posModifier(-100, 100) ;

	float startingXArea = (GLOBAL_MAX_X * 0.75) ;
	float startingYArea = (GLOBAL_MAX_Y * 0.5) ;

	DirectionVector<float> test(-11, 1.5, 0, nullptr) ;

	secondaryGameObjects->push_back(new GameObject(AssetType::block, "/Assets/Blocks/Blocks_01_256x256_Alt_03_006.png",
			0.50, Pos2<float>(startingXArea, (startingYArea + posModifier()), 0, defaultCheck<float>))) ;

	secondaryGameObjects->push_back(new GameObject(AssetType::block, "/Assets/Blocks/Blocks_01_256x256_Alt_03_005.png",
			0.50, Pos2<float>(startingXArea, (startingYArea + posModifier()), 0, defaultCheck<float>))) ;
	
	SharedGameData::initData(GameObject::getAllGameObjects(), GameObject::getMap()) ;
	
	/* start some of the GameObjects moving. exec() will take over from here */
	gameObjects->at(0)->moveDown() ;
	gameObjects->at(1)->moveUp() ;
}

void WorldController::exec() {
	worldSimulationThread = thread(&WorldController::runWorldSimulation) ;  //runWorldSimulation()
	checkDelThread = thread(&GameObject::checkForMarkedDeletions) ;
}

void WorldController::runWorldSimulation() {
	
	while (GLOBAL_CONTINUE_SIGNAL) {
		auto startTime = mainGameClock->checkTimeElapsed() ;
		auto refreshTime = eight_milliseconds ;
		
		update() ;
		
		auto timeElapsed = (mainGameClock->checkTimeElapsed()) - startTime ;
		auto sleepTime = refreshTime - timeElapsed ;
		this_thread::sleep_for(sleepTime) ;
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
