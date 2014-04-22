//
//  Header.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#ifndef GameWorld_Position_h
#define GameWorld_Position_h

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
	oneDirection //the best direction! (and also a base case in certain recursive algorithms)
} ;

/**
 * Helps with checking validity of Position objects
 * Used in Position's checkBounds()
 */
template<typename N>
struct BoundsCheck {
	const N MAX_X ;
	const N MIN_X ;
	const N MAX_Y ;
	const N MIN_Y ;
	
	BoundsCheck<N>(N MAX_X_, N MIN_X_, N MAX_Y_, N MIN_Y_) :
		MAX_X(MAX_X_), MIN_X(MIN_X_), MAX_Y(MAX_Y_), MIN_Y(MIN_Y_) {}
} ;

template <typename N>
struct Position {
	
public:
	
	N x ;
	N y ;
    N z ;
	
	static const BoundsCheck<N> defaultCheck ;
	
	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Position() : x(0), y(0), z(0) {}
	
	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Position(const BoundsCheck<N> check) : x(0), y(0), z(0) { this->checkBounds(check) ; }
    
    /**
     * Copy constructor for Position
     */
    Position(const Position & other) : Position(other.x, other.y, other.z)  {}
	
	/**
     * Copy constructor for Position
     */
    Position(const Position & other, const BoundsCheck<N> check) : Position(other.x, other.y, other.z)  { this->checkBounds(check) ; }
	
	/**
     * Move constructor for Position
     */
    Position(Position && other) : Position(other.x, other.y, other.z) {}
	
	/**
     * Move constructor for Position
     */
    Position(Position && other, const BoundsCheck<N> check) : Position(other.x, other.y, other.z) { this->checkBounds(check) ; }
    
	/**
     * Creates a Position with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
	Position(N x, N y, N z) : x{x}, y{y}, z{z} {}
	
    /**
     * Creates a Position with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
	Position(N x, N y, N z, const BoundsCheck<N> check) : x{x}, y{y}, z{z} {
		this->checkBounds(check) ;
	}
	
    /**
     * Destructor for Position
     */
    virtual ~Position() {}
    
    /**
     * Assigment operator overload (copy)
     */
    Position & operator=(const Position & rhs) {
        if (this != &rhs) {
            this->x = rhs.x ;
            this->y = rhs.y ;
            this->z = rhs.z ;
        }
		return *this;
    }
	
	/**
     * Assigment operator overload (move)
     */
    Position & operator=(Position && rhs) {
        if (this != &rhs) {
            this->x = rhs.x ;
            this->y = rhs.y ;
            this->z = rhs.z ;
        }
		return(*this) ;
    }
	
	
	bool operator==(const Position & rhs) {
		if ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z)) {
			return true ;
		}
		else {
			return false ;
		}
	}
	
	bool operator==(Position & rhs) {
		if ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z)) {
			return true ;
		}
		else {
			return false ;
		}
	}
	
	bool operator!=(const Position & rhs) {
		return !(this->operator==(rhs)) ;
	}
	
	bool operator!=(Position & rhs) {
		return !(this->operator==(rhs)) ;
	}
	
	Position operator+(const Position & rhs) const {
		N x = this->x + rhs.x ;
		N y = this->y + rhs.y ;
		N z = this->z + rhs.z ;
		Position temp(x, y, z) ;
		return temp ;
	}
	
	Position operator-(const Position & rhs) const {
		N x = this->x - rhs.x ;
		N y = this->y - rhs.y ;
		N z = this->z - rhs.z ;
		Position temp(x, y, z) ;
		return temp ;
	}
	
	
	N getX() const { return this->x ; }
	
	N getY() const { return this->y ; }
	
	N getZ() const { return this->z ; }
	
	
	
	void setX(const N x) { this->x = x ; }
	
	void setX(const N x, const BoundsCheck<N> check) { this->x = x ; checkBounds(check) ; }
	
	void setY(const N y) { this->y = y ; }
	
	void setY(const N y, const BoundsCheck<N> check) { this->y = y ; checkBounds(check) ; }
	
	void setZ(const N z) { this->z = z ; }
	
	void setZ(const N z, const BoundsCheck<N> check) { this->z = z ; checkBounds(check) ; }
	
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
	}
	
	/**
	 * Increments or decrements the x, y and z values according to
	 * the arguments passed in. Use negative values to decrement. Passing
	 * 0 for any argument will keep the x, y, or z value the same.
	 *
	 * @param delta_x The change in x value
	 * @param delta_y The change in y value
	 * @param delta_z The change in z value
	 */
	void modify(N delta_x, N delta_y, N delta_z, const BoundsCheck<N> check) {
		
		this->x += delta_x ;
		this->y += delta_y ;
		this->z += delta_z ;
		this->checkBounds(check) ;
	}
	
	static N calcDistance(const Position & here, const Position & there) {
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

/**
 * A relatively simple data structure representing a Position vector.
 *
 * Note: Classes with a Position data member will typically want to have a pointer,
 * instead of holding the Position locally. This is because many objects in the GameWorld
 * may not actually have a physcical Position in space, in which case they can just hold a
 * null pointer.
 */

	void checkBounds(const BoundsCheck<N> check) {
		
		if (this->x >= check.MAX_X) {
			*(Debug::debugOutput) << "An X value was over the global limit. Reducing value..." << endl ;
			this->x = check.MAX_X - 1 ;
		}
		if (this->x < check.MIN_X) {
			*(Debug::debugOutput) << "An X value was under the global minimum. Increasing value..." << endl ;
			this->x = check.MIN_X ;
		}
		if (this->y >= check.MAX_Y) {
			*(Debug::debugOutput) << "A Y value was over the global limit. Reducing value..." << endl ;
			this->y = check.MAX_Y - 1 ;
		}
		if (this->y < check.MIN_Y) {
			*(Debug::debugOutput) << "A Y value was under the global minimum. Increasing value..." << endl ;
			this->y = check.MIN_Y ;
		}
	}

} ;

