//
//  WorldController.cpp
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#include "WorldController.h"

using namespace std ;


thread WorldController::mainThread ;
thread WorldController::checkDelThread ;
const GameMap<GameObject> * WorldController::map = nullptr ;

WorldController::WorldController() {}


void WorldController::init() {
    
	Position<float> * pos = new Position<float>(0, 0) ;
    
	GraphicsData<float, int> * backdrop = new GraphicsData<float, int>(AssetFile::backgroundImageFilenames->at(0), pos, 0.0, 1.0, PositionType::screenPosition) ;

	WorldController::map = GameObject::getMap() ;

	Randm<int> posModifier(-100, 100) ;

	
	/* Init enemies */
	/*
	new Enemy(AssetType::character, AssetFileIO::getRandomImageFilename(AssetType::character),
              0.50, Pos2<float>(globalMaxX() - 200, (startingYAreaHi + posModifier()), 0, defaultCheck<float>())) ;

	new Enemy(AssetType::character, AssetFileIO::getRandomImageFilename(AssetType::character),
              0.50, Pos2<float>(globalMaxX() - 200, (startingYAreaLo + posModifier()), 0, defaultCheck<float>())) ;
	*/
	/* Init obstacles */
	
	Randm<float>randomSizeModifier(0.25, 1.0) ;
	
	Randm<int> randm(0, 100) ;
	
	for (auto i = 0 ; i < 10 ; i++) {
		new GameObject(AssetFileIO::getRandomImageFile(AssetType::asteroid), 0.50,
					   Pos2<float>(*Randm<float>::randPositionSetter, BoundsCheck<float>::defaultCheck), Angle(0), true, SafeBoolean::f, true) ;
	}
	
	/* Init game state */
	GameState::initData(GameObject::getAllGameObjects(), GameObject::getMap()) ;
    
}

void WorldController::begin_main() {
	mainThread = thread(&WorldController::main) ;  //runWorldSimulation()
	checkDelThread = thread(&GameObject::checkForMarkedDeletions) ;
}

void WorldController::main() {
	
	auto * rt = &refreshTime ; //debug symbol
    
	bool * cont = &GLOBAL_CONTINUE_FLAG ; //debug variable - rm
	
	while (GLOBAL_CONTINUE_FLAG) {
		

		auto startTime = GameState::mainGameClock->checkTimeElapsed() ;
		
		if (timeFlow == TimeFlow::forward) {
			main_forwardTime() ;
		}
		else if (timeFlow == TimeFlow::reverse) {
			main_reverseTime() ;
		}
		
		auto * mloop = &mainGameLoopCount ; //debug var, delete this
		auto * wloop = &worldLoopCount ; //debug var, delete
		
		auto time2 = GameState::mainGameClock->checkTimeElapsed() ;
		auto timeElapsed = time2 - startTime ;
		auto sleepTime = refreshTime - timeElapsed ;
		
		worldLoopCount++ ;
		
		this_thread::sleep_for(sleepTime) ;
		
		if (worldLoopCount > mainGameLoopCount) {
			unique_lock<mutex> locked(syncMutex) ;
			
			conditionalWait.wait(locked) ;
		}
		
		conditionalWait.notify_all() ;
	}
}

void WorldController::main_forwardTime() {
	/* Do stuff */
    GameObject::allDoDefaultBehaviors(TimeFlow::forward) ;
}

void WorldController::main_reverseTime() {

}


void WorldController::exit() {
	
	GameState::mainMutex.lock() ; //we don't want our Adapter thinking its safe to read our GameObjects any more
	
	checkDelThread.join() ;
	
	mainThread.join() ;
	
	for (auto i = 0 ; i < GameObject::getAllGameObjects()->size() ; i++) {
        if (GameObject::getAllGameObjects()->at(i) != nullptr) {
			//delete gameObjects->at(i) ;
		}
	}

	delete GameObject::getAllGameObjects() ;
	delete map ; 
    GameObject::getAllGameObjects() = nullptr ;
	
	GameState::mainMutex.unlock() ;
}






