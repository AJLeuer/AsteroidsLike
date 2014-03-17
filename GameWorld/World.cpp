//
//  World.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "World.h"

NPC * World::testNPC = nullptr ;

World::World() {}

void World::init() {
	testNPC = new NPC("👩", "Kathy", true, CharacterState::idle, Health(100, 0), Damage(20, 0), Reaction::neutral) ;
	cout << testNPC->draw() << endl ;
}
