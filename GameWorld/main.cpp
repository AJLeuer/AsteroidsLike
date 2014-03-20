//
//  main.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <ncurses.h>
#include <unistd.h>

#include <iostream>

#include "World.h"
#include "GameObject.h"
#include "NPC.h"
#include "../Adapter/Adapter.h"
#include "../Adapter/TestAdapter.h"


using namespace std ;

int main(void)
{
	srand((unsigned)time(NULL)) ;
	
	/*
	AdapterInterface * chosenOutputAdapter = new TestAdapter(1) ;
	
	World::init() ;
	chosenOutputAdapter->init() ;
	
	chosenOutputAdapter->show() ;
	
	
	World::close() ;
	chosenOutputAdapter->close() ;
	 */
	
	NPC npc = NPC(rand()); //rnd
	cout << npc.toString()->c_str() << endl ;
	
	npc.setName("hi") ;
	
	cout << npc.toString()->c_str() << endl ;
	
	return 0;
}

