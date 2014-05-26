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



/**
 * Position<> is a simple vector type with the useful feature of storing all its previous states,
 * back through time. Thus an object using Position to store location information will be able to
 * retrace its steps.
 * Note: Classes with a Position data member will typically want to have a pointer,
 * instead of holding the Position locally. This is because many objects in the GameWorld
 * may not actually have a physcical Position in space, in which case they can just hold a
 * null pointer.
 */
template <typename N>
struct Position {

protected:

	N x ;
	N y ;
    N z ;

	/**
	 * A special copying function that won't push another instance of ourselves onto
	 * pastPositions or copy pastPositions (it's left null), avoiding the possibility of infinite recursion. Will use this for
	 * when we need to push a copy of ourselves onto pastPositions. See setAll(), archive() and pastPositions.
	 */
	Position<N> copyNoSave() {
		Position<N> copy {this->x, this->y, this->z} ;
		copy.pastPositions = nullptr ;
		return copy ;
	}

	/** A vector container storing all the previous positions of this Position object,
	 *  with the most recent positions at the end of the vector, and the initial position at
	 *  the front. See archive() (via setAll()).
	 */
	vector<Position<N>> * pastPositions ;
    
    //friend class VectorHeading ;

public:
	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Position() : x(0), y(0), z(0), pastPositions(new vector<Position<N>>) {}
	
	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Position(const BoundsCheck<N> & check) : x(0), y(0), z(0), pastPositions(new vector<Position<N>>) { this->checkBounds(check) ; }
	
	/**
     * Creates a Positionwith all coordinates initialized to n
     */
	Position(N n) : x(n), y(n), z(n), pastPositions(new vector<Position<N>>) {}
	
	/**
     * Creates a Positionwith all coordinates initialized to n
     */
	Position(N n, const BoundsCheck<N> & check) : x(n), y(n), z(n), pastPositions(new vector<Position<N>>) { this->checkBounds(check) ; }
	
	/**
     * Creates a Positionwith all coordinates randomized, with bounds set by check
     */
	Position(fastRand<N> rand, const BoundsCheck<N> & check) :
		x(rand.nextValue(check.min_X, check.max_X)),
		y(rand.nextValue(check.min_Y, check.max_Y)),
		z(0),
		pastPositions(new vector<Position<N>>)
	{
		checkBounds(check) ;
	}

	
    /**
     * Copy constructor for Position
     */
    Position(const Position & other) : Position(other.x, other.y, other.z)  {
		if (other.pastPositions != nullptr) {
			this->pastPositions = new vector<Position<N>>(*other.pastPositions) ;
		}
	}
	
	/**
     * Copy constructor for Position
     */
    Position(const Position & other, const BoundsCheck<N> & check) : Position(other.x, other.y, other.z)  {
		if (other.pastPositions != nullptr) {
			this->pastPositions = new vector<Position<N>>(*other.pastPositions) ;
		}
		this->checkBounds(check) ;
	}
	
	/**
     * Move constructor for Position
     */
    Position(Position && other) : Position(other.x, other.y, other.z) {

		/* Debug code */
		//DebugOutput << "Warning: Position move constructor called. The argument's pastPositions are now null. \n" ;
		/* End Debug code */

		pastPositions = other.pastPositions ;

		other.pastPositions = nullptr ;
	}
	
	/**
     * Move constructor for Position
     */
    Position(Position && other, const BoundsCheck<N> & check) : Position(other.x, other.y, other.z) {

		/* Debug code */
		DebugOutput << "Warning: Position move constructor called. The argument's pastPositions are now null. \n" ;
		/* End Debug code */

		pastPositions = other.pastPositions ;

		other.pastPositions = nullptr ;

		this->checkBounds(check) ;
	}
    
	/**
     * Creates a Position with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
	Position(N x, N y, N z) : x{x}, y{y}, z{z}, pastPositions(new vector<Position<N>>) {}
	
    /**
     * Creates a Position with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
	Position(N x, N y, N z, const BoundsCheck<N> & check) : x{x}, y{y}, z{z}, pastPositions(new vector<Position<N>>)  {
		this->checkBounds(check) ;
	}
	
    /**
     * Destructor for Position
     */
    virtual ~Position() {
		/* Debug code */
		//DebugOutput << "Warning: Position destructor called. This instance's pastPositions were deleted \n" ;
		/* End Debug code */

		if (pastPositions != nullptr) {
			delete pastPositions ;
		}
	}


