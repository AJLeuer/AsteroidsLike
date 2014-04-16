//
//  Debug.h
//  GameWorld
//
//  Created by Adam James Leuer on 4/6/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__Debug__
#define __GameWorld__Debug__

#include <iostream>
#include <ostream>
#include <fstream>


using namespace std ;

class Debug {
	
public:
	static ostream * debugOutput ;
	
	static void init(bool stdoutput) ;
	
	
	
	/* Use for whatever */
	static unsigned debugCounter ;
	
} ;




	

#endif /* defined(__GameWorld__Debug__) */
