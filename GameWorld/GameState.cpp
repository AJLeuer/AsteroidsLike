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

bool GameState::GLOBAL_CONTINUE_SIGNAL = true ;
Time * GameState::mainGameClock = new Time() ;
std::mutex GameState::sharedMutex ;
string GameState::currentDirectory ;

void GameState::initCoordinateSystems() {
    worldCoordinateSystem<float> = new CoordinateSystem<float>(GLOBAL_MAX_X, GLOBAL_MAX_X) ;
    screenCoordinateSystem<float> = new CoordinateSystem<float>(WINDOW_MAX_X, WINDOW_MAX_Y) ;
    
    float originX = 0.5 * (GLOBAL_MAX_X - WINDOW_MAX_X) ;
    float originY = 0.5 * (GLOBAL_MAX_Y - WINDOW_MAX_Y) ;
    
    screenCoordsAsWorldCoords<float> = new CoordinateSystem2<float>(*screenCoordinateSystem<float>, worldCoordinateSystem<float>,
																	{originX, originY}) ;
	originX = 0.5 * (WINDOW_MAX_X - GLOBAL_MAX_X) ;
	originY = 0.5 * (WINDOW_MAX_Y - GLOBAL_MAX_Y) ;
	
	worldCoordsAsScreenCoords<float> = new CoordinateSystem2<float>(*worldCoordinateSystem<float>, screenCoordinateSystem<float>,
																	{originX, originY}) ;
}

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

template<typename N>
CoordinateSystem<N> * worldCoordinateSystem = nullptr ;

template<typename N>
CoordinateSystem<N> * screenCoordinateSystem = nullptr ;

template<typename N>
CoordinateSystem2<N> * worldCoordsAsScreenCoords = nullptr ;

template<typename N>
CoordinateSystem2<N> * screenCoordsAsWorldCoords = nullptr ;


//static bool GLOBAL_CONTINUE_SIGNAL = true ;

