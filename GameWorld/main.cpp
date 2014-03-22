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

int foo() {
	return 1 ;
}

int main(void) {

	srand((unsigned)time(NULL)) ;
	/*
	BasicTime timer ;
	
	timer.startTimer() ;
	
	
	
	World::init() ;
	
	
	
	AdapterInterface * chosenOutputAdapter = new TestAdapter(1) ;
	
	
	chosenOutputAdapter->init() ;
	(*chosenOutputAdapter)() ;
	
	//std::thread thread1(chosenOutputAdapter); //should call show()
	
	//thread1.join() ;

	
	World::close() ;
	
	chosenOutputAdapter->close() ;
	 
	 
	
	auto timeElapsed = timer.stopTimer() ;
	 */
	
	NPC * npc0 = new NPC(rand()) ;
	NPC * npc1 = new NPC(rand()) ;
	
	BasicTime timer ;
	timer.startTimer() ;
	
	npc0->wander(10, 1e+7) ;
	npc1->wander(10, 1e+7) ;
	
	while (timer.checkTimeElapsed() < 1.1e+7) {
		cout << *npc0 << endl ;
		cout << *npc1 << endl ;
		usleep(1e+6) ;
	}
	
	delete npc0 ;
	delete npc1 ;
		
	return 0;
}

