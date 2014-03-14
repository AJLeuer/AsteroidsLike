//
//  GameEvent.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "GameEvent.h"

unsigned GameEvent::IDs = 0 ;


GameEvent::GameEvent() :
	ID(IDs)
{
	IDs++ ;
}


GameEvent::GameEvent(const GameEvent & other) :
	GameEvent()
{
	this->icon = other.icon ;
}


GameEvent::GameEvent(string symbol) :
	GameEvent()
{
	this->icon = symbol ;
}


GameEvent::~GameEvent() {
	//todo
}


GameEvent & GameEvent::operator=(const GameEvent & rhs) {
	if (this != &rhs) {
		this->ID = IDs ;
		IDs++ ;
		this->icon = rhs.icon ;
	}
	return *this ;
}

void GameEvent::operator()() {
	//todo
}

void GameEvent::notify() {
	//todo
}

string GameEvent::draw() {
	return this->icon ;
}