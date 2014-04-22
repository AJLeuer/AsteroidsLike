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
#include "../Util/Time.h"

#include "../Output/Adapter.hpp"
#include "../Output/TestAdapter.hpp"

#include "MainController.h"
#include "GameObject.h"
#include "WorldController.h"
#include "NPC.h"


using namespace std ;

int foo() {
	return 1 ;
}



int main(void) {

	Time timer ;
	timer.startTimer() ;
	
	
	MainController::start(GLOBAL_CONTINUE_SIGNAL) ;
	usleep(1e7) ; // sleep 1.0 x 10⁷ microseconds or 10 seconds
	MainController::stop(GLOBAL_CONTINUE_SIGNAL) ;
	
	
	/* end debug */
	return 0;
}

