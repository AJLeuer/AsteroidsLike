//
//  Header.h
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#ifndef GameWorld_Position_h
#define GameWorld_Position_h


#include <cmath>
#include <iostream>
#include <ostream>
#include <sstream>
#include <queue>

#include <SDL2/SDL_video.h>

#include "Debug.h"
#include "Util.hpp"
#include "BoundsCheck.hpp"
#include "Velocity.hpp"
#include "GameRandom.hpp"



//#include "../Control/Configuration.h"


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
	noDirection //the best direction! (and also a base case in certain recursive algorithms)
} ;


/**
 * Position<> is a simple vector type with the useful feature of storing all its previous states,
 * back through time. Thus an object using Position to store location information will be able to
 * retrace its steps.
 * Note: Classes with a Position data member will typically want to have a pointer,
 * instead of holding the Position locally. This is because many objects in the World
 * may not actually have a physcical Position in space, in which case they can just hold a
 * null pointer.
 */
template <typename N>
struct Position {

protected:

	N x ;
	N y ;
    N z ;

public:
	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Position() : x(0), y(0), z(0) {}
	
	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Position(const BoundsCheck<N> & check) : x(0), y(0), z(0) { this->checkBounds(check) ; }
	
	/**
     * Creates a Positionwith all coordinates initialized to n
     */
	Position(N n) : x(n), y(n), z(n) {}
	
	/**
     * Creates a Positionwith all coordinates initialized to n
     */
	Position(N n, const BoundsCheck<N> & check) : x(n), y(n), z(n) { this->checkBounds(check) ; }
	
	/**
     * Creates a Positionwith all coordinates randomized, with bounds set by check
     */
    template<typename R>
	Position(FastRand<R> rand) :
		x(rand.nextValue()),
		y(rand.nextValue()),
		z(0) {}
	
	/**
     * Creates a Positionwith all coordinates randomized, with bounds set by check
     */
    template<typename R>
	Position(FastRand<R> rand, const BoundsCheck<N> & check) :
		x(rand.nextValue(check.min_X, check.max_X)),
		y(rand.nextValue(check.min_Y, check.max_Y)),
		z(0)
	{
		checkBounds(check) ;
	}

    /**
     * Copy constructor for Position
     */
    Position(const Position & other) : Position(other.x, other.y, other.z) {}
	
	/**
     * Copy constructor for Position
     */
    Position(const Position & other, const BoundsCheck<N> & check) : Position(other.x, other.y, other.z)  {
		checkBounds(check) ;
	}
	
	/**
     * Move constructor for Position
     */
    Position(Position && other) : Position(other.x, other.y, other.z) {}
	
	/**
     * Move constructor for Position
     */
    Position(Position && other, const BoundsCheck<N> & check) : Position(other.x, other.y, other.z) {
		checkBounds(check) ;
	}
    
	/**
     * Creates a Position with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
	Position(N x, N y, N z) : x(x), y(y), z(z) {}
	
    /**
     * Creates a Position with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
	Position(N x, N y, N z, const BoundsCheck<N> & check) : x(x), y(y), z(z) {
		this->checkBounds(check) ;
	}
	
    /**
     * Destructor for Position
     */
    virtual ~Position() {}


    /**
     * Assigment operator (copy).
     */
    virtual Position & operator=(const Position & rhs) {

		this->x = rhs.x ;
		this->y = rhs.y ;
		this->z = rhs.z ;
       
        return *this;
    }
	
	
	/**
     * Assigment operator (move)
     */
    virtual Position & operator=(Position && rhs) {

		this->x = rhs.x ;
		this->y = rhs.y ;
		this->z = rhs.z ;

		return(*this) ;
    }
    
	
	
