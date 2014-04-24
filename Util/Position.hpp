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
#include "Util.hpp"
#include "BoundsCheck.hpp"

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

template <typename N>
struct Position {
	
public:
	
	N x ;
	N y ;
    N z ;
	
	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Position() : x(0), y(0), z(0) {}
	
	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Position(const BoundsCheck<N> check) : x(0), y(0), z(0) { this->checkBounds(check) ; }
	
	/**
     * Creates a Positionwith all coordinates initialized to n
     */
	Position(N n) : x(n), y(n), z(n) {}
	
	/**
     * Creates a Positionwith all coordinates initialized to n
     */
	Position(N n, const BoundsCheck<N> check) : x(n), y(n), z(n) { this->checkBounds(check) ; }
	
	/**
     * Creates a Positionwith all coordinates randomized, with bounds set by check
     */
	Position(fastRand<N> rand, const BoundsCheck<N> check) {
		x = rand.nextValue(check.MIN_X, check.MAX_X) ;
		y = rand.nextValue(check.MIN_Y, check.MAX_Y) ;
		z = 0 ;
	}
	
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
	
	
	bool operator==(const Position & rhs) const {
		if ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z)) {
			return true ;
		}
		else {
			return false ;
		}
	}
	
	bool operator==(Position & rhs) const {
		if ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z)) {
			return true ;
		}
		else {
			return false ;
		}
	}
	
	bool operator!=(const Position & rhs) const {
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
	
	void setAll(const N x, const N y, const N z) {
		this->x = x ;
		this->y = y ;
		this->z = z ;
	}
	
	void setAll(const N x, const N y, const N z, const BoundsCheck<N> check) {
		this->x = x ;
		this->y = y ;
		this->z = z ;
		checkBounds(check) ;
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


/**
 * Gives a representation a vector or direction in 3 dimensions
 * Note: do not use with unsigned ints
 */
template<typename N>
struct vectorHeading : public Position<float> {
	
protected:
	Position<N> last ;
	const Position<N> * current ;
	
	/* x, y, and z here (the one we inherited) will be used as deltas that we can add to current to calculate next */
	vectorHeading(const Position<float> & overrideCurrData, const Position<N> * current_, bool b) ;
	void update() ;
	
public:
	vectorHeading(float headingX, float headingY, float headingZ, Position<N> * current_) ;
	vectorHeading(const Position<N> & last_, Position<N> * current_) ;
	vectorHeading(const Position<N> * current_) ;
	vectorHeading(const vectorHeading<N> & other) ;
	vectorHeading(vectorHeading<N> && other) ;
	~vectorHeading() ;
	vectorHeading & operator=(const vectorHeading<N> & rhs) ;
	vectorHeading & operator=(vectorHeading<N> && rhs) ;
	
	void normalize() ;
	void updateAndNormalize() ;
	Position<N> calculateNextPosition(const BoundsCheck<N> &) ;
	static Position<N> calculateNextPosition(const vectorHeading<N> & dir, const Position<N> * current, const BoundsCheck<N> & check) ;

} ;

template<typename N>
vectorHeading<N>::vectorHeading(float headingX, float headingY, float headingZ, Position<N> * current_) :
	Position<float>(headingX, headingY, headingZ),
	current(current_) {}

template<typename N>
vectorHeading<N>::vectorHeading(const Position<float> & overrideCurrData, const Position<N> * current_, bool b) :
	Position<float>(overrideCurrData),
	current(current_) {}

template<typename N>
vectorHeading<N>::vectorHeading(const Position<N> & last_, Position<N> * current_) :
	Position<float>(),
	last(last_), current(current_)
{
	update() ;
}

template<typename N>
vectorHeading<N>::vectorHeading(const Position<N> * current_) :
	Position<float>(),
	last(*current_), current(current_) {}

template<typename N>
vectorHeading<N>::vectorHeading(const vectorHeading<N> & other) :
	Position<float>(other),
	last(Position<N>(other.last)), current(other.current) {}

template<typename N>
vectorHeading<N>::vectorHeading(vectorHeading<N> && other) :
	Position<float>(std::move(other)),
	last(std::move(other.last)), current(other.current)
{
	other.current = nullptr ;
}

template<typename N>
vectorHeading<N>::~vectorHeading()
{
	this->current = nullptr ;
}

template<typename N>
vectorHeading<N> & vectorHeading<N>::operator=(const vectorHeading<N> & rhs) {
	if (this != &rhs) {
		Position<float>::operator=(rhs) ;
		this->last = Position<N>(rhs.last) ;
		this->current = rhs.current ;
	}
	return *this ;
}

template<typename N>
vectorHeading<N> & vectorHeading<N>::operator=(vectorHeading<N> && rhs) {
	if (this != &rhs) {
		Position<float>::operator=(std::move(rhs)) ;
		this->last = Position<N>(rhs.last) ;
		this->current = rhs.current ;
		rhs.current = nullptr ;
	}
	return *this ;
}

template<typename N>
void vectorHeading<N>::update() {
	
	if (last != *current) { //only if we've moved...
		Position<N> temp = ((*current) - last) ;              /* uses Location's operator+() overload to add
															   our x, y, and z (which are offset values) to those
															   stored in current, giving our new location */
		this->setAll(temp.x, temp.y, temp.z) ;
		this->last = Position<N>(*this->current) ;
	}
}

template<typename N>
void vectorHeading<N>::normalize() {
	auto distance = pythag<float>(x, y) ;
	if (x != 0) {
		x = (x / distance) ;
	}
	if (y != 0) {
		y = (y / distance) ;
	}
	z = 0 ;
}

template<typename N>
void vectorHeading<N>::updateAndNormalize() {
	update() ;
	normalize() ;
}


template<typename N>
Position<N> vectorHeading<N>::calculateNextPosition(const BoundsCheck<N> & check) {
	normalize() ;
	N nx = (*this->current).x + roundF((*this).x) ;
	N ny = (*this->current).y + roundF((*this).y) ;
	N nz = (*this->current).z + roundF((*this).z) ;
	Position<N> next(nx, ny, nz, check) ;
	return std::move(next) ;
}

template<typename N>
Position<N> vectorHeading<N>::calculateNextPosition(const vectorHeading<N> & dir, const Position<N> * current, const BoundsCheck<N> & check) {
	Position<float> direc(dir.x, dir.y, dir.z) ;
	vectorHeading<N> calc = vectorHeading<N>(direc, current, true) ;
	return calc.calculateNextPosition(check) ;
}





#endif
