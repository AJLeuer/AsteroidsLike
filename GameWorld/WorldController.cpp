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
vector<GameObject*> * WorldController::obstacleObjects = new vector<GameObject *>() ;
thread WorldController::worldSimulationThread ;
thread WorldController::checkDelThread ;
unsigned WorldController::worldControllerLoopCount = 0 ;
const GameMap<GameObject> * WorldController::map = nullptr ;

WorldController::WorldController() {}


void WorldController::init() {

	WorldController::gameObjects = GameObject::getAllGameObjects() ;
	WorldController::map = GameObject::getMap() ;

	FastRand<int> posModifier(-100, 100) ;

	float startingYAreaHi = (GLOBAL_MAX_Y * 0.75) ;
	float startingYAreaLo = (GLOBAL_MAX_Y * 0.25) ;
	

	DirectionVector<float> test(-11, 1.5, 0, nullptr) ;

	obstacleObjects->push_back(new GameObject(AssetType::block, "/Assets/Blocks/Blocks_01_256x256_Alt_03_006.png",
			0.50, Pos2<float>(GLOBAL_MAX_X, (startingYAreaHi + posModifier()), 0, defaultCheck<float>))) ;

	obstacleObjects->push_back(new GameObject(AssetType::block, "/Assets/Blocks/Blocks_01_256x256_Alt_03_005.png",
			0.50, Pos2<float>(GLOBAL_MAX_X, (startingYAreaLo + posModifier()), 0, defaultCheck<float>))) ;
	
	SharedGameData::initData(GameObject::getAllGameObjects(), GameObject::getMap()) ;
	
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
		worldControllerLoopCount++ ;
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

void WorldController::update() {
	obstacleBehavior() ;
	/* other actor behavior */
}

void WorldController::obstacleBehavior() {
	/* So we can alter our behavior each time through the loop */
	bool even = ((worldControllerLoopCount % 2) == 0) ;
	
	FastRand<float> randOffsetter(-defaultOffset<float>, defaultOffset<float>) ;
	auto randOffsetVal = (randOffsetter() * 30) ;
	
	for (auto i = 0 ; i < obstacleObjects->size() ; i++) {
		if (even) {
			obstacleObjects->at(i)->moveUp(randOffsetVal) ; /* will actually move us up or down (randomly) */
			obstacleObjects->at(i)->moveUp(randOffsetVal * (-1)) ; /* will put us back where we started longtitudinally, though
																	laterally we've still moved to the left  */
		}
		else {
			obstacleObjects->at(i)->moveLeft(defaultOffset<float> * 2) ;
		}
	}
}