	virtual bool operator==(const Position & rhs) const {
		if ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z)) {
			return true ;
		}
		else {
			return false ;
		}
	}
	
	virtual bool operator==(Position & rhs) const {
		if ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z)) {
			return true ;
		}
		else {
			return false ;
		}
	}
	
	virtual bool operator!=(const Position & rhs) const {
		return !(this->operator==(rhs)) ;
	}
	
	virtual bool operator!=(Position & rhs) {
		return !(this->operator==(rhs)) ;
	}
	
	Position operator+(const Position & rhs) const {
        
        Position temp = Position(this->x, this->y, this->z) ;
        
		temp.x = temp.x + rhs.x ;
		temp.y = temp.y + rhs.y ;
		temp.z = temp.z + rhs.z ;
		
		return temp ;
	}
	
	Position operator-(const Position & rhs) const {
        
        Position temp = Position(this->x, this->y, this->z) ;
        
		temp.x = temp.x - rhs.x ;
		temp.y = temp.y - rhs.y ;
		temp.z = temp.z - rhs.z ;
		
		return temp ;
	}
    
	template<typename O, typename P>
    friend const Position<P> * operator+(const Position<P> & lhs, const Position<O> * rhs) {

		P x = lhs.x + rhs->x ;
		P y = lhs.y + rhs->y ;
		P z = lhs.z + rhs->z ;

		return new Position<P>(x, y, z) ;
	}

	template<typename O, typename P>
    friend const Position<P> * operator-(const Position<P> & lhs, const Position<O> * rhs) {

		P x = lhs.x - rhs->x ;
		P y = lhs.y - rhs->y ;
		P z = lhs.z - rhs->z ;

		return new Position<P>(x, y, z) ;
	}
	
	friend ostream & operator<<(std::ostream & os, const Position<N> * pos) {
		os << "Position: X = " << pos->x << ", Y = " << pos->y << ", Z = " << pos->z << '\n' ;
		return os ;
	}
	
	friend ostream & operator<<(std::ostream & os, const Position<N> & pos) {
		os << "Position: X = " << pos.x << ", Y = " << pos.y << ", Z = " << pos.z << '\n' ;
		return os ;
	}


	/**
	 * Sets x, y, and z to the given values.
	 */
	virtual void setAll(const N x, const N y, const N z) {
		this->x = x ;
		this->y = y ;
		this->z = z ;
	}

	virtual void setAll(const N x, const N y, const N z, const BoundsCheck<N> & check) {
		setAll(x, y, z) ;
		checkBounds(&check) ;
	}

	virtual void setAll(const Position<N> & other) {
		setAll(other.x, other.y, other.z) ;
	}

	virtual void setAll(const Position<N> & other, const BoundsCheck<N> & check) {
		setAll(other.x, other.y, other.z) ;
		checkBounds(&check) ;
	}

	virtual void setAll(const N n) { setAll(n, n, n) ; }

	virtual void setAll(const N n, const BoundsCheck<N> & check) { setAll(n, n, n, check) ; }

	virtual void setAllZero() { setAll(0) ; }

	
	N getX() const { return this->x ; }
	
	N getY() const { return this->y ; }
	
	N getZ() const { return this->z ; }

	/**
	 * @return x as an integer
	 */
	int getIntX() const { return roundF<N, int>(x) ; }

	/**
	 * @return z as an integer
	 */
	int getIntY() const { return roundF<N, int>(y) ; }

	/**
	 * @return z as an integer
	 */
	int getIntZ() const { return roundF<N, int>(z) ; }
	
	virtual void setX(const N x) { setAll(x, this->y, this->z) ; }
	
	virtual void setX(const N x, const BoundsCheck<N> & check) { setX(x) ; checkBounds(check) ; }
	
	virtual void setY(const N y) { setAll(this->x, y, this->z) ; }
	
	virtual void setY(const N y, const BoundsCheck<N> & check) { setY(y) ; checkBounds(check) ; }
	
	virtual void setZ(const N z) { setAll(this->x, this->y, z) ; }
	
	virtual void setZ(const N z, const BoundsCheck<N> & check) { setZ(z) ; checkBounds(check) ; }


	virtual void x_plus_one() { setX(x++) ; }

	virtual void x_plus_one(const BoundsCheck<N> & check) { setX(x++) ; checkBounds(check) ; }

	virtual void x_minus_one() { setX(x--) ; }

	virtual void x_minus_one(const BoundsCheck<N> & check) { setX(x--) ; checkBounds(check) ; }


	virtual void y_plus_one() { setY(y++) ; }

	virtual void y_plus_one(const BoundsCheck<N> & check) { setY(y++) ; checkBounds(check) ; }

	virtual void y_minus_one() { setY(y--) ; }

	virtual void y_minus_one(const BoundsCheck<N> & check) { setY(y--) ; checkBounds(check) ; }

	virtual void moveRight() { setAll((this->x+1), this->y, this->z) ; }
	virtual void moveLeft() { setAll((this->x-1), this->y, this->z) ; }
	virtual void moveUp() { setAll(this->x, (this->y-1), this->z) ; }
	virtual void moveDown() { setAll(this->x, (this->y+1), this->z) ; }

	virtual void moveUpRight() { setAll((this->x+1), (this->y-1), this->z) ; }
	virtual void moveUpLeft() { setAll((this->x-1), (this->y-1), this->z) ; }
	virtual void moveDownRight() { setAll((this->x+1), (this->y+1), this->z) ; }
	virtual void moveDownLeft() { setAll((this->x-1), (this->y+1), this->z) ; }
	
	/**
	 * Increments or decrements the x, y and z values according to 
	 * the arguments passed in. Use negative values to decrement. Passing
	 * 0 for any argument will keep the x, y, or z value the same.
	 *
	 * @param delta_x The change in x value
	 * @param delta_y The change in y value
	 * @param delta_z The change in z value
	 */
	virtual void modify(N delta_x, N delta_y, N delta_z) {
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
	virtual void modify(N delta_x, N delta_y, N delta_z, const BoundsCheck<N> & check) {
		auto tempX = this->x ;
		auto tempY = this->y ;
		auto tempZ = this->z ;
		
		tempX += delta_x ;
		tempY += delta_y ;
		tempZ += delta_z ;

		setAll(tempX, tempY, tempZ, check) ;
	}

	virtual void moveHere(N x, N y, N z) {
		setAll(x, y, z) ;
	}

	virtual void moveHere(N x, N y, N z, const BoundsCheck<N> & check) {
		moveHere(x, y, z) ;
		checkBounds(check) ;
	}

	virtual void moveHere(const Position<N> & other) {
		setAll(other.x, other.y, other.z) ;
	}

	virtual void moveHere(const Position<N> & other, const BoundsCheck<N> & check) {
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
		ss << this ;
		return ss.str() ;
	}


	void checkBounds(const BoundsCheck<N> * check, N objWidth = 0, N objHeight = 0) {
		
		if ((this->x /*+ objWidth*/) >= check->max_X) {
			{
			/* Debug code */
			DebugOutput << "An X value was over the global limit. Reducing value... \n" ;
			/* End Debug code */
			}
			this->x = check->max_X /*- objWidth*/ - 1 ;
		}
		if (this->x /*- objWidth*/ < check->min_X) {
			{
			/* Debug code */
			DebugOutput << "An X value was under the global minimum. Increasing value... \n" ;
			/* End Debug code */
			}
			this->x = check->min_X /*+ objWidth*/ ;
		}
		if ((this->y /*+ objHeight*/) >= check->max_Y) {
			{
			/* Debug code */
			DebugOutput << "A Y value was over the global limit. Reducing value... \n" ;
			/* End Debug code */
			}
			this->y = check->max_Y /*- objHeight*/ - 1 ;
		}
		if (this->y < check->min_Y) {
			{
			/* Debug code */
			DebugOutput << "A Y value was under the global minimum. Increasing value... \n" ;
			/* End Debug code */
			}
			this->y = check->min_Y ;
		}
	}
	
	void checkBounds(const BoundsCheck<N> & check, N objWidth = 0, N objHeight = 0) {
		
		if ((this->x /*+ objWidth*/) >= check.max_X) {
			{
				/* Debug code */
				DebugOutput << "An X value was over the global limit. Reducing value... \n" ;
				/* End Debug code */
			}
			this->x = check.max_X /*- objWidth*/ - 1 ;
		}
		if (this->x /*- objWidth*/ < check.min_X) {
			{
				/* Debug code */
				DebugOutput << "An X value was under the global minimum. Increasing value... \n" ;
				/* End Debug code */
			}
			this->x = check.min_X /*+ objWidth*/ ;
		}
		if ((this->y /*+ objHeight*/) >= check.max_Y) {
			{
				/* Debug code */
				DebugOutput << "A Y value was over the global limit. Reducing value... \n" ;
				/* End Debug code */
			}
			this->y = check.max_Y /*- objHeight*/ - 1 ;
		}
		if (this->y < check.min_Y) {
			{
				/* Debug code */
				DebugOutput << "A Y value was under the global minimum. Increasing value... \n" ;
				/* End Debug code */
			}
			this->y = check.min_Y ;
		}
	}

	bool overBounds(const BoundsCheck<N> * check, N objWidth = 0, N objHeight = 0) {
		if (((this->x + objWidth) >= check->max_X) || ((this->y + objHeight) >= check->max_Y)) {
			return true ;
		}
		else if	((this->x < check->min_X) || (this->y < check->min_Y)) {
			return true ;
		}
		else {
			return false ;
		}
	}

	bool overXBounds(const BoundsCheck<N> * check) {
		if ((this->x >= check->max_X) || (this->x < check->min_X)) {
			return true ;
		}
		else {
			return false ;
		}
	}

	bool overYBounds(const BoundsCheck<N> * check) {
		if ((this->y >= check->max_Y) || (this->y < check->min_Y)) {
			return true ;
		}
		else {
			return false ;
		}
	}

} ;



