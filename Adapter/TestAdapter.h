//
//  TestAdapter.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/19/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__TestAdapter__
#define __GameWorld__TestAdapter__

#include <ncurses.h>
#include <unistd.h>

#include <iostream>
#include <thread>

#include "AdapterUtil.h"
#include "AdapterInterface.h"

#include "Util.h"
#include "Location.hpp"
#include "../GameWorld/GameObject.h"
#include "../GameWorld/WorldController.h"

using namespace std ;

class TestAdapter : public AdapterInterface {
	
private:
	
	void show_threaded() ;

	
public:
	TestAdapter() : AdapterInterface() {}
	
	TestAdapter(int) : AdapterInterface(this) {}
	
	TestAdapter(TestAdapter &) : AdapterInterface() {}
	
	TestAdapter(TestAdapter &&) : AdapterInterface(this) {}
	
	/**
	 * Assignment operator overload (copy)
	 */
	TestAdapter & operator=(const TestAdapter &) { return *this ; }
	
	/**
	 * Assignment operator overload (move)
	 */
	TestAdapter & operator=(TestAdapter &&) { AdapterInterface(this) ; return *this ; }
	
	~TestAdapter() {} //should automatically call ~AdapterInterface()
	
	void init() ;
	
	void show() ;
	
	void operator()() ;
	
	void close() ;
	
} ;

#endif /* defined(__GameWorld__TestAdapter__) */
