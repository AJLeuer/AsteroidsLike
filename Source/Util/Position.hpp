//
//  Header.h
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//


#ifndef SpriteFight_Position_h
#define SpriteFight_Position_h


#include <complex>
#include <valarray>
#include <iostream>
#include <ostream>
#include <sstream>
#include <queue>
#include <vector>
#include <array>
#include <tuple>
#include <initializer_list>
#include <deque>

#include <SDL2/SDL_rect.h>

#include "Debug.h"
#include "Util.hpp"
#include "BoundsCheck.hpp"
#include "Velocity.hpp"
#include "Random.hpp"
#include "BasicConcurrency.h"

#include "../Control/GlobalVar.h"

#define UP 0, -1, 0
#define DOWN 0, 1, 0
#define LEFT -1, 0, 0
#define RIGHT 1, 0, 0


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
 * @brief A storage class for angular values. All underlying values are represented in degrees.
 *
 * @note Since the standard library trigonometric functions (i.e. sin(), cos(), etc.) expect
 * argument values expressed in radians, values passed to those functions should first be converted.
 * Angle provides the member function valInRadians() for just that purpose.
 */
class Angle {

protected:

	float value ;

public:

	constexpr inline Angle(const float angle) : value(angle) {
		value = Mod(value, 360.0f) ;
	}

	constexpr inline Angle(const Angle & other) : value(other.value) {
		value = Mod(value, 360.0f) ;
	}

    template<typename N>
    constexpr inline Angle(Randm<N> & randm) {
        value = randm(0, 360) ;
    }
	
	constexpr inline operator float() const {
		return value ;
	}

	constexpr inline Angle & operator = (const Angle & rhs) {
		if (this != &rhs) {
			this->value = Mod(rhs.value, 360.0f) ;
		}
		return *this ;
	}

	constexpr inline Angle & operator = (const float & f) {
		value = Mod(f, 360.0f) ;
		return *this ;
	}

	constexpr inline Angle operator + (const float otherAngle) const {
		Angle val(this->value + otherAngle) ;
		return val ;
	}

	constexpr inline Angle operator - (const float otherAngle) const {
		Angle val(this->value - otherAngle) ;
		return val ;
	}

	constexpr inline void operator += (const float otherAngle) {
		this->value = Mod((value + otherAngle), 360.0f) ;
	}

	constexpr inline void operator -= (const float otherAngle) {
		this->value = Mod((value - otherAngle), 360.0f) ;
	}

    float inRadians() {
        return (convertToRadians<float>(((float) * this))) ;
    }


} ;


