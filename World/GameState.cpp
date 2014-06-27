//
//  GameState.cpp
//  World
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
SDL_Window * GameState::window = nullptr ;
SDL_Renderer * GameState::renderer = nullptr ;

bool GameState::GLOBAL_CONTINUE_SIGNAL = true ;
Time * GameState::mainGameClock = new Time() ;
std::mutex GameState::sharedMutex ;
string GameState::currentDirectory ;



void GameState::initData(vector<GameObject *> * gobs, const GameMap<GameObject> * map) {
	GameState::gameObjects = gobs ;
	GameState::map = map ;
	dataIsInit = true ;
}

void GameState::initGraphics(SDL_Window * window, SDL_Renderer * renderer) {
	GameState::window = window ;
	GameState::renderer = renderer ;
	graphicsAreInit = true ;
}


const vector<GameObject*> * GameState::getGameObjects()  {
	if (dataIsInit == false) {
		stringstream ss ;
		ss << "GameState::initData() must be called before reading any of its data members \n" ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
	return gameObjects ;
}


const GameMap<GameObject> * GameState::getMap()  {
	if (dataIsInit == false) {
		stringstream ss ;
		ss << "GameState::initData() must be called before reading any of its data members \n" ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
	return map ;
}

SDL_Window * GameState::getMainWindow() {
	if (graphicsAreInit == false) {
		stringstream ss ;
		ss << "GameState::initGraphics() must be called before reading GameState::getMainRenderer() \n" ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
	return window ;
}

SDL_Renderer * GameState::getMainRenderer() {
	if (graphicsAreInit == false) {
		stringstream ss ;
		ss << "GameState::initGraphics() must be called before reading GameState::getMainRenderer() \n" ;
		cerr << ss.rdbuf() ;
		throw exception() ;
	}
	return renderer ;
}


unsigned mainGameLoopCount = 0 ;
unsigned worldLoopCount = 0 ;




//static bool GLOBAL_CONTINUE_SIGNAL = true ;

