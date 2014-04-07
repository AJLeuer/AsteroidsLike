//
//  main.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include <ncurses.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <thread>

#include "../Util/BasicTime.h"

#include "../Adapter/Adapter.h"
#include "../Adapter/TestAdapter.h"

#include "World.h"
#include "GameObject.h"
#include "NPC.h"



using namespace std ;

int foo() {
	return 1 ;
}

int main(void) {

	srand((unsigned)time(NULL)) ;
	
	
	BasicTime timer ;
	
	timer.startTimer() ;

	
	World::init() ;
	
	AdapterInterface * chosenOutputAdapter = new TestAdapter(1) ;
	
	
	chosenOutputAdapter->init() ;

	chosenOutputAdapter->show() ;
	
	bool run = true ;
	
	World::foo(10e+6, &run) ;
	
	usleep(10e+6) ;
	
	run = false ;
	
	World::close() ;
	
	chosenOutputAdapter->close() ;
	
	return 0;
}

