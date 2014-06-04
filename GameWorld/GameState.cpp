//
//  GameState.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/14/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <iostream>

#include "GameState.h"

using namespace std ;

bool GameState::dataIsInit = false ;
bool GameState::graphicsAreInit = false ;
const vector<GameObject *> * GameState::gameObjects = nullptr ;
const GameMap<GameObject> * GameState::map ;
SDL_Renderer * GameState::renderer = nullptr ;


void GameState::initData(vector<GameObject *> * gobs, const GameMap<GameObject> * map) {
	GameState::gameObjects = gobs ;
	GameState::map = map ;
	dataIsInit = true ;
}

void GameState::initGraphics(SDL_Renderer * renderer_) {
	renderer = renderer_ ;
	graphicsAreInit = true ;
}


const vector<GameObject*> * GameState::getGameObjects()  {
	if (dataIsInit == false) {
		stringstream ss ;
		ss << "GameState::initData() must be called before reading any of its data members \n" ;
		*Debug::debugOutput << ss.rdbuf() ;
		throw exception() ;
	}
	return gameObjects ;
}


const GameMap<GameObject> * GameState::getMap()  {
	if (dataIsInit == false) {
		stringstream ss ;
		ss << "GameState::initData() must be called before reading any of its data members \n" ;
		*Debug::debugOutput << ss.rdbuf() ;
		throw exception() ;
	}
	return map ;
}

SDL_Renderer * GameState::getMainRenderer() {
	if (graphicsAreInit == false) {
		stringstream ss ;
		ss << "GameState::initGraphics() must be called before reading GameState::getMainRenderer() \n" ;
		*Debug::debugOutput << ss.rdbuf() ;
		throw exception() ;
	}
	return renderer ;
}


unsigned mainGameLoopCount = 0 ;
unsigned worldLoopCount = 0 ;

Time<nanosecPrecisionClock> * mainGameClock = nullptr ;

std::mutex sharedMutex ;

bool GLOBAL_CONTINUE_SIGNAL = true ;

string currentDirectory = "" ;
