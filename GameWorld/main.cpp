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
#include "../Adapter/Adapter.h"


using namespace std ;

int main(void)
{
	
	World::init() ;
	Adapter::init() ;
	
	usleep(500000000) ;
	
	World::running = false ;
	Adapter::close() ;
	
	return 0;
}