/**
 * Similar to Position, but also holds copies of each of its previous states.
 */
template<typename N>
struct Pos2 : public Position<N> {

protected:

	/** 
	 * A vector container storing all the previous positions of this object,
	 * with the most recent positions at the end of the vector, and the initial position at
	 * the front. See archive().
	 */
	queue<Position<N>> * pastPositions ;

	/**
	 * Saves our current state
	 */
	void archive() {
		Position<N> archived(this->x, this->y, this->z) ; //archived will just hold this, without the pastPositions (no infinite recursion here!)

		if (pastPositions->size() > 10000) { //once we go over a certain size, we'll delete the oldest to save space
			pastPositions->pop() ;
		}
		pastPositions->push(archived) ;
	}

public:

	/**
     * Creates a Pos2 with all coordinates initialized to 0
     */
	Pos2() : Position<N>(0, 0, 0), pastPositions(new queue<Position<N>>) {}

	/**
     * Creates a Pos2 with all coordinates initialized to 0
     */
	Pos2(const BoundsCheck<N> & check) : Position<N>(check), pastPositions(new queue<Position<N>>) {}

	/**
     * Creates a Pos2 with all coordinates initialized to n
     */
	Pos2(N n) : Position<N>(n), pastPositions(new queue<Position<N>>) {}

