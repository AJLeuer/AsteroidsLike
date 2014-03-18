//
//  World.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "World.h"

vector<NPC> * World::testNPCs = nullptr ;

World::World() {}

void World::init() {
	//testing code
	testNPCs = new vector<NPC>() ;
	for (vector<NPC>::size_type i = 0 ; i < 15 ; i++) {
		testNPCs->push_back(NPC(rand())) ;
	}
	//end testing
}

void World::playGameInRealTime() {
	//todo
}

void World::playGameRecorded(std::ostream *writeTO) {
	//testing code
	for (vector<NPC>::size_type i = 0 ; i < 15 ; i++) {
		testNPCs->at(i).textDescription(writeTO) ;
		*writeTO << endl << endl ;
	}
	//end testing
}
