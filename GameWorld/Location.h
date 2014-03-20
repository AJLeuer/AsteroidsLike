//
//  Header.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef GameWorld_Location_h
#define GameWorld_Location_h

#include <iostream>
#include <sstream>

using namespace std ;

/**
 * A relatively simple data structure representing a location vector.
 *
 * Note: Classes with a location data member will typically want to have a pointer,
 * instead of holding the Location locally. This is because many objects in the GameWorld
 * may not actually have a physcical location in space, in which case they can just hold a 
 * null pointer.
 */
struct Location {
	
	double x ;
	double y ;
    double z ;
	
	/**
     * Creates a location with all coordinates initialized to 0
     */
	Location() : x(0), y(0), z(0) {}
    
    /**
     * Copy constructor for Location
     */
    Location(const Location & other) : x{other.x}, y{other.y}, z{other.z} {}
    
    /**
     * Creates a location with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
	Location(double x, double y, double z) : x{x}, y{y}, z{z} {}
    
    /**
     * Destructor for Location
     */
    ~Location() {}
    
    /**
     * Assigment operator overload
     */
    Location & operator=(const Location & rhs) {
        if (this != &rhs) {
            this->x = rhs.x ;
            this->y = rhs.y ;
            this->z = rhs.z ;
        }
        return *this ;
    }
	
	std::string toString() {
		stringstream ss ;
		ss << "x = " << x << ", y = " << y << ", z = " << z ;
		return ss.str() ;
	}
};


#endif