	/**
     * Creates a Pos2 with all coordinates initialized to n
     */
	Pos2(N n, const BoundsCheck<N> & check) : Position<N>(n, check), pastPositions(new queue<Position<N>>) {}
	
	/**
     * Creates a Pos2 all coordinates randomized, with bounds set by check
     */
    template<typename R>
	Pos2(FastRand<R> & rand) :
		Position<N>(rand),
		pastPositions(new queue<Position<N>>) {}

	/**
     * Creates a Pos2 all coordinates randomized, with bounds set by check
     */
    template<typename R>
	Pos2(FastRand<R> & rand, const BoundsCheck<N> & check) :
		Position<N>(rand, check),
		pastPositions(new queue<Position<N>>) {}

	/**
     * Copy constructor for Pos2
     */
    Pos2(const Pos2 & other) : Position<N>(other)  {
		if (other.pastPositions != nullptr) {
			this->pastPositions = new queue<Position<N>>(*other.pastPositions) ;
		}
	}

	/**
     * Copy constructor for Pos2
     */
    Pos2(const Pos2 & other, const BoundsCheck<N> & check) : Position<N>(other, check)  {
		if (other.pastPositions != nullptr) {
			this->pastPositions = new queue<Position<N>>(*other.pastPositions) ;
		}
	}

	/**
     * Move constructor for Pos2
     */
    Pos2(Pos2 && other) : Position<N>(std::move(other)) {

		{
		/* Debug code */
		DebugOutput << "Warning: Pos2 move constructor called. The argument's pastPositions are now null. \n" ;
		/* End Debug code */
		}

		pastPositions = other.pastPositions ;

		other.pastPositions = nullptr ;
	}

	/**
     * Move constructor for Position
     */
    Pos2(Pos2 && other, const BoundsCheck<N> & check) : Position<N>(std::move(other), check) {

		{
		/* Debug code */
		DebugOutput << "Warning: Pos2 move constructor called. The argument's pastPositions are now null. \n" ;
		/* End Debug code */
		}

		pastPositions = other.pastPositions ;

		other.pastPositions = nullptr ;
	}

	/**
     * Creates a Pos2 with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
	Pos2(N x, N y, N z) : Position<N>(x, y, z), pastPositions(new queue<Position<N>>) {}

	/**
     * Creates a Pos2 with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     * @param z The z coordinate
     */
	Pos2(N x, N y, N z, const BoundsCheck<N> & check) : Position<N>(x, y, z, check), pastPositions(new queue<Position<N>>) {}

	/**
     * Destructor for Position
     */
    virtual ~Pos2() {
		
		{
		/* Debug code */
		DebugOutput << "Warning: Pos2 destructor called. This instance's pastPositions were deleted \n" ;
		/* End Debug code */
		}

		if (pastPositions != nullptr) {
			delete pastPositions ;
		}
	}

	/**
     * Assigment operator (copy).
	 *
     */
    Pos2 & operator=(const Pos2 & rhs) {

		{
        /* Debug code */
        DebugOutput << "Warning, assignment operator (copy) for Pos2 called. This may cause a performance hit. \n" ;
        /* End Debug code */
		}

		if (pastPositions != nullptr) {
			delete pastPositions ;
		}

		pastPositions = nullptr ;

	    this->Position<N>::operator=(std::move(rhs)) ;

        pastPositions = new queue<Position<N>>() ;

		for (auto i = rhs.pastPositions->begin() ; i != rhs.pastPositions.end() ; i++) {
			this->pastPositions->push(Position<N>(*i)) ; //push_back()
		}

        return *this;
    }

