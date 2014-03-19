//
//  Adapter.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/18/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Adapter.h"

using namespace std ;

WINDOW * Adapter::gameWindow = nullptr ;

vector<GameObject*> * Adapter::WorldObjects = nullptr ;

void Adapter::init() {
	
	if (World::running) {
		WorldObjects = &(World::allObjects) ;
	}
	
	/*
	unsigned y, x;
	y = AdapterUtil::termHeight() ;
	x = AdapterUtil::termWidth() ;
	
	gameWindow = newwin(y, x, 0, 0) ;
	*/
	
	setlocale(LC_ALL, ""); //allows printing more types of characters (?)
	
	initscr();  //don't need this because we're using our own window
	
	cbreak() ; //character-at-a-time  input  without buffering
	noecho() ; //wont print input back out
	
	curs_set(0) ; //sets cursor to invisible
	
	
	
	
	//signal(SIGWINCH, Adapter::resizeHandler()); //later
	
}

//maybe finish this another time
/*
void* Adapter::resizeHandler(){
	int nh, nw;
 getmaxyx(stdscr, nh, nw);  // get the new screen size
	wattron() ;
}
*/

void Adapter::start() {
	while (World::running) {
		for (auto i = 0 ; i < WorldObjects->size() ; i++) {
			Location trans = AdapterUtil::transLocation(*(WorldObjects->at(i)->getLocation())) ;
			mvwaddstr(stdscr, trans.y, trans.x, WorldObjects->at(i)->draw().c_str()) ;
			wrefresh(stdscr) ;
			
		}
	}
}

void Adapter::close() {
	delwin(gameWindow) ;
}