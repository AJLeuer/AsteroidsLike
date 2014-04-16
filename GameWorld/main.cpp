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

#include "../Util/Util.hpp"
#include "../Util/BasicTime.h"

#include "../Adapter/Adapter.h"
#include "../Adapter/TestAdapter.h"

#include "WorldController.h"
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
	//Debug::init(true) ;
	
	WorldController::init() ;
	
	AdapterInterface * chosenOutputAdapter = new TestAdapter(1) ;
	
	
	chosenOutputAdapter->init() ;

	chosenOutputAdapter->show() ;
	
	bool run = true ;
	
	WorldController::foo(1, 0e+6, &run) ;
	
	usleep(0e+6) ;
	
	run = false ;
	
	WorldController::close() ;
	
	chosenOutputAdapter->close() ;
	
	

	Debug::init(false) ;
	
	//*(Debug::debugOutput) << Debug::debugCounter ;

	
	return 0;
}