	/**
     * Assigment operator (move)
     */
    Pos2 & operator=(Pos2 && rhs) {
		
		{
		/* Debug code */
		DebugOutput << "Warning, assignment operator (move) for Pos2 called. This may cause unexpected behavior. \n" ;
		/* End Debug code */
		}

		this->Position<N>::operator=(std::move(rhs)) ;

		this->pastPositions = rhs.pastPositions ;
		rhs.pastPositions = nullptr ;

		return(*this) ;
    }

	/**
     * Assigment operator (copy). Treats rhs as this Pos2 object's current position, and pushes back its previous state
	 * onto pastPositions.
     */
    Pos2 & operator=(const Position<N> & rhs) {
		
		{
        /* Debug code */
        DebugOutput << "Warning, assignment operator (copy) for Pos2 called. This may cause unexpected behavior. \n" ;
        /* End Debug code */
		}

        setAll(rhs) ;

        return *this;
    }

	/**
     * Assigment operator (move)
     */
    Pos2 & operator=(Position<N> && rhs) {
		
		{
		/* Debug code */
		DebugOutput << "Warning, assignment operator (move) for Pos2 called. This may cause unexpected behavior. \n" ;
		/* End Debug code */
		}

		setAll(rhs) ;

        return *this ;
    }

	bool operator==(const Pos2 & rhs) const {
		return Position<N>::operator==(rhs) ;
	}

	bool operator==(Pos2 & rhs) const {
		return Position<N>::operator==(rhs) ;
	}

	bool operator==(const Position<N> & rhs) const {
		return Position<N>::operator==(rhs) ;
	}

	bool operator==(Position<N> & rhs) const {
		return Position<N>::operator==(rhs) ;
	}

	bool operator!=(Pos2 & rhs) const {
		return Position<N>::operator!=(rhs) ;
	}

	bool operator!=(const Pos2 & rhs) const {
		return Position<N>::operator!=(rhs) ;
	}

	bool operator!=(Position<N> & rhs) const {
		return Position<N>::operator!=(rhs) ;
	}

	bool operator!=(const Position<N> & rhs) const {
		return Position<N>::operator!=(rhs) ;
	}

	Pos2 operator+(const Pos2 & rhs) const {
		return Position<N>::operator+(rhs) ;
	}

	Pos2 operator+(const Position<N> & rhs) const {
		Pos2 pos2 = Position<N>::operator+(rhs) ;
		return pos2 ;
	}

	Pos2 operator-(const Pos2 & rhs) const {
		return Position<N>::operator-(rhs) ;
	}

	Pos2 operator-(const Position<N> & rhs) const {
		return Position<N>::operator-(rhs) ;
	}

	const Position<N> * getCurrent() {
		return (Position<N>)(this) ;
	}
	
	const queue<Position<N>> * getHistory() {
		return pastPositions ;
	}

	void setAll(const N x, const N y, const N z) {
		archive() ;
		this->Position<N>::setAll(x, y, z) ;
	}

	void setAll(const N x, const N y, const N z, const BoundsCheck<N> & check) {
		archive() ;
		Position<N>::setAll(x, y, z, check) ;
	}

	void setAll(const Position<N> & other) {
		setAll(other.getX(), other.getY(), other.getZ()) ;
	}

	void setAll(const Position<N> & other, const BoundsCheck<N> & check) {
		setAll(other.getX(), other.getY(), other.getZ(), check) ;
	}

	void setAll(const N n) { setAll(n, n, n) ; }

	void setAll(const N n, const BoundsCheck<N> & check) { setAll(n, n, n, check) ; }

	void setAllZero() { setAll(0) ; }


	void setX(const N x) { setAll(x, this->y, this->z) ; }

	void setX(const N x, const BoundsCheck<N> & check) { setX(x) ; this->checkBounds(check) ; }

	void setY(const N y) { setAll(this->x, y, this->z) ; }

	void setY(const N y, const BoundsCheck<N> & check) { setY(y) ; this->checkBounds(check) ; }

	void setZ(const N z) { setAll(this->x, this->y, z) ; }

	void setZ(const N z, const BoundsCheck<N> & check) { setZ(z) ; this->checkBounds(check) ; }

	void x_plus_one() { setX(this->x++) ; }

	void x_plus_one(const BoundsCheck<N> & check) { setX(this->x++) ; this->checkBounds(check) ; }

	void x_minus_one() { setX(this->x--) ; }

	void x_minus_one(const BoundsCheck<N> & check) { setX(this->x--) ; this->checkBounds(check) ; }


	void y_plus_one() { setY(this->y++) ; }

	void y_plus_one(const BoundsCheck<N> & check) { setY(this->y++) ; this->checkBounds(check) ; }

	void y_minus_one() { setY(this->y--) ; }

