//
//  WorldControl.cpp
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#include "WorldControl.h"

using namespace std ;


//vector<GameObject*> * WorldControl::allGameObjects = GameObject::allGameObjects ;


thread WorldControl::worldEventThread ;


const GameMap<GameObject> * WorldControl::map = nullptr ;

WorldControl::WorldControl() {}


void WorldControl::init() {
    
	Vect<float> * pos = new Vect<float>(0, 0) ;
    
	GraphicsData<float, int> * backdrop = new GraphicsData<float, int>(AssetFile::backgroundImageFilenames->at(0), pos, 0.0, 1.0, PositionType::screenPosition, Util::SafeBoolean::t, Util::SafeBoolean::f, Util::SafeBoolean::f, Util::SafeBoolean::f) ;
	
	//backdrop->disableCollisionDetection() ;

	WorldControl::map = GameObject::getMap() ;

	Randm<int> posModifier(-100, 100) ;

	
	/* Init enemies */
	/*
	new Enemy(AssetType::character, AssetFileIO::getRandomImageFilename(AssetType::character),
              0.50, Vect<float>(globalMaxX() - 200, (startingYAreaHi + posModifier()), 0, defaultCheck<float>())) ;

	new Enemy(AssetType::character, AssetFileIO::getRandomImageFilename(AssetType::character),
              0.50, Vect<float>(globalMaxX() - 200, (startingYAreaLo + posModifier()), 0, defaultCheck<float>())) ;
	*/
	/* Init obstacles */
	
	Randm<float> randomSizeModifier(0.25, 1.0) ;
	
	Randm<unsigned> randm(0, 100) ;
	
	/* Create asteroids */
	for (auto i = 0 ; i < 5 ; i++) {
		new GameObject(AssetFileIO::getRandomImageFile(AssetType::asteroid), 0.50,
					   Vect<float>(* Randm<float>::randPositionSetter, BoundsCheck<float>::defaultCheck), Angle(0), true, Util::SafeBoolean::f, Util::SafeBoolean::t, Util::SafeBoolean::t) ;
	}
	
	/* Init game state */
	GameState::initData(GameObject::accessAllGameObjects(), GameObject::getMap()) ;
    
}

void WorldControl::begin_main() {
	
	worldEventThread = thread(& WorldControl::main) ;  //runWorldSimulation()
	//no need to detach, joined later
	
}

void WorldControl::main() {
	
	auto * rt = & refreshTime ; //debug symbol
    
	bool * cont = & GLOBAL_CONTINUE_FLAG ; //debug variable - rm
	
	while (GLOBAL_CONTINUE_FLAG) {
		

		auto startTime = GameState::mainGameClock->checkTimeElapsed() ;
		
		if (timeFlow == TimeFlow::forward) {
			main_forwardTime() ;
		}
		else if (timeFlow == TimeFlow::reverse) {
			main_reverseTime() ;
		}
		
		/* Every once it a while it will be necessary to clean out the main game object container to
		   remove the accumulated null pointers */
		if ((worldLoopCount % 384) == 0) {
            auto temporaryGameObjects = Util::copyWithoutNullValues(GameObject::accessAllGameObjects(), new vector<GameObject*>) ;
			delete ( GameObject::accessAllGameObjects() ) ;
			GameObject::accessAllGameObjects() = temporaryGameObjects ;
		}
		
		auto * mloop = &mainGameLoopCount ; //debug var, delete this
		auto * wloop = &worldLoopCount ; //debug var, delete
		
		auto time2 = GameState::mainGameClock->checkTimeElapsed() ;
		auto timeElapsed = time2 - startTime ;
		auto sleepTime = refreshTime - timeElapsed ;
		
		worldLoopCount++ ;
		
		this_thread::sleep_for(sleepTime) ;
		
		if (worldLoopCount > mainGameLoopCount) {
			unique_lock<mutex> locked(mainThreadsSync) ;
			
			shared_conditional.wait(locked) ;
		}
		
		shared_conditional.notify_all() ;
	}
}

void WorldControl::main_forwardTime() {
	/* Do stuff */
	static unsigned calls = 0 ;
	
	for (auto i = 0 ; i < GameObject::accessAllGameObjects()->size() ; i++) {
		
		if ((GameObject::accessAllGameObjects()->at(i) != nullptr) && (GameObject::accessAllGameObjects()->at(i)->accessMutex().try_lock())) {
			
			GameObject * current = GameObject::accessAllGameObjects()->at(i) ;

			auto allGOs = GameObject::accessAllGameObjects() ; //debug var
			
			//if this is the first time calling
			if (calls == 0) {
				current->doDefaultBehavior(true) ;
			}
			else {
				current->doDefaultBehavior(false) ;
			}
			
			/* do any other stuff with GameObjects */
			
			/* always call update at the end */
			current->update() ;
			
			current->accessMutex().unlock() ;
		}
	}
	
	calls++ ;
}

void WorldControl::main_reverseTime() {
	
	for (auto i = 0 ; i < GameObject::accessAllGameObjects()->size() ; i++) {
		
		if (GameObject::accessAllGameObjects()->at(i) != nullptr) {
			
			GameObject::accessAllGameObjects()->at(i)->reversePreviousAction() ;
			
		}
	}
}


void WorldControl::exit() {
	
	GameState::mainMutex.lock() ; //we don't want our Adapter thinking its safe to read our GameObjects any more
	
	shared_conditional.notify_all() ; //so no one is left waiting on shared_conditional
	worldEventThread.join() ;
	
	for (auto i = 0 ; i < GameObject::accessAllGameObjects()->size() ; i++) {
        if (GameObject::accessAllGameObjects()->at(i) != nullptr) {
			//delete gameObjects->at(i) ;
		}
	}

	delete GameObject::accessAllGameObjects() ;
	delete map ; 
    GameObject::accessAllGameObjects() = nullptr ;
	
	
	GameState::mainMutex.unlock() ;
}