template<typename N>
const BoundsCheck<N> Position<N>::defaultCheck(GLOBAL_MAX_X, GLOBAL_MIN_X, GLOBAL_MAX_Y, GLOBAL_MIN_Y) ;


/**
 * Gives a representation a vector or direction in 3 dimensions
 * Note: do not use with unsigned ints
 */
template<typename N>
struct vectorHeading : public Position<N> {
	
protected:
	Position<N> last ;
	const Position<N> * current ;
	
	/* x, y, and z here are deltas that we can add to current to calculate next */
	
	void update() ;
	
public:
	vectorHeading(const Position<N> & last_, Position<N> * current_) ;
	vectorHeading(Position<N> * current_) ;
	vectorHeading(int n) ; //don't use
	~vectorHeading() ;
	
	void update(const Position<N> * next) ;
	Position<N> calculateNextPosition() ;
	Position<N> calculateNextPositionAndUpdate() ;

} ;

template<typename N>
vectorHeading<N>::vectorHeading(const Position<N> & last_, Position<N> * current_) :
	last(last_), current(current_)
{
	update() ;
}

template<typename N>
vectorHeading<N>::vectorHeading(Position<N> * current_) :
	last(*current_), current(current_)
{
	this->x = 0 ;
	this->y = 0 ;
	this->z = 0 ;
}

template<typename N>
vectorHeading<N>::vectorHeading(int n) :
	last(Position<N>(0, 0, 0)), current(nullptr)
{
	this->x = 0 ;
	this->y = 0 ;
	this->z = 0 ;
}

template<typename N>
vectorHeading<N>::~vectorHeading()
{
	this->current = nullptr ;
}

template<typename N>
void vectorHeading<N>::update() {
	Position<N> temp = ((*current) - last) ;
	this->modify(temp.x, temp.y, temp.z) ;
}

template<typename N>
void vectorHeading<N>::update(const Position<N> * next) {
	this->last = (*this->current) ;
	this->current = next ;
	update() ;
}

template<typename N>
Position<N> vectorHeading<N>::calculateNextPosition() {
	Position<N> next = (*this->current) + (*this) ; /* uses Location's operator+() overload to add
													our x, y, and z (which are offset values) to those
													 stored in current, giving our new location */
	return next ;
}

/**
 * This should be the default choice for both calculating the next value
 * and updating current state
 */
template<typename N>
Position<N> vectorHeading<N>::calculateNextPositionAndUpdate() {
	auto next = calculateNextPosition() ;
	this->last = (*current) ;
	delete current ;
	this->current = new Position<N>(next) ;
	update() ;
	
	return next ;
}




#endif
