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

#include "../Util/Util.h"
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
	
	/*
	BasicTime timer ;
	
	timer.startTimer() ;

	
	WorldController::init() ;
	
	AdapterInterface * chosenOutputAdapter = new Adapter(1) ;
	
	
	chosenOutputAdapter->init() ;

	chosenOutputAdapter->show() ;
	
	bool run = true ;
	
	WorldController::foo(1, 30e+6, &run) ;
	
	usleep(30e+6) ;
	
	run = false ;
	
	WorldController::close() ;
	
	chosenOutputAdapter->close() ;
	*/
	
	bool bl = true ;
	
	
	WorldController::init() ;
	WorldController::foo(5, 9e+110, & bl) ;
	 
	
	cout << Debug::debugCounter << endl ;
	return 0;
}

