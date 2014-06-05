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
thread WorldController::worldSimulationThread ;
thread WorldController::checkDelThread ;
const GameMap<GameObject> * WorldController::map = nullptr ;

WorldController::WorldController() {}


void WorldController::init() {

	WorldController::gameObjects = GameObject::getAllGameObjects() ;
	WorldController::map = GameObject::getMap() ;

	FastRand<int> posModifier(-100, 100) ;

	float startingYAreaHi = (GLOBAL_MAX_Y * 0.75) ;
	float startingYAreaLo = (GLOBAL_MAX_Y * 0.25) ;
	

	DirectionVector<float> test(-11, 1.5, 0, nullptr) ;
	
	/* Init enemies */
	new Enemy(AssetType::character, AssetFileIO::getRandomImageFilename(AssetType::character),
			0.50, Pos2<float>(GLOBAL_MAX_X, (startingYAreaHi + posModifier()), 0, defaultCheck<float>)) ;

	new Enemy(AssetType::character, AssetFileIO::getRandomImageFilename(AssetType::character),
			0.50, Pos2<float>(GLOBAL_MAX_X, (startingYAreaLo + posModifier()), 0, defaultCheck<float>)) ;
	
	/* Init obstacles */
	
	FastRand<float>randomSizeModifier(0.75, 3.0) ;
	
	for (auto i = 0 ; i < 7 ; i++) {
		new GameObject(AssetType::block, AssetFileIO::getRandomImageFilename(AssetType::block), randomSizeModifier(),
					   Pos2<float>(randPosSetter<float>, defaultCheck<float>)) ;
	}
	
	/* Init game state */
	GameState::initData(GameObject::getAllGameObjects(), GameObject::getMap()) ;
	
}

void WorldController::main() {
	worldSimulationThread = thread(&WorldController::runWorldSimulation) ;  //runWorldSimulation()
	checkDelThread = thread(&GameObject::checkForMarkedDeletions) ;
}

void WorldController::runWorldSimulation() {
	
	while (GLOBAL_CONTINUE_SIGNAL) {
		auto startTime = mainGameClock->checkTimeElapsed() ;
		auto refreshTime = eight_milliseconds ;
		

		auto timeElapsed = (mainGameClock->checkTimeElapsed()) - startTime ;
		auto sleepTime = refreshTime - timeElapsed ;
		worldLoopCount++ ;
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