	void y_minus_one(const BoundsCheck<N> & check) { setY(this->y--) ; this->checkBounds(check) ; }

	void moveRight() { setAll((this->x+1), this->y, this->z) ; }
	void moveLeft() { setAll((this->x-1), this->y, this->z) ; }
	void moveUp() { setAll(this->x, (this->y+1), this->z) ; }
	void moveDown() { setAll(this->x, (this->y-1), this->z) ; }

	void moveUpRight() { setAll((this->x+1), (this->y+1), this->z) ; }
	void moveUpLeft() { setAll((this->x-1), (this->y+1), this->z) ; }
	void moveDownRight() { setAll((this->x+1), (this->y-1), this->z) ; }
	void moveDownLeft() { setAll((this->x-1), (this->y-1), this->z) ; }

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

	void moveHere(N x, N y, N z) {
		setAll(x, y, z) ;
	}

	void moveHere(N x, N y, N z, const BoundsCheck<N> & check) {
		moveHere(x, y, z) ;
		this->checkBounds(check) ;
	}

	void moveHere(const Pos2 & other) {
		setAll(other.x, other.y, other.z) ;
	}

	void moveHere(const Position<N> & other) {
		setAll(other.getX(), other.getY(), other.getZ()) ;
	}

	void moveHere(const Pos2 & other, const BoundsCheck<N> & check) {
		moveHere(other) ;
		this->Position<N>::checkBounds(check) ;
	}

	void moveHere(const Position<N> & other, const BoundsCheck<N> & check) {
		moveHere(other) ;
		this->Position<N>::checkBounds(check) ;
	}

} ;

/**
 * This class provides facilities for calculating an object's current trajectory,
 * predicting its next Position, monitoring its speed, maintaining a record of its
 * last two Positions (for more detailed record keeping of past Positions, see Pos2),
 * and more.
 *
 * Note: do not use with unsigned ints
 */
template<typename N>
struct Vectr : public Position<float> {
	
protected:
    
    /**
     * The Position preceding the most recent Position
     */
    Position<N> last ;
    
    /**
     * The Position most recently visited
     */
	Position<N> mostRecent ;
    
    /**
     * A pointer to the current Position
     */
	const Position<N> * current ;
    
    /**
     * The non-normalized distance between the current Position and mostRecent Position
     */
    N absDistanceMoved = 0 ;
	
	N * totalDistanceMoved ;
	
	/**
	 * Monitors velocity
	 */
	Velocity<N> * velocity ;
	
	mutex * sharedVelMutex  ;
	bool sharedVelBool = true ;
	
	/* x, y, and z here (the one we inherited) will be used as deltas that we can add to current to calculate next */
	Vectr(const Position<float> & overrideCurrData, const Position<N> * current_, bool b) ;
	void update() ;
	
	friend class Vectr ;
	
public:
	
	Vectr(float headingX, float headingY, float headingZ, Position<N> * current_) ;
	Vectr(const Position<N> & mostRecent_, Position<N> * current_) ;
	Vectr(const Position<N> * current_) ;
	Vectr(const Vectr<N> & other) ;
	Vectr(Vectr<N> && other) ;
	~Vectr() ;
	Vectr & operator=(const Vectr<N> & rhs) ;
	Vectr & operator=(Vectr<N> && rhs) ;
	
	void normalize() ;
	void updateAndNormalize() ;
	
