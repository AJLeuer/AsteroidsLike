//
//  Header.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_Location_h
#define GameWorld_Location_h


struct Location {
	
	double x ;
	double y;
	
	
	Location() : x(0), y(0) {}
	Location(double x, double y) : x{x}, y{y} {}
	
};


#endif
