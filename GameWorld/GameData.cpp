//
//  GameData.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 4/14/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <iostream>

#include "GameData.h"


using namespace std ;

bool SharedGameData::dataIsInit = false ;
bool SharedGameData::graphicsAreInit = false ;
const vector<GameObject *> * SharedGameData::gameObjects = nullptr ;
const GameMap<GameObject> * SharedGameData::map ;
SDL_Renderer * SharedGameData::renderer = nullptr ;


void SharedGameData::initData(vector<GameObject *> * gobs, const GameMap<GameObject> * map) {
	SharedGameData::gameObjects = gobs ;
	SharedGameData::map = map ;
	dataIsInit = true ;
}

void SharedGameData::initGraphics(SDL_Renderer * renderer_) {
	renderer = renderer_ ;
	graphicsAreInit = true ;
}


const vector<GameObject*> * SharedGameData::getGameObjects()  {
	if (dataIsInit == false) {
		stringstream ss ;
		ss << "SharedGameData::initData() must be called before reading any of its data members" << '\n';
		*Debug::debugOutput << ss.rdbuf() ;
		throw exception() ;
	}
	return gameObjects ;
}


const GameMap<GameObject> * SharedGameData::getMap()  {
	if (dataIsInit == false) {
		stringstream ss ;
		ss << "SharedGameData::initData() must be called before reading any of its data members" << '\n' ;
		*Debug::debugOutput << ss.rdbuf() ;
		throw exception() ;
	}
	return map ;
}

SDL_Renderer * SharedGameData::getMainRenderer() {
	if (graphicsAreInit == false) {
		stringstream ss ;
		ss << "SharedGameData::initGraphics() must be called before reading SharedGameData::getMainRenderer()" << '\n' ;
		*Debug::debugOutput << ss.rdbuf() ;
		throw exception() ;
	}
	return renderer ;
}

std::mutex sharedMutex ;

bool GLOBAL_CONTINUE_SIGNAL = true ;

const int GLOBAL_MAX_X { GLOBAL_MAX_X_ } ;
const int GLOBAL_MIN_X { GLOBAL_MIN_X_ } ;
const int GLOBAL_MAX_Y { GLOBAL_MAX_Y_ } ;
const int GLOBAL_MIN_Y { GLOBAL_MIN_Y_ } ;

string currentDirectory { "" } ;





