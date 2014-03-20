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
#include <thread>

#include "../Util/BasicTime.h"

#include "../Adapter/Adapter.h"
#include "../Adapter/TestAdapter.h"

#include "World.h"
#include "GameObject.h"
#include "NPC.h"



using namespace std ;

int main(void)
{
	srand((unsigned)time(NULL)) ;
	
	BasicTime timer ;
	
	timer.startTimer() ;
	
	
	AdapterInterface * chosenOutputAdapter = new TestAdapter(1) ;
	
	std::thread first(chosenOutputAdapter) ;
	World::init() ;
	chosenOutputAdapter->init() ;
	
	chosenOutputAdapter->show() ;
	
	
	World::close() ;
	chosenOutputAdapter->close() ;
	 
	 
	
	auto timeElapsed = timer.stopTimer() ;
		
	return 0;
}

