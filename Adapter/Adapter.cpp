//
//  Adapter.cpp
//  GameWorld
//
//  Created by Adam James Leuer on 3/18/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#include "Adapter.h"

Adapter::Adapter(int n) :
	AdapterInterface(this, n) {}

void Adapter::init() {
	this->AdapterInterface::init() ;
	
	//local initializations:
	setlocale(LC_ALL, ""); //allows printing more types of characters (?)
	
	initscr();  //this inits and returns a reference to a WINDOW*, which we don't need
	
	cbreak() ; //character-at-a-time  input  without buffering
	noecho() ; //wont print input back out
	
	curs_set(0) ; //sets cursor to invisible
}

void Adapter::show() {
	while (World::isRunning()) {
		for (auto i = 0 ; i < (*WorldObjects)->size() ; i++) {
			Location trans = AdapterUtil::transLocation(*((*WorldObjects)->at(i)->getLocation())) ;
			mvwaddstr(stdscr, trans.y, trans.x, "H"/*(*WorldObjects)->at(i)->draw().c_str()*/) ;
		}
		wrefresh(stdscr) ;
	}
}

void Adapter::close() {
	this->AdapterInterface::close() ;
	
	//local cleanup:
	endwin() ;
}