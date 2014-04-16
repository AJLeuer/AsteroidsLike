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

#include "Debug.h"
#include "Util.h"

#include "../GameWorld/GameData.h"





using namespace std ;

enum Direction {
	north = 'n',
	south = 's',
	east = 'e',
	west = 'w',
	ne = north + east,
	se = south + east,
	sw = south + west,
	nw = north + west,
	oneDirection //the best direction!
} ;



/**
 * A relatively simple data structure representing a location vector.
 *
 * Note: Classes with a location data member will typically want to have a pointer,
 * instead of holding the Location locally. This is because many objects in the GameWorld
 * may not actually have a physcical location in space, in which case they can just hold a 
 * null pointer.
 */
template <typename N>
struct Location {
private:
	void boundsCheck() {
		if (this->x >= (*Location<N>::MAX_X)) {
			*(Debug::debugOutput) << "An X value was over the global limit. Reducing value..." << endl ;
			this->x = *MAX_X - 1 ;
		}
		if (this->x < (*Location<N>::MIN_X)) {
			*(Debug::debugOutput) << "An X value was under the global minimum. Increasing value..." << endl ;
			this->x = *MIN_X ;
		}
		if (this->y >= (*Location<N>::MAX_Y)) {
			*(Debug::debugOutput) << "A Y value was over the global limit. Reducing value..." << endl ;
			this->y = *MAX_Y - 1 ;
		}
		if (this->y < (*Location<N>::MIN_Y)) {
			*(Debug::debugOutput) << "A Y value was under the global minimum. Increasing value..." << endl ;
			this->y = *MIN_Y ;
		}
	}

public:
	
	static const N * MAX_X ;
	static const N * MIN_X ;
	static const N * MAX_Y ;
	static const N * MIN_Y ;
	//static const N * MAX_Z ;
	//static const N * MIN_Z ;
	
	N x ;
	N y ;
    N z ;
	
	/**
     * Creates a Locationwith all coordinates initialized to 0
     */
	Location() : x(0), y(0), z(0) { this->boundsCheck() ; }
    
    /**
     * Copy constructor for Location
     */
    Location(const Location & other) : Location(other.x, other.y, other.z)  {}
	
	/**
     * Move constructor for Location
     */
    Location(Location && other) : Location(other.x, other.y, other.z) {}
    
    /**
     * Creates a location with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
	Location(N x, N y, N z) : x{x}, y{y}, z{z} {
		this->boundsCheck() ;
	}
	
	Location(N nx, N ny, N nz, bool noBoundsCheck) {
		if (!noBoundsCheck) {
			this->x = nx ;
			this->y = ny ;
			this->z = nz ;
			this->boundsCheck() ;
		}
		else {
			this->x = nx ;
			this->y = ny ;
			this->z = nz ;
		}
	}
    
    /**
     * Destructor for Location
     */
    ~Location() {}
    
    /**
     * Assigment operator overload (copy)
     */
    Location & operator=(const Location & rhs) {
        if (this != &rhs) {
            this->x = rhs.x ;
            this->y = rhs.y ;
            this->z = rhs.z ;
        }
		this->boundsCheck() ;
		return *this;
    }
	
	/**
     * Assigment operator overload (move)
     */
    Location & operator=(Location && rhs) {
        if (this != &rhs) {
            this->x = rhs.x ;
            this->y = rhs.y ;
            this->z = rhs.z ;
        }
		this->boundsCheck() ;
		return(*this) ;
    }
	
	bool operator==(const Location & rhs) {
		if ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z)) {
			return true ;
		}
		else {
			return false ;
		}
	}
	
	bool operator==(Location & rhs) {
		if ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z)) {
			return true ;
		}
		else {
			return false ;
		}
	}
	
	bool operator!=(const Location & rhs) {
		return !(this->operator==(rhs)) ;
	}
	
	bool operator!=(Location & rhs) {
		return !(this->operator==(rhs)) ;
	}
	
	
	Location operator+(const Location & rhs) {
		N x = this->x + rhs.x ;
		N y = this->y + rhs.y ;
		N z = this->z + rhs.z ;
		Location temp(x, y, z) ;
		return temp ;
	}
	
	Location operator-(const Location & rhs) {
		N x = this->x - rhs.x ;
		N y = this->y - rhs.y ;
		N z = this->z - rhs.z ;
		Location temp(x, y, z) ;
		return temp ;
	}
	
	
	N getX() const { return this->x ; }
	
	N getY() const { return this->y ; }
	
	N getZ() const { return this->z ; }
	
	void setX(const N x) { this->x = x ; this->boundsCheck() ; }
	
	void setY(const N y) { this->y = y ; this->boundsCheck() ; }
	
	void setZ(const N z) { this->z = z ; this->boundsCheck() ; }
	
	/**
	 * Increments or decrements the x, y and z values according to 
	 * the arguments passed in. Use negative values to decrement. Passing
	 * 0 for any argument will keep the x, y, or z value the same.
	 *
	 * @param delta_x The change in x value
	 * @param delta_y The change in y value
	 * @param delta_z The change in z value
	 */
	void modify(N delta_x, N delta_y, N delta_z) {
		
		this->x += delta_x ;
		this->y += delta_y ;
		this->z += delta_z ;
		this->boundsCheck() ;
	}
	
	static N calcDistance(const Location & here, const Location & there) {
		N dx = here.x - there.x ;
		N dy = here.y - there.y ;
		N nx = setUnsigned(dx) ;
		N ny = setUnsigned(dy) ;
		N dist = pythag(nx, ny) ;
		return dist ;
	}
	
	std::string toString() const {
		stringstream ss ;
		ss << "x = " << x << ", y = " << y << ", z = " << z ;
		return ss.str() ;
	}
};

template <typename N>
const N * Location<N>::MAX_X = &GLOBAL_MAX_X ;

template <typename N>
const N * Location<N>::MIN_X = &GLOBAL_MIN_X ;

template <typename N>
const N * Location<N>::MAX_Y = &GLOBAL_MAX_Y ;

template <typename N>
const N * Location<N>::MIN_Y = &GLOBAL_MIN_Y ;

//template <typename N>
//const N * Location<N>::MAX_Z = nullptr ;

//template <typename N>
//const N * Location<N>::MIN_Z = nullptr ;


#endif