/**
 * Position<> is a simple vector data type.
 *
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

public:
	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Position() : x(0), y(0) {}

	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Position(const BoundsCheck<N> & check) : x(0), y(0) { this->checkBounds(check) ; }

	/**
     * Creates a Positionwith all coordinates initialized to n
     */
	Position(N n) : x(n), y(n) {}

	/**
     * Creates a Positionwith all coordinates initialized to n
     */
	Position(N n, const BoundsCheck<N> & check) : x(n), y(n) { this->checkBounds(check) ; }


	template<typename M>
	Position(M x, M y) : x(static_cast<N>(x)), y(static_cast<N>(y)) {}

	/**
     * Creates a Positionwith all coordinates randomized, with bounds set by check
     */
    template<typename R>
	Position(Randm<R> rand) :
		x(rand.nextValue()),
		y(rand.nextValue()) {}

	/**
     * Creates a Positionwith all coordinates randomized, with bounds set by check
     */
    template<typename R>
	Position(Randm<R> rand, const BoundsCheck<N> & check) :
		x(rand.nextValue(check.min_X, check.max_X)),
		y(rand.nextValue(check.min_Y, check.max_Y))
	{
		checkBounds(check) ;
	}

    /**
     * Copy constructor for Position
     */
    Position(const Position & other) : Position(other.x, other.y) {}

	/**
     * Copy constructor for Position
     */
    Position(const Position & other, const BoundsCheck<N> & check) : Position(other.x, other.y)  {
		checkBounds(check) ;
	}

	/**
     * Move constructor for Position
     */
    Position(Position && other) : Position(other.x, other.y) {}

	/**
     * Move constructor for Position
     */
    Position(Position && other, const BoundsCheck<N> & check) : Position(other.x, other.y) {
		checkBounds(check) ;
	}

	/**
     * Creates a Position with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     */
	Position(N x, N y) : x(x), y(y) {}

    /**
     * Creates a Position with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     */
	Position(N x, N y, const BoundsCheck<N> & check) : x(x), y(y) {
		this->checkBounds(check) ;
	}

    Position(Randm<N> & randm) {
        x = randm(BoundsCheck<N>::defaultCheck.min_X, BoundsCheck<N>::defaultCheck.max_X) ;
        y = randm(BoundsCheck<N>::defaultCheck.min_Y, BoundsCheck<N>::defaultCheck.max_Y) ;
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

        return *this;
    }


	/**
     * Assigment operator (move)
     */
    virtual Position & operator=(Position && rhs) {

		this->x = rhs.x ;
		this->y = rhs.y ;

		return(*this) ;
    }

	/**
	 * Typecast to SDL_Point operator overload
	 */
	operator SDL_Point() {
		SDL_Point point ;
		point.x = this->x ;
		point.y = this->y ;
		return point ;
	}

	virtual bool operator==(const Position & rhs) const {
		if ((this->x == rhs.x) && (this->y == rhs.y)) {
			return true ;
		}
		else {
			return false ;
		}
	}

	virtual bool operator!=(const Position & rhs) const {
		return !(this->operator==(rhs)) ;
	}

	virtual Position operator+(const Position & rhs) const {

        Position temp = Position(this->x, this->y) ;

		temp.x = temp.x + rhs.x ;
		temp.y = temp.y + rhs.y ;

		return temp ;
	}

	virtual Position operator-(const Position & rhs) const {

        Position temp = Position(this->x, this->y) ;

		temp.x = temp.x - rhs.x ;
		temp.y = temp.y - rhs.y ;

		return temp ;
	}

	virtual Position operator*(const N n) const {

    Position temp = Position(x, y) ;
        
		temp.x = temp.x * n ;
		temp.y = temp.y * n ;

		return temp ;
	}

	virtual Position operator/(const N n) const {

    Position temp = Position(x, y) ;

		temp.x = temp.x / n ;
		temp.y = temp.y / n ;

		return temp ;
	}


	virtual void normalize() {
		auto distance = pythag<float>(x, y) ;
		if (distance != 0) {
			x = (x / distance) ;
		}
		if (distance != 0) {
			y = (y / distance) ;
		}
	}

	template<typename O, typename P>
    friend const Position<P> * operator+(const Position<P> & lhs, const Position<O> * rhs) {

		P x = lhs.x + rhs->x ;
		P y = lhs.y + rhs->y ;

		return new Position<P>(x, y) ;
	}

	template<typename O, typename P>
    friend const Position<P> * operator-(const Position<P> & lhs, const Position<O> * rhs) {

		P x = lhs.x - rhs->x ;
		P y = lhs.y - rhs->y ;

		return new Position<P>(x, y) ;
	}

	friend ostream & operator<<(std::ostream & os, const Position<N> * pos) {
		os << "Position: X = " << pos->x << ", Y = " << pos->y  << '\n' ;
		return os ;
	}

	friend ostream & operator<<(std::ostream & os, const Position<N> & pos) {
		os << "Position: X = " << pos.x << ", Y = " << pos.y << '\n' ;
		return os ;
	}

	/**
	 * Sets x, y, and z to the given values.
	 */
	virtual void setAll(const N x, const N y) {
		this->x = x ;
		this->y = y ;
	}

	virtual void setAll(const N x, const N y, const BoundsCheck<N> & check) {
		setAll(x, y) ;
		checkBounds(&check) ;
	}

	virtual void setAll(const Position<N> & other) {
		setAll(other.x, other.y) ;
	}

	virtual void setAll(const Position<N> & other, const BoundsCheck<N> & check) {
		setAll(other.x, other.y) ;
		checkBounds(&check) ;
	}

	virtual void setAll(const N n) { setAll(n, n) ; }

	virtual void setAll(const N n, const BoundsCheck<N> & check) { setAll(n, n, check) ; }

	virtual void setAllZero() { setAll(0) ; }


	N getX() const { return this->x ; }

	N getY() const { return this->y ; }

	/**
	 * @return x as an integer
	 */
	int getIntX() const { return roundF<N, int>(x) ; }

	/**
	 * @return y as an integer
	 */
	int getIntY() const { return roundF<N, int>(y) ; }

	virtual void setX(const N x) { setAll(x, this->y) ; }

	virtual void setX(const N x, const BoundsCheck<N> & check) { setX(x) ; checkBounds(check) ; }

	virtual void setY(const N y) { setAll(this->x, y) ; }

	virtual void setY(const N y, const BoundsCheck<N> & check) { setY(y) ; checkBounds(check) ; }


	virtual void x_plus_one() { setX(x++) ; }

	virtual void x_plus_one(const BoundsCheck<N> & check) { setX(x++) ; checkBounds(check) ; }

	virtual void x_minus_one() { setX(x--) ; }

	virtual void x_minus_one(const BoundsCheck<N> & check) { setX(x--) ; checkBounds(check) ; }


	virtual void y_plus_one() { setY(y++) ; }

	virtual void y_plus_one(const BoundsCheck<N> & check) { setY(y++) ; checkBounds(check) ; }

	virtual void y_minus_one() { setY(y--) ; }

	virtual void y_minus_one(const BoundsCheck<N> & check) { setY(y--) ; checkBounds(check) ; }


	/**
	 * Increments or decrements the x, y and z values according to
	 * the arguments passed in. Use negative values to decrement. Passing
	 * 0 for any argument will keep the x, y, or z value the same.
	 *
	 * @param delta_x The change in x value
	 * @param delta_y The change in y value
	 * @param delta_z The change in z value
	 */
	virtual void modify(N delta_x, N delta_y) {
		auto tempX = this->x ;
		auto tempY = this->y ;

		tempX += delta_x ;
		tempY += delta_y ;

		setAll(tempX, tempY) ;
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
	virtual void modify(N delta_x, N delta_y, const BoundsCheck<N> & check) {
		auto tempX = this->x ;
		auto tempY = this->y ;

		tempX += delta_x ;
		tempY += delta_y ;

		setAll(tempX, tempY, check) ;
	}

	virtual void moveHere(N x, N y) {
		setAll(x, y) ;
	}

	virtual void moveHere(N x, N y, const BoundsCheck<N> & check) {
		moveHere(x, y) ;
		checkBounds(check) ;
	}

	virtual void moveHere(const Position<N> & other) {
		setAll(other.x, other.y) ;
	}

	virtual void moveHere(const Position<N> & other, const BoundsCheck<N> & check) {
		moveHere(other) ;
		checkBounds(check) ;
	}

    virtual void rotate(Angle 𝛳) {
        const N prevX = x ;
        const N prevY = y ;

        x = (prevX * cos(𝛳.inRadians())) - (prevY * sin(𝛳.inRadians())) ;
        y = (prevX * sin(𝛳.inRadians())) + (prevY * cos(𝛳.inRadians())) ;
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


	void checkBounds(const BoundsCheck<N> * check) {

		if ((this->x) >= check->max_X) {
			{
			/* Debug code */
			DebugOutput << "An X value was over the global limit. Reducing value... \n" ;
			/* End Debug code */
			}
			this->x = check->max_X /*- objWidth*/ - 1 ;
		}
		if (this->x < check->min_X) {
			{
			/* Debug code */
			DebugOutput << "An X value was under the global minimum. Increasing value... \n" ;
			/* End Debug code */
			}
			this->x = check->min_X /*+ objWidth*/ ;
		}
		if ((this->y) >= check->max_Y) {
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

	void checkBounds(const BoundsCheck<N> & check) {

		if ((this->x) >= check.max_X) {
			{
				/* Debug code */
				DebugOutput << "An X value was over the global limit. Reducing value... \n" ;
				/* End Debug code */
			}
			this->x = check.max_X - 1 ;
		}
		if (this->x < check.min_X) {
			{
				/* Debug code */
				DebugOutput << "An X value was under the global minimum. Increasing value... \n" ;
				/* End Debug code */
			}
			this->x = check.min_X ;
		}
		if ((this->y) >= check.max_Y) {
			{
				/* Debug code */
				DebugOutput << "A Y value was over the global limit. Reducing value... \n" ;
				/* End Debug code */
			}
			this->y = check.max_Y - 1 ;
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

	bool overBounds(const BoundsCheck<N> & check) const {
        if (overXBounds(& check) || overYBounds(& check)) {
            return true ;
        }
		else {
			return false ;
		}
	}

	bool overXBounds(const BoundsCheck<N> * check) const {
		if (((this->x) >= check->max_X) || ((this->x) < check->min_X)) {
			return true ;
		}
		else {
			return false ;
		}
	}

	bool overYBounds(const BoundsCheck<N> * check) const {
		if (((this->y) >= check->max_Y) || ((this->y) < check->min_Y)) {
			return true ;
		}
		else {
			return false ;
		}
	}

} ;



/**
 * This class provides facilities for storing an object's current vector,
 * predicting its next Position, monitoring its speed, maintaining a record of its
 * last two Positions (for more detailed record keeping of past Positions, see Position),
 * and more.
 *
 * Note: do not use with unsigned ints
 */
template<typename N>
struct Vectr : public Position<float> {

protected:

	static BasicMutex * sharedVelMutex ;

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

	/* The current rotation of this vector */
	Angle currentRotation = 0.0 ;


	bool sharedVelBool = true ;

	/* x, y, and z here (the one we inherited) will be used as deltas that we can add to current to calculate next */
	Vectr(const Position<float> & overrideCurrData, const Position<N> * current_, SafeBoolean tf) ;

	void update() ;


public:

    Vectr() ;
    Vectr(Angle rotation, SafeBoolean tf) ;
	Vectr(Randm<N> randm) ;
	Vectr(float headingX, float headingY, Angle rotation, SafeBoolean tf) ;
	Vectr(float headingX, float headingY, Position<N> * current_, Angle rotation, SafeBoolean tf) ;
	Vectr(const Position<N> * current_, Angle rotation, SafeBoolean tf) ;
	Vectr(const Vectr<N> & other) ;
	Vectr(Vectr<N> && other) ;
	~Vectr() ;

	Vectr & operator=(Vectr<N> && rhs) ;

	/**
	 * Rotate the number of degrees equal to the difference between
	 * this vectors current rotation, and the given angle 𝛳
	 */
	void setVectorAndOrientation(Angle 𝛳) ;

	/**
	 * Rotate the number of degrees equal to the difference between
	 * this vectors current rotation, and the given angle 𝛳
	 */
	void rotateVectorAndOrientation(Angle 𝛳) ;

    void rotateVector(Angle 𝛳) ;

    void rotateOrientation(Angle 𝛳) ;

	Vectr & copy(const Vectr & other, const Position * newCurrent) ;
    Vectr copy() const ;

	const Velocity<N> * getVelocity() const { return this->velocity ; }

	const Angle * getOrientation() const { return & currentRotation ; }

	Angle copyOrientation() const { return currentRotation ; }

    void modifyOrientation(const Angle & angleOffset) { currentRotation += angleOffset ; }

	void overrideCurrentOrientation(const Angle & newAngle) { currentRotation = newAngle; }

	const Position * getCurrent() const { return current ; }

	const Position getLast() const { return this->last ; }

	void updateAndNormalize() ;

	N getLastMoveDistance() const { return absDistanceMoved ; }

	Velocity<N> & calculateVelocity() ;

	Position<N> calculateNextPosition(float modifier = 1.0) ;

	Position<N> calculateNextPositionChecked(float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;


	Position<N> calculateReverseNextPosition(float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;


	Position<N> calculateReverseXPosition(float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;


	Position<N> calculateReverseYPosition(float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;


} ;

template<typename N>
BasicMutex * Vectr<N>::sharedVelMutex = Velocity<N>::sharedVelMutex ;

template<typename N>
Vectr<N>::Vectr() :
    Position<float>(0, -1), /* default direction is up */
	current(nullptr),
    totalDistanceMoved(new N()),
	velocity(nullptr),
    currentRotation(0.0){}

template<typename N>
Vectr<N>::Vectr(Angle rotation, SafeBoolean tf) :
    Position<float>(0, -1), /* default direction is up */
    current(nullptr),
    totalDistanceMoved(new N()),
    velocity(nullptr),
    currentRotation(rotation)
{
	if (tf == SafeBoolean::t) {
		velocity = new Velocity<N>(totalDistanceMoved, &sharedVelBool) ;
	}
	else { // if (tf == SafeBoolean::f)
		velocity = nullptr ;
	}
}

template<typename N>
Vectr<N>::Vectr(Randm<N> randm) :
	Position<float>(randm(), randm()),
	current(nullptr),
	totalDistanceMoved(new N()),
	velocity(nullptr)
{
	Randm<float> randmf(0, 360) ;
	currentRotation = randmf() ;
}

template<typename N>
Vectr<N>::Vectr(float headingX, float headingY, Angle rotation, SafeBoolean tf) :
	Position<float>(headingX, headingY),
	current(nullptr),
	totalDistanceMoved(new N),
    currentRotation(rotation)
{
	if (tf == SafeBoolean::t) {
		velocity = new Velocity<N>(totalDistanceMoved, &sharedVelBool) ;
	}
	else { // if (tf == SafeBoolean::f)
		velocity = nullptr ;
	}
}

template<typename N>
Vectr<N>::Vectr(float headingX, float headingY, Position<N> * current_, Angle rotation, SafeBoolean tf) :
	Position<float>(headingX, headingY),
	current(current_),
	totalDistanceMoved(new N),
    currentRotation(rotation)
{
	if (tf == SafeBoolean::t) {
		velocity = new Velocity<N>(totalDistanceMoved, &sharedVelBool) ;
	}
	else { // if (tf == SafeBoolean::f)
		velocity = nullptr ;
	}
}

template<typename N>
Vectr<N>::Vectr(const Position<N> * current_, Angle rotation, SafeBoolean tf) :
	Position<float>(0, -1),
    last(*current_),
	mostRecent(*current_),
    current(current_),
	totalDistanceMoved(new N),
    currentRotation(rotation)
{
	if (tf == SafeBoolean::t) {
		velocity = new Velocity<N>(totalDistanceMoved, &sharedVelBool) ;
	}
	else { // if (tf == SafeBoolean::f)
		velocity = nullptr ;
	}
}


template<typename N>
Vectr<N>::Vectr(const Vectr<N> & other) :
	Position<float>(other),
    last(Position<N>(other.last)),
	mostRecent(Position<N>(other.mostRecent)),
	current(other.current),
    absDistanceMoved(other.absDistanceMoved),
	totalDistanceMoved(new N(*other.totalDistanceMoved)),
	currentRotation(other.currentRotation)
{
	if (other.velocity != nullptr) {
		velocity = new Velocity<N>(totalDistanceMoved, &sharedVelBool) ;
	}
	else { // if (tf == SafeBoolean::f)
		velocity = nullptr ;
	}
}


template<typename N>
Vectr<N>::Vectr(Vectr<N> && other) :
	Position<float>(std::move(other)),
    last(std::move(other.last)),
	mostRecent(std::move(other.mostRecent)), current(other.current),
    absDistanceMoved(other.absDistanceMoved),
	totalDistanceMoved(other.totalDistanceMoved),
	velocity(other.velocity),
	currentRotation(std::move(other.currentRotation))
{
	other.current = nullptr ;
	other.totalDistanceMoved = nullptr ;
	other.velocity = nullptr ;
}

template<typename N>
Vectr<N>::~Vectr()
{
	/* check if we're running a velocity monitor first */
	if (velocity != nullptr) {
		sharedVelMutex->lock() ;
	}

	sharedVelBool = false ;

	this->current = nullptr ;

	if (velocity != nullptr) {
		delete velocity;
	}

	if (totalDistanceMoved != nullptr) {
		delete totalDistanceMoved ;
	}

	if (velocity != nullptr) {
		sharedVelMutex->unlock() ;
	}
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
		this->currentRotation = std::move(rhs.currentRotation) ;

		rhs.current = nullptr ;
		rhs.totalDistanceMoved = nullptr ;
		rhs.velocity = nullptr ;
	}
	return *this ;
}

template<typename N>
void Vectr<N>::setVectorAndOrientation(Angle 𝛳) {

    normalize() ;

    this->Position<N>::rotate(𝛳) ;

	this->currentRotation = 𝛳 ; //i.e. (currentRotation + abs𝛳) % 360

	assert((currentRotation == 𝛳)) ; /* debug code, remove */

	/* we should still be normalized here */
}

template<typename N>
void Vectr<N>::rotateVectorAndOrientation(Angle 𝛳) {

	normalize() ;

	this->Position<N>::rotate(𝛳) ;

	this->currentRotation += 𝛳 ; //i.e. (currentRotation + abs𝛳) % 360

	/* we should still be normalized here */
}

template<typename N>
void Vectr<N>::rotateVector(Angle 𝛳) {

    normalize() ;

    this->Position<N>::rotate(𝛳) ;
}

template<typename N>
void Vectr<N>::rotateOrientation(Angle 𝛳) {
    this->currentRotation += 𝛳 ;
    //i.e. (currentRotation + abs𝛳) % 360
}

template<typename N>
Vectr<N> & Vectr<N>::copy(const Vectr & other, const Position * newCurrent) {

	this->Position<float>::operator=(other) ;

	this->last = Position<N>(other.last) ;
	this->mostRecent = Position<N>(other.mostRecent) ;
	this->current = newCurrent ;
	this->absDistanceMoved = other.absDistanceMoved ;
	this->totalDistanceMoved = new N(*other.totalDistanceMoved) ;
	this->currentRotation = other.currentRotation ;

	if (other.velocity != nullptr) {
		this->velocity = new Velocity<N>(this->totalDistanceMoved, &this->sharedVelBool) ;
	}
	else {
		this->velocity = nullptr ;
	}

	return *this ;
}

template<typename N>
Vectr<N> Vectr<N>::copy() const {

	Vectr<N> newVect ;
	newVect.copyVect(*this) ;

    return std::move(newVect) ;
}

template<typename N>
ostream & operator<<(std::ostream & os, const Vectr<N> * vec) {
	os << "Vector: X = " << vec->x << ", Y = " << vec->y << ", Z = " << vec->z << '\n' ;
	return os ;
}

template<typename N>
ostream & operator<<(std::ostream & os, const Vectr<N> & vec) {
	os << "Vector: X = " << vec.x << ", Y = " << vec.y << ", Z = " << vec.z << '\n' ;
	return os ;
}


template<typename N>
void Vectr<N>::update() {

	if ((current != nullptr) && (mostRecent != *current)) { //only if we've moved...

        absDistanceMoved = calcEuclidianDistance(mostRecent, *current) ;
		*totalDistanceMoved += absDistanceMoved ;

		Position<N> temp = ((*current) - mostRecent) ;               /*  uses Position operator- overload to subtract
                                                                      our x, y, and z (which are offset values) from those
                                                                      stored in current, giving our new location  */
		setAll(temp.getX(), temp.getY()) ;
        last = mostRecent ;
		mostRecent = std::move((Position<N>(*this->current))) ;
	}

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
Position<N> Vectr<N>::calculateNextPosition(float modifier) {

	normalize() ;

	auto stor = *this * modifier ;

	N nx ;
	N ny ;
	N nz ;

	nx = (current)->getX() + stor.getX() ;
	ny = (current)->getY() + stor.getY() ;

	Position<N> next(nx, ny) ;

	return next ;
}

template<typename N>
Position<N> Vectr<N>::calculateNextPositionChecked(float modifier, const BoundsCheck<N> & check) {

	normalize() ;

	auto stor = *this * modifier ;

	N nx ;
	N ny ;
	N nz ;

	nx = (current)->getX() + stor.getX() ;
	ny = (current)->getY() + stor.getY() ;

	Position<N> next(nx, ny, check) ;

	return next ;
}

template<typename N>
Position<N> Vectr<N>::calculateReverseNextPosition(float modifier, const BoundsCheck<N> & check) {
	x = (x * -1) ;
	y = (y * -1) ;
	return calculateNextPosition(modifier, check) ;
}

template<typename N>
Position<N> Vectr<N>::calculateReverseXPosition(float modifier, const BoundsCheck<N> & check) {
	x = (x * -1) ;
	return calculateNextPositionChecked(modifier, check) ;
}

template<typename N>
Position<N> Vectr<N>::calculateReverseYPosition(float modifier, const BoundsCheck<N> & check) {
	y = (y * -1) ;
	return calculateNextPositionChecked(modifier, check) ;
}

template<typename N>
Position<N> & operator+=(Position<N> & rhs, const Position<N> & lhs) {
	rhs.setX(rhs.getX() + lhs.getX()) ;
	rhs.setY(rhs.getY() + lhs.getY()) ;
	return rhs ;
}

template<typename N>
Position<N> & operator-=(Position<N> & lhs, const Position<N> & rhs) {
	lhs.setX(lhs.getX() - rhs.getX()) ;
	lhs.setY(lhs.getY() - rhs.getY()) ;
	return lhs ;
}

/*
template<typename N>
Vectr<N> & operator+=(Vectr<N> & rhs, Position<N> & lhs) {
    rhs.setX(rhs.getX() + lhs.getX()) ;
    rhs.setY(rhs.getY() + lhs.getY()) ;
    return rhs ;
}

template<typename N>
Vectr<N> & operator-=(Vectr<N> & rhs, Position<N> & lhs) {
    rhs.setX(rhs.getX() - lhs.getX()) ;
    rhs.setY(rhs.getY() - lhs.getY()) ;
    return rhs ;
}

*/


/**
 * Translates a Position from within the World to a Position
 * equivelent within the boundaries of the current screen.
 * Gets the current GLOBAL_max_Position (and min) from World
 * to calculate the ratio
 *
 * @param inSpriteFight The Position from within the World
 */
template<typename T>
extern Position<T> transPosition(const Position<T> & inSpriteFight) {

	auto worldXSize = globalMaxX() /*- GameObject::GLOBAL_min_X*/ ;
	auto worldYSize = globalMaxY() /*- GameObject::GLOBAL_min_Y*/ ;

	T tempX = inSpriteFight.getX() /*+ (worldXSize - GameObject::GLOBAL_max_X)*/ ;
	T tempY = inSpriteFight.getY() /*+ (worldYSize - GameObject::GLOBAL_max_Y)*/ ;

	unsigned tw = termWidth() ;
	unsigned th = termHeight() ;

	T x = (tw * tempX)/worldXSize ;
	T y = (th * tempY)/worldYSize ;

	return Position<T>(x, y) ;
}

template<typename N>
struct Resolution : public Position<N> {

private:

	N getZ() const { return 0 ; }

	virtual void setZ(const N z) {}

public:

	Resolution() :
		Position<N>(0, 0) {}

	Resolution(N x, N y) :
		Position<N>(x, y) {}

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
