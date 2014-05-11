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

bool SharedGameData::isInit = false ;
const vector<GameObject *> * SharedGameData::gameObjects = nullptr ;
const GameMap<GameObject> * SharedGameData::map ;

void SharedGameData::initData(vector<GameObject *> * gobs, const GameMap<GameObject> * map) {
	SharedGameData::gameObjects = gobs ;
	SharedGameData::map = map ;
	isInit = true ;
}


const vector<GameObject*> * SharedGameData::getGameObjects()  {
	if (isInit == false) {
		stringstream ss ;
		ss << "SharedGameData::init() must be called before reading any of its data members" << '/n' ;
		*Debug::debugOutput << ss.rdbuf() ;
		throw exception() ;
	}
	return gameObjects ;
}


const GameMap<GameObject> * SharedGameData::getMap()  {
	if (isInit == false) {
		stringstream ss ;
		ss << "SharedGameData::init() must be called before reading any of its data members" << '/n' ;
		*Debug::debugOutput << ss.rdbuf() ;
		throw exception() ;
	}
	return map ;
}


std::mutex sharedMutex ;

extern bool GLOBAL_CONTINUE_SIGNAL = true ;

extern const long GLOBAL_MAX_X { GLOBAL_MAX_X_ } ;
extern const long GLOBAL_MIN_X { GLOBAL_MIN_X_ } ;
extern const long GLOBAL_MAX_Y { GLOBAL_MAX_Y_ } ;
extern const long GLOBAL_MIN_Y { GLOBAL_MIN_Y_ } ;

extern string currentDirectory = "" ;