	Velocity<N> & calculateVelocity() ;

	
	static Position<N> calculateNextPosition(Vectr<N> &, float modifier = 1.0) ;

	
	static Position<N> calculateNextPositionCh(Vectr<N> &, float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;

	
	static Position<N> calculateReverseNextPosition(Vectr<N> &, float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;

	
	static Position<N> calculateReverseXPosition(Vectr<N> &, float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;

	
	static Position<N> calculateReverseYPosition(Vectr<N> &, float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;

	
	static Position<N> calculateNextPosition(Vectr<N> & dir, const Position<N> * current, float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;

} ;

template<typename N>
Vectr<N>::Vectr(float headingX, float headingY, float headingZ, Position<N> * current_) :
	Position<float>(headingX, headingY, headingZ),
	current(current_),
	totalDistanceMoved(new N),
	sharedVelMutex(new mutex())
{
	velocity = new Velocity<N>(totalDistanceMoved, sharedVelMutex, &sharedVelBool) ;
}

template<typename N>
Vectr<N>::Vectr(const Position<float> & overrideCurrData, const Position<N> * current_, bool b) :
	Position<float>(overrideCurrData),
	current(current_),
	totalDistanceMoved(new N),
	sharedVelMutex(new mutex())
{
	velocity = new Velocity<N>(totalDistanceMoved, sharedVelMutex, &sharedVelBool) ;
}


template<typename N>
Vectr<N>::Vectr(const Position<N> * current_) :
	Position<float>(),
    last(*current_),
	mostRecent(*current_), current(current_),
	totalDistanceMoved(new N),
	sharedVelMutex(new mutex())
{
	velocity = new Velocity<N>(totalDistanceMoved, sharedVelMutex, &sharedVelBool) ;
}


template<typename N>
Vectr<N>::Vectr(const Vectr<N> & other) :
	Position<float>(other),
    last(Position<N>(other.last)),
	mostRecent(Position<N>(other.mostRecent)),
    absDistanceMoved(other.absDistanceMoved),
	totalDistanceMoved(new N(*other.totalDistanceMoved)),
	sharedVelMutex(new mutex())

{
	velocity = new Velocity<N>(totalDistanceMoved, sharedVelMutex, &sharedVelBool) ;
}


template<typename N>
Vectr<N>::Vectr(Vectr<N> && other) :
	Position<float>(std::move(other)),
    last(std::move(other.last)),
	mostRecent(std::move(other.mostRecent)), current(other.current),
    absDistanceMoved(other.absDistanceMoved),
	totalDistanceMoved(other.totalDistanceMoved),
	sharedVelMutex(other.sharedVelMutex),
	velocity(other.velocity)
{
	other.current = nullptr ;
	other.totalDistanceMoved = nullptr ;
	other.velocity = nullptr ;
}

template<typename N>
Vectr<N>::~Vectr()
{
	sharedVelBool = false ;
	sharedVelMutex->lock() ;
	
	this->current = nullptr ;
	
	if (velocity != nullptr) {
		delete velocity;
	}
	
	if (totalDistanceMoved != nullptr) {
		delete totalDistanceMoved ;
	}
	
	sharedVelMutex->unlock() ;
	delete sharedVelMutex ;
}

template<typename N>
Vectr<N> & Vectr<N>::operator=(const Vectr<N> & rhs) {
	if (this != &rhs) {
		
		delete current ;
		delete totalDistanceMoved ;
		delete velocity ;
		
		this->Position<float>::operator=(rhs) ;
        this->last = Position<N>(rhs.last) ;
		this->mostRecent = Position<N>(rhs.mostRecent) ;
		this->current = rhs.current ;
        this->absDistanceMoved = rhs.absDistanceMoved ;
		this->totalDistanceMoved = new N(*rhs.totalDistanceMoved) ;
		this->velocity = new Velocity<N>(totalDistanceMoved, sharedVelMutex, &sharedVelBool) ;
		
	}
	return *this ;
}

template<typename N>
Vectr<N> & Vectr<N>::operator=(Vectr<N> && rhs) {
	if (this != &rhs) {
		
		this->Position<float>::operator=(std::move(rhs)) ;
        this->last = Position<N>(rhs.last) ;
		this->mostRecent = Position<N>(rhs.mostRecent) ;
		this->current = rhs.current ;
        this->absDistanceMoved = rhs.absDistanceMoved ;
		this->totalDistanceMoved = rhs.totalDistanceMoved ;
		this->velocity = rhs.velocity ;
		
		rhs.current = nullptr ;
		rhs.totalDistanceMoved = nullptr ;
		rhs.velocity = nullptr ;
	}
	return *this ;
}

template<typename N>
void Vectr<N>::update() {

	if (mostRecent != *current) { //only if we've moved...
        
        absDistanceMoved = calcEuclidianDistance(mostRecent, *current) ;
		*totalDistanceMoved += absDistanceMoved ;
        
		Position<N> temp = ((*current) - mostRecent) ;               /*  uses Position operator+() overload to add
															       our x, y, and z (which are offset values) to those
															       stored in current, giving our new location  */
		setAll(temp.getX(), temp.getY(), temp.getZ()) ;
        last = mostRecent ;
		mostRecent = std::move((Position<N>(*this->current))) ;
	}

}

template<typename N>
void Vectr<N>::normalize() {
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
void Vectr<N>::updateAndNormalize() {
	update() ;
	normalize() ;
}

template<typename N>
Velocity<N> & Vectr<N>::calculateVelocity() {
	return this->velocity ;
}

template<typename N>
Position<N> Vectr<N>::calculateNextPosition(Vectr<N> & vec, float modifier) {
	
	vec.normalize() ;

	N nx ;
	N ny ;
	N nz ;

	nx = (vec.current)->getX() + (vec.getX() * modifier) ;
	ny = (vec.current)->getY() + (vec.getY() * modifier) ;
	nz = (vec.current)->getZ() + (vec.getZ() * modifier) ;

	Position<N> next(nx, ny, nz) ;
	return next ;
}

template<typename N>
Position<N> Vectr<N>::calculateNextPositionCh(Vectr<N> & vec, float modifier, const BoundsCheck<N> & check) {

	vec.normalize() ;

	N nx ;
	N ny ;
	N nz ;

	nx = (vec.current)->getX() + (vec.getX() * modifier) ;
	ny = (vec.current)->getY() + (vec.getY() * modifier) ;
	nz = (vec.current)->getZ() + (vec.getZ() * modifier) ;

	Position<N> next(nx, ny, nz, check) ;
	return next ;
}

template<typename N>
Position<N> Vectr<N>::calculateReverseNextPosition(Vectr<N> & vec, float modifier, const BoundsCheck<N> & check) {
	vec.x = (vec.x * -1) ;
	vec.y = (vec.y * -1) ;
	vec.z = (vec.z * -1) ;
	return calculateNextPosition(vec, modifier, check) ;
}

template<typename N>
Position<N> Vectr<N>::calculateReverseXPosition(Vectr<N> & vec, float modifier, const BoundsCheck<N> & check) {
	vec.x = (vec.x * -1) ;
	return calculateNextPositionCh(vec, modifier, check) ;
}

template<typename N>
Position<N> Vectr<N>::calculateReverseYPosition(Vectr<N> & vec, float modifier, const BoundsCheck<N> & check) {
	vec.y = (vec.y * -1) ;
	return calculateNextPositionCh(vec, modifier, check) ;
}

template<typename N>
Position<N> Vectr<N>::calculateNextPosition(Vectr<N> & dir, const Position<N> * current, float modifier, const BoundsCheck<N> & check) {
	Position<float> direc(dir.x, dir.y, dir.z) ;
	Vectr<N> calc = Vectr<N>(direc, current, true) ;
	return calculateNextPositionCh(calc, modifier, check) ;
}


/**
 * Translates a Position from within the World to a Position
 * equivelent within the boundaries of the current screen.
 * Gets the current GLOBAL_max_Position (and min) from World
 * to calculate the ratio
 *
 * @param inGameWorld The Position from within the World
 */
template<typename T>
extern Position<T> transPosition(const Position<T> & inGameWorld) {

	auto worldXSize = globalMaxX() /*- GameObject::GLOBAL_min_X*/ ;
	auto worldYSize = globalMaxY() /*- GameObject::GLOBAL_min_Y*/ ;
	
	T tempX = inGameWorld.getX() /*+ (worldXSize - GameObject::GLOBAL_max_X)*/ ;
	T tempY = inGameWorld.getY() /*+ (worldYSize - GameObject::GLOBAL_max_Y)*/ ;
	
	unsigned tw = termWidth() ;
	unsigned th = termHeight() ;
	
	T x = (tw * tempX)/worldXSize ;
	T y = (th * tempY)/worldYSize ;
	
	return Position<T>(x, y, 0) ;
}

template<typename N>
struct Resolution : public Position<N> {
	
private:
	
	N getZ() const { return 0 ; }
	
	virtual void setZ(const N z) {}
	
public:
	
	Resolution() :
		Position<N>(0, 0 , 0) {}
	
	Resolution(N x, N y) :
		Position<N>(x, y, 0) {}
	
	Resolution(const Resolution & other) :
		Position<N>(other) {}
	
	Resolution(Resolution && other) :
		Position<N>(std::move(other)) {}
	
	Resolution & operator=(const Resolution & rhs) {
		if (this != &rhs) {
			this->Position<N>::operator=(rhs) ;
		}
		return *this ;
	}
	
	~Resolution() {}
	
	Resolution & operator=(Resolution && rhs) {
		if (this != &rhs) {
			this->Position<N>::operator=(std::move(rhs)) ;
		}
		return *this ;
	}
	
	Resolution operator*(const N n) const {
		
        auto x_product = (this->x * n) ;
		auto y_product = (this->y * n) ;
		
		return Resolution<N>(x_product, y_product) ;
	}
	
	double operator/(const Resolution & rhs) const {
		
        double x_quotient = (static_cast<double>(this->getX()) / static_cast<double>(rhs.getX())) ;
		double y_quotient = (static_cast<double>(this->getY()) / static_cast<double>(rhs.getY())) ;
		
		double avrg = average<double>(x_quotient, y_quotient) ;
		
		return avrg ;
	}
	
	friend ostream & operator<<(std::ostream & os, const Resolution<N> * pos) {
		os << pos->x << "x" << pos->y << '\n' ;
		return os ;
	}
	
	friend ostream & operator<<(std::ostream & os, const Resolution<N> & pos) {
		os << pos.x << "x" << pos.y << '\n' ;
		return os ;
	}
	
} ;





















#endif
