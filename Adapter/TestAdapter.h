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

#include "AdapterUtil.h"
#include "AdapterInterface.h"

#include "Location.h"
#include "../GameWorld/GameObject.h"
#include "../GameWorld/World.h"

using namespace std ;

class TestAdapter : public AdapterInterface {
	
private:
	
	TestAdapter() {}
	
public:
	
	TestAdapter(int) ;
	
	void init() ;
	
	void show() ;
	
	void close() ;
	
} ;

#endif /* defined(__GameWorld__TestAdapter__) */