    /**
     * Assigment operator (copy).
     * The copy assignment operator for Position is unusual. Instead of copying rhs completely, it takes the positional data
     * from rhs, and updates this->x, y, and z to match. However, it does not copy the rhs's pastPositions, instead merely updating
     * this Position's own pastPosition with this Position's current positional values (by calling archive()), before copying the positional
     * values from rhs to this. Thus the copy assignment operator acts as though rhs is simply the latest position that this Position has been
     * set to. If you need to properly duplicate rhs instead, call the move assignment operator, or better yet, the copy constructor.
	 * See setAll() for identical behavior.
     */
    Position & operator=(const Position & rhs) {
        
        // Debug code
        DebugOutput << "Warning, assignment operator (copy) for Position called. This may cause unexpected behavior. \n" ;
        // End Debug code

        this->setAll(rhs) ;
       
        return *this;
    }
	
	
	/**
     * Assigment operator (move)
     */
    Position & operator=(Position && rhs) {
		// Debug code
		//DebugOutput << "Warning, assignment operator (move) for Position called. This may cause unexpected behavior. \n" ;
		// End Debug code

        if (this != &rhs) {
            this->x = rhs.x ;
            this->y = rhs.y ;
            this->z = rhs.z ;
            this->pastPositions = rhs.pastPositions ;
            
            rhs.pastPositions = nullptr ;
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
        
        Position temp = Position(this->x, this->y, this->z) ;
        
		temp.x = temp.x + rhs.x ;
		temp.y = temp.y + rhs.y ;
		temp.z = temp.z + rhs.z ;
		
		return std::move(temp) ;
	}
	
	Position operator-(const Position & rhs) const {
        
        Position temp = Position(this->x, this->y, this->z) ;
        
		temp.x = temp.x - rhs.x ;
		temp.y = temp.y - rhs.y ;
		temp.z = temp.z - rhs.z ;
		
		return std::move(temp) ;
	}
    
    /*
    friend Position & operator-(const Position & lhs, const Position & rhs) {
		N x = lhs.x - rhs.x ;
		N y = lhs.y - rhs.y ;
		N z = lhs.z - rhs.z ;
		this->setAll(x, y, z) ;
		return *this ;
	}
     */

	/** 
	 * Saves our current state
	 */
	void archive() {
		Position<N> archive = this->copyNoSave() ;
		archive.pastPositions = nullptr ;
		this->pastPositions->push_back(archive) ;
	}

	const vector<Position<N>> * getHistory() {
		return this->pastPositions ;
	}

	/**
	 * This function is key! Everything that changes state must call it because it calls archive() to store our
	 * previous state.
	 */
	void setAll(const N x, const N y, const N z) {
		this->archive() ;
		this->x = x ;
		this->y = y ;
		this->z = z ;
	}

	void setAll(const N x, const N y, const N z, const BoundsCheck<N> & check) {
		setAll(x, y, z) ;
		checkBounds(check) ;
	}

	void setAll(const Position<N> & other) {
		setAll(other.x, other.y, other.z) ;
	}

	void setAll(const Position<N> & other, const BoundsCheck<N> & check) {
		setAll(other.x, other.y, other.z) ;
		checkBounds(check) ;
	}

	void setAll(const N n) { setAll(n, n, n) ; }

	void setAll(const N n, const BoundsCheck<N> check) { setAll(n, n, n, check) ; }

	void setAllZero() { setAll(0) ; }

	
	N getX() const { return this->x ; }
	
	N getY() const { return this->y ; }
	
	N getZ() const { return this->z ; }
	
	void setX(const N x) { setAll(x, this->y, this->z) ; }
	
	void setX(const N x, const BoundsCheck<N> & check) { setX(x) ; checkBounds(check) ; }
	
	void setY(const N y) { setAll(this->x, y, this->z) ; }
	
	void setY(const N y, const BoundsCheck<N> & check) { setY(y) ; checkBounds(check) ; }
	
	void setZ(const N z) { setAll(this->x, this->y, z) ; }
	
	void setZ(const N z, const BoundsCheck<N> & check) { setZ(z) ; checkBounds(check) ; }


	void x_plus_one() { setX(x++) ; }

	void x_plus_one(const BoundsCheck<N> & check) { setX(x++) ; checkBounds(check) ; }

	void x_minus_one() { setX(x--) ; }

	void x_minus_one(const BoundsCheck<N> & check) { setX(x--) ; checkBounds(check) ; }


	void y_plus_one() { setY(y++) ; }

	void y_plus_one(const BoundsCheck<N> & check) { setY(y++) ; checkBounds(check) ; }

	void y_minus_one() { setY(y--) ; }

	void y_minus_one(const BoundsCheck<N> & check) { setY(y--) ; checkBounds(check) ; }
	
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
		auto tempX = this->x ;
		auto tempY = this->y ;
		auto tempZ = this->z ;

		tempX += delta_x ;
		tempY += delta_y ;
		tempZ += delta_z ;

		setAll(tempX, tempY, tempZ) ;
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
	void modify(N delta_x, N delta_y, N delta_z, const BoundsCheck<N> & check) {
		auto tempX = this->x ;
		auto tempY = this->y ;
		auto tempZ = this->z ;
		
		tempX += delta_x ;
		tempY += delta_y ;
		tempZ += delta_z ;

		setAll(tempX, tempY, tempZ, check) ;
	}

	void moveHere(const Position<N> & other) {
		setAll(other.x, other.y, other.z) ;
	}

	void moveHere(const Position<N> & other, const BoundsCheck<N> & check) {
		moveHere(other) ;
		checkBounds(check) ;
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


	void checkBounds(const BoundsCheck<N> & check) {
		
		if (this->x >= check.max_X) {
			*(Debug::debugOutput) << "An X value was over the global limit. Reducing value..." << endl ;
			this->x = check.max_X - 1 ;
		}
		if (this->x < check.min_X) {
			*(Debug::debugOutput) << "An X value was under the global minimum. Increasing value..." << endl ;
			this->x = check.min_X ;
		}
		if (this->y >= check.max_Y) {
			*(Debug::debugOutput) << "A Y value was over the global limit. Reducing value..." << endl ;
			this->y = check.max_Y - 1 ;
		}
		if (this->y < check.min_Y) {
			*(Debug::debugOutput) << "A Y value was under the global minimum. Increasing value..." << endl ;
			this->y = check.min_Y ;
		}
	}

} ;


/**
 * Gives a representation a vector or direction in 3 dimensions
 * Note: do not use with unsigned ints
 */
template<typename N>
struct VectorHeading : public Position<float> {
	
protected:
	Position<N> last ;
	const Position<N> * current ;
	
	/* x, y, and z here (the one we inherited) will be used as deltas that we can add to current to calculate next */
	VectorHeading(const Position<float> & overrideCurrData, const Position<N> * current_, bool b) ;
	void update() ;
	
public:
	VectorHeading(float headingX, float headingY, float headingZ, Position<N> * current_) ;
	VectorHeading(const Position<N> & last_, Position<N> * current_) ;
	VectorHeading(const Position<N> * current_) ;
	VectorHeading(const VectorHeading<N> & other) ;
	VectorHeading(VectorHeading<N> && other) ;
	~VectorHeading() ;
	VectorHeading & operator=(const VectorHeading<N> & rhs) ;
	VectorHeading & operator=(VectorHeading<N> && rhs) ;
	
	void normalize() ;
	void updateAndNormalize() ;
	Position<N> calculateNextPosition(const BoundsCheck<N> &) ;
	static Position<N> calculateNextPosition(const VectorHeading<N> & dir, const Position<N> * current, const BoundsCheck<N> & check) ;

} ;

template<typename N>
VectorHeading<N>::VectorHeading(float headingX, float headingY, float headingZ, Position<N> * current_) :
	Position<float>(headingX, headingY, headingZ),
	current(current_) {}

template<typename N>
VectorHeading<N>::VectorHeading(const Position<float> & overrideCurrData, const Position<N> * current_, bool b) :
	Position<float>(overrideCurrData),
	current(current_) {}

template<typename N>
VectorHeading<N>::VectorHeading(const Position<N> & last_, Position<N> * current_) :
	Position<float>(),
	last(last_), current(current_)
{
	update() ;
}

template<typename N>
VectorHeading<N>::VectorHeading(const Position<N> * current_) :
	Position<float>(),
	last(*current_), current(current_) {}

template<typename N>
VectorHeading<N>::VectorHeading(const VectorHeading<N> & other) :
	Position<float>(other),
	last(Position<N>(other.last)), current(other.current) {}

template<typename N>
VectorHeading<N>::VectorHeading(VectorHeading<N> && other) :
	Position<float>(std::move(other)),
	last(std::move(other.last)), current(other.current)
{
	other.current = nullptr ;
}

template<typename N>
VectorHeading<N>::~VectorHeading()
{
	this->current = nullptr ;
}

template<typename N>
VectorHeading<N> & VectorHeading<N>::operator=(const VectorHeading<N> & rhs) {
	if (this != &rhs) {
		this->copyIgnoreArchived(rhs) ;
		this->last = Position<N>(rhs.last) ;
		this->current = rhs.current ;
	}
	return *this ;
}

template<typename N>
VectorHeading<N> & VectorHeading<N>::operator=(VectorHeading<N> && rhs) {
	if (this != &rhs) {
		this->Position<float>::operator=(std::move(rhs)) ;
		this->last = Position<N>(rhs.last) ;
		this->current = rhs.current ;
		rhs.current = nullptr ;
	}
	return *this ;
}

template<typename N>
void VectorHeading<N>::update() {
	
	if (last != *current) { //only if we've moved...
        
		Position<N> temp = ((*current) - last) ;              /* uses Location's operator+() overload to add
															   our x, y, and z (which are offset values) to those
															   stored in current, giving our new location */
		this->setAll(temp.getX(), temp.getY(), temp.getZ()) ;
		this->last = std::move((Position<N>(*this->current))) ;
	}
}

template<typename N>
void VectorHeading<N>::normalize() {
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
void VectorHeading<N>::updateAndNormalize() {
	update() ;
	normalize() ;
}


template<typename N>
Position<N> VectorHeading<N>::calculateNextPosition(const BoundsCheck<N> & check) {
	normalize() ;
	N nx = (*this->current).getX() + roundF((*this).getX()) ;
	N ny = (*this->current).getY() + roundF((*this).getY()) ;
	N nz = (*this->current).getZ() + roundF((*this).getZ()) ;
	Position<N> next(nx, ny, nz, check) ;
	return std::move(next) ;
}

template<typename N>
Position<N> VectorHeading<N>::calculateNextPosition(const VectorHeading<N> & dir, const Position<N> * current, const BoundsCheck<N> & check) {
	Position<float> direc(dir.x, dir.y, dir.z) ;
	VectorHeading<N> calc = VectorHeading<N>(direc, current, true) ;
	return calc.calculateNextPosition(check) ;
}


/**
 * Translates a Position from within the GameWorld to a Position
 * equivelent within the boundaries of the current screen.
 * Gets the current GLOBAL_max_Position (and min) from World
 * to calculate the ratio
 *
 * @param inGameWorld The Position from within the GameWorld
 */
template<typename T>
extern Position<T> transPosition(const Position<T> & inGameWorld) {

	auto worldXSize = GLOBAL_MAX_X /*- GameObject::GLOBAL_min_X*/ ;
	auto worldYSize = GLOBAL_MAX_Y /*- GameObject::GLOBAL_min_Y*/ ;
	
	T tempX = inGameWorld.getX() /*+ (worldXSize - GameObject::GLOBAL_max_X)*/ ;
	T tempY = inGameWorld.getY() /*+ (worldYSize - GameObject::GLOBAL_max_Y)*/ ;
	
	unsigned tw = termWidth() ;
	unsigned th = termHeight() ;
	
	T x = (tw * tempX)/worldXSize ;
	T y = (th * tempY)/worldYSize ;
	
	return Position<T>(x, y, 0) ;
}





#endif
