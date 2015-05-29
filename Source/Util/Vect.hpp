//
//  Header.h
//
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
	ne = Direction::north + Direction::east,
	se = Direction::south + Direction::east,
	sw = Direction::south + Direction::west,
	nw = Direction::north + Direction::west,
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
 * Vect<> is a simple vector data type.
 *
 * Note: Classes with a Vect data member will typically want to have a pointer,
 * instead of holding the Vect locally. This is because many objects in the World
 * may not actually have a physcical position in space, in which case they can just hold a
 * null pointer.
 */
template <typename N>
struct Vect {

protected:

	N x ;
	N y ;

public:
	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Vect() : x(0), y(0) {}

	/**
     * Creates a Positionwith all coordinates initialized to 0
     */
	Vect(const BoundsCheck<N> & check) : x(0), y(0) { this->checkBounds(check) ; }

	/**
     * Creates a Positionwith all coordinates initialized to n
     */
	Vect(N n) : x(n), y(n) {}

	/**
     * Creates a Positionwith all coordinates initialized to n
     */
	Vect(N n, const BoundsCheck<N> & check) : x(n), y(n) { this->checkBounds(check) ; }


	template<typename M>
	Vect(M x, M y) : x(static_cast<N>(x)), y(static_cast<N>(y)) {}

	/**
     * Creates a Positionwith all coordinates randomized, with bounds set by check
     */
    template<typename R>
	Vect(Randm<R> rand) :
		x(rand.nextValue()),
		y(rand.nextValue()) {}

	/**
     * Creates a Positionwith all coordinates randomized, with bounds set by check
     */
    template<typename R>
	Vect(Randm<R> rand, const BoundsCheck<N> & check) :
		x(rand.nextValue(check.min_X, check.max_X)),
		y(rand.nextValue(check.min_Y, check.max_Y))
	{
		checkBounds(check) ;
	}

    /**
     * Copy constructor for Vect
     */
    Vect(const Vect & other) : Vect(other.x, other.y) {}

	/**
     * Copy constructor for Vect
     */
    Vect(const Vect & other, const BoundsCheck<N> & check) : Vect(other.x, other.y)  {
		checkBounds(check) ;
	}

	/**
     * Move constructor for Vect
     */
    Vect(Vect && other) : Vect(other.x, other.y) {}

	/**
     * Move constructor for Vect
     */
    Vect(Vect && other, const BoundsCheck<N> & check) : Vect(other.x, other.y) {
		checkBounds(check) ;
	}

	/**
     * Creates a Vect with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     */
	Vect(N x, N y) : x(x), y(y) {}

    /**
     * Creates a Vect with coordinates initialized to the
     * given arguments
     *
     * @param x The x coordinate
     * @param y The y coordinate
     */
	Vect(N x, N y, const BoundsCheck<N> & check) : x(x), y(y) {
		this->checkBounds(check) ;
	}

    Vect(Randm<N> & randm) {
        x = randm(BoundsCheck<N>::defaultCheck.min_X, BoundsCheck<N>::defaultCheck.max_X) ;
        y = randm(BoundsCheck<N>::defaultCheck.min_Y, BoundsCheck<N>::defaultCheck.max_Y) ;
    }

    /**
     * Destructor for Vect
     */
    virtual ~Vect() {}


    /**
     * Assigment operator (copy).
     */
    virtual Vect & operator=(const Vect & rhs) {

		this->x = rhs.x ;
		this->y = rhs.y ;

        return * this;
    }


	/**
     * Assigment operator (move)
     */
    virtual Vect & operator=(Vect && rhs) {

		this->x = rhs.x ;
		this->y = rhs.y ;

		return (* this) ;
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

	virtual bool operator == (const Vect & rhs) const {
		if ((this->x == rhs.x) && (this->y == rhs.y)) {
			return true ;
		}
		else {
			return false ;
		}
	}

	virtual bool operator != (const Vect & rhs) const {
		return !(this->operator==(rhs)) ;
	}
	
	virtual bool operator > (const Vect & rhs) const {
		auto this_magnitude = this->magnitude() ;
		auto rhs_magnitude = rhs.magnitude() ;
		return (this_magnitude > rhs_magnitude) ;
	}
	
	virtual bool operator < (const Vect & rhs) const {
		auto this_magnitude = this->magnitude() ;
		auto rhs_magnitude = rhs.magnitude() ;
		return (this_magnitude < rhs_magnitude) ;
	}
	
	virtual bool operator >= (const Vect & rhs) const {
		return ((* this > rhs) || (* this == rhs)) ;
	}
	
	virtual bool operator <= (const Vect & rhs) const {
		return ((* this < rhs) || (* this == rhs)) ;
	}

	virtual Vect operator + (const Vect & rhs) const {

        Vect temp = Vect(this->x, this->y) ;

		temp.x = temp.x + rhs.x ;
		temp.y = temp.y + rhs.y ;

		return temp ;
	}

	virtual Vect operator - (const Vect & rhs) const {

        Vect temp = Vect(this->x, this->y) ;

		temp.x = temp.x - rhs.x ;
		temp.y = temp.y - rhs.y ;

		return temp ;
	}

	virtual Vect operator * (const N n) const {

    	Vect temp = Vect(x, y) ;
        
		temp.x = temp.x * n ;
		temp.y = temp.y * n ;

		return temp ;
	}

	virtual Vect operator / (const N n) const {

    Vect temp = Vect(x, y) ;

		temp.x = temp.x / n ;
		temp.y = temp.y / n ;

		return temp ;
	}
	
	static N dotProduct(const Vect v1, const Vect v2) {
		N n = (v1.x * v2.x) + (v1.y + v2.y) ;
		return n ;
	}
	
	static N distanceBetween(const Vect v1, const Vect v2) {
		const auto sq_x_diff = pow((v2.x - v1.x), 2) ;
		const auto sq_y_diff = pow((v2.y - v1.y), 2) ;
		auto distance = sqrt(sq_x_diff + sq_y_diff) ;
		return distance ;
	}
	
	float magnitude() const {
		auto sumSquares = (pow<float>(x, 2)) + (pow<float>(y, 2)) ;
		float result = sqrt(sumSquares) ;
		return result ;
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
	
	/**
	 * Projects the Vect vect onto this
	 *
	 * @return The new Vect, reflecting the value of vect when projected
	 */
	Vect projectOnto(const Vect & vect) const {
		N dotProduct1 = dotProduct(vect, * this) ;
		N dotProductDivisor = dotProduct(* this, * this) ;
		N divResult = (dotProduct1 / dotProductDivisor) ;
		const Vect<N> result = (* this) * divResult ;
		return result ;
	}

	template<typename O, typename P>
    friend const Vect<P> * operator+(const Vect<P> & lhs, const Vect<O> * rhs) {

		P x = lhs.x + rhs->x ;
		P y = lhs.y + rhs->y ;

		return new Vect<P>(x, y) ;
	}

	template<typename O, typename P>
    friend const Vect<P> * operator-(const Vect<P> & lhs, const Vect<O> * rhs) {

		P x = lhs.x - rhs->x ;
		P y = lhs.y - rhs->y ;

		return new Vect<P>(x, y) ;
	}

	friend ostream & operator<<(std::ostream & os, const Vect<N> * pos) {
		os << "Vect: X = " << pos->x << ", Y = " << pos->y  << '\n' ;
		return os ;
	}

	friend ostream & operator<<(std::ostream & os, const Vect<N> & pos) {
		os << "Vect: X = " << pos.x << ", Y = " << pos.y << '\n' ;
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

	virtual void setAll(const Vect<N> & other) {
		setAll(other.x, other.y) ;
	}

	virtual void setAll(const Vect<N> & other, const BoundsCheck<N> & check) {
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

	virtual void moveHere(const Vect<N> & other) {
		setAll(other.x, other.y) ;
	}

	virtual void moveHere(const Vect<N> & other, const BoundsCheck<N> & check) {
		moveHere(other) ;
		checkBounds(check) ;
	}

    virtual void rotate(Angle 𝛳) {
        const N prevX = x ;
        const N prevY = y ;

        x = (prevX * cos(𝛳.inRadians())) - (prevY * sin(𝛳.inRadians())) ;
        y = (prevX * sin(𝛳.inRadians())) + (prevY * cos(𝛳.inRadians())) ;
    }
	
	/**
	 * Rotates angle degrees around the point centerPoint
	 */
	void rotateAround(const Angle 𝛳, const Vect centerPoint) {
		float s = sin(𝛳);
		float c = cos(𝛳);
			
		// translate point back to origin:
		this->x -= centerPoint.x ;
		this->y -= centerPoint.y;
			
		// rotate point
		float xnew = this->x * c - this->y * s;
		float ynew = this->x * s + this->y * c;
			
		// translate point back:
		this->x = xnew + centerPoint.x;
		this->y = ynew + centerPoint.y;
	}

	static N calcDistance(const Vect & here, const Vect & there) {
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
 * predicting its next Vect, monitoring its speed, maintaining a record of its
 * last two Positions (for more detailed record keeping of past Positions, see Vect),
 * and more.
 *
 * Note: do not use with unsigned ints
 */
template<typename N>
struct VectorAndVelocity : public Vect<float> {

protected:

	static BasicMutex * sharedVelMutex ;

    /**
     * The Vect preceding the most recent Vect
     */
    Vect<N> last ;

    /**
     * The Vect most recently visited
     */
	Vect<N> mostRecent ;

    /**
     * A pointer to the current Vect
     */
	const Vect<N> * current ;

    /**
     * The non-normalized distance between the current Vect and mostRecent Vect
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
	VectorAndVelocity(const Vect<float> & overrideCurrData, const Vect<N> * current_, SafeBoolean tf) ;

	void update() ;


public:

    VectorAndVelocity() ;
    VectorAndVelocity(Angle rotation, SafeBoolean tf) ;
	VectorAndVelocity(Randm<N> randm) ;
	VectorAndVelocity(float headingX, float headingY, Angle rotation, SafeBoolean tf) ;
	VectorAndVelocity(float headingX, float headingY, Vect<N> * current_, Angle rotation, SafeBoolean tf) ;
	VectorAndVelocity(const Vect<N> * current_, Angle rotation, SafeBoolean tf) ;
	VectorAndVelocity(const VectorAndVelocity<N> & other) ;
	VectorAndVelocity(VectorAndVelocity<N> && other) ;
	~VectorAndVelocity() ;

	VectorAndVelocity & operator=(VectorAndVelocity<N> && rhs) ;

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

	VectorAndVelocity & copy(const VectorAndVelocity & other, const Vect * newCurrent) ;
    VectorAndVelocity copy() const ;

	const Velocity<N> * getVelocity() const { return this->velocity ; }

	const Angle * getOrientation() const { return & currentRotation ; }

	Angle copyOrientation() const { return currentRotation ; }

    void modifyOrientation(const Angle & angleOffset) { currentRotation += angleOffset ; }

	void overrideCurrentOrientation(const Angle & newAngle) { currentRotation = newAngle; }

	const Vect * getCurrent() const { return current ; }

	const Vect getLast() const { return this->last ; }

	void updateAndNormalize() ;

	N getLastMoveDistance() const { return absDistanceMoved ; }

	Velocity<N> & calculateVelocity() ;

	Vect<N> calculateNextPosition(float modifier = 1.0) ;

	Vect<N> calculateNextPositionChecked(float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;


	Vect<N> calculateReverseNextPosition(float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;


	Vect<N> calculateReverseXPosition(float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;


	Vect<N> calculateReverseYPosition(float modifier = 1.0, const BoundsCheck<N> & = BoundsCheck<N>::defaultCheck) ;


} ;

template<typename N>
BasicMutex * VectorAndVelocity<N>::sharedVelMutex = Velocity<N>::sharedVelMutex ;

template<typename N>
VectorAndVelocity<N>::VectorAndVelocity() :
    Vect<float>(0, -1), /* default direction is up */
	current(nullptr),
    totalDistanceMoved(new N()),
	velocity(nullptr),
    currentRotation(0.0){}

template<typename N>
VectorAndVelocity<N>::VectorAndVelocity(Angle rotation, SafeBoolean tf) :
    Vect<float>(0, -1), /* default direction is up */
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
VectorAndVelocity<N>::VectorAndVelocity(Randm<N> randm) :
	Vect<float>(randm(), randm()),
	current(nullptr),
	totalDistanceMoved(new N()),
	velocity(nullptr)
{
	Randm<float> randmf(0, 360) ;
	currentRotation = randmf() ;
}

template<typename N>
VectorAndVelocity<N>::VectorAndVelocity(float headingX, float headingY, Angle rotation, SafeBoolean tf) :
	Vect<float>(headingX, headingY),
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
VectorAndVelocity<N>::VectorAndVelocity(float headingX, float headingY, Vect<N> * current_, Angle rotation, SafeBoolean tf) :
	Vect<float>(headingX, headingY),
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
VectorAndVelocity<N>::VectorAndVelocity(const Vect<N> * current_, Angle rotation, SafeBoolean tf) :
	Vect<float>(0, -1),
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
VectorAndVelocity<N>::VectorAndVelocity(const VectorAndVelocity<N> & other) :
	Vect<float>(other),
    last(Vect<N>(other.last)),
	mostRecent(Vect<N>(other.mostRecent)),
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
VectorAndVelocity<N>::VectorAndVelocity(VectorAndVelocity<N> && other) :
	Vect<float>(std::move(other)),
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
VectorAndVelocity<N>::~VectorAndVelocity()
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
VectorAndVelocity<N> & VectorAndVelocity<N>::operator=(VectorAndVelocity<N> && rhs) {
	if (this != &rhs) {

		this->Vect<float>::operator=(std::move(rhs)) ;
        this->last = Vect<N>(rhs.last) ;
		this->mostRecent = Vect<N>(rhs.mostRecent) ;
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
void VectorAndVelocity<N>::setVectorAndOrientation(Angle 𝛳) {

    normalize() ;

    this->Vect<N>::rotate(𝛳) ;

	this->currentRotation = 𝛳 ; //i.e. (currentRotation + abs𝛳) % 360

	assert((currentRotation == 𝛳)) ; /* debug code, remove */

	/* we should still be normalized here */
}

template<typename N>
void VectorAndVelocity<N>::rotateVectorAndOrientation(Angle 𝛳) {

	normalize() ;

	this->Vect<N>::rotate(𝛳) ;

	this->currentRotation += 𝛳 ; //i.e. (currentRotation + abs𝛳) % 360

	/* we should still be normalized here */
}

template<typename N>
void VectorAndVelocity<N>::rotateVector(Angle 𝛳) {

    normalize() ;

    this->Vect<N>::rotate(𝛳) ;
}

template<typename N>
void VectorAndVelocity<N>::rotateOrientation(Angle 𝛳) {
    this->currentRotation += 𝛳 ;
    //i.e. (currentRotation + abs𝛳) % 360
}

template<typename N>
VectorAndVelocity<N> & VectorAndVelocity<N>::copy(const VectorAndVelocity & other, const Vect * newCurrent) {

	this->Vect<float>::operator=(other) ;

	this->last = Vect<N>(other.last) ;
	this->mostRecent = Vect<N>(other.mostRecent) ;
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
VectorAndVelocity<N> VectorAndVelocity<N>::copy() const {

	VectorAndVelocity<N> newVect ;
	newVect.copyVect(*this) ;

    return std::move(newVect) ;
}

template<typename N>
ostream & operator<<(std::ostream & os, const VectorAndVelocity<N> * vec) {
	os << "Vector: X = " << vec->x << ", Y = " << vec->y << ", Z = " << vec->z << '\n' ;
	return os ;
}

template<typename N>
ostream & operator<<(std::ostream & os, const VectorAndVelocity<N> & vec) {
	os << "Vector: X = " << vec.x << ", Y = " << vec.y << ", Z = " << vec.z << '\n' ;
	return os ;
}


template<typename N>
void VectorAndVelocity<N>::update() {

	if ((current != nullptr) && (mostRecent != *current)) { //only if we've moved...

        absDistanceMoved = calcEuclidianDistance(mostRecent, *current) ;
		*totalDistanceMoved += absDistanceMoved ;

		Vect<N> temp = ((*current) - mostRecent) ;               /*  uses Vect operator- overload to subtract
                                                                      our x, y, and z (which are offset values) from those
                                                                      stored in current, giving our new location  */
		setAll(temp.getX(), temp.getY()) ;
        last = mostRecent ;
		mostRecent = std::move((Vect<N>(*this->current))) ;
	}

}

template<typename N>
void VectorAndVelocity<N>::updateAndNormalize() {
	update() ;
	normalize() ;
}

template<typename N>
Velocity<N> & VectorAndVelocity<N>::calculateVelocity() {
	return this->velocity ;
}

template<typename N>
Vect<N> VectorAndVelocity<N>::calculateNextPosition(float modifier) {

	normalize() ;

	auto stor = *this * modifier ;

	N nx ;
	N ny ;
	N nz ;

	nx = (current)->getX() + stor.getX() ;
	ny = (current)->getY() + stor.getY() ;

	Vect<N> next(nx, ny) ;

	return next ;
}

template<typename N>
Vect<N> VectorAndVelocity<N>::calculateNextPositionChecked(float modifier, const BoundsCheck<N> & check) {

	normalize() ;

	auto stor = *this * modifier ;

	N nx ;
	N ny ;
	N nz ;

	nx = (current)->getX() + stor.getX() ;
	ny = (current)->getY() + stor.getY() ;

	Vect<N> next(nx, ny, check) ;

	return next ;
}

template<typename N>
Vect<N> VectorAndVelocity<N>::calculateReverseNextPosition(float modifier, const BoundsCheck<N> & check) {
	x = (x * -1) ;
	y = (y * -1) ;
	return calculateNextPosition(modifier, check) ;
}

template<typename N>
Vect<N> VectorAndVelocity<N>::calculateReverseXPosition(float modifier, const BoundsCheck<N> & check) {
	x = (x * -1) ;
	return calculateNextPositionChecked(modifier, check) ;
}

template<typename N>
Vect<N> VectorAndVelocity<N>::calculateReverseYPosition(float modifier, const BoundsCheck<N> & check) {
	y = (y * -1) ;
	return calculateNextPositionChecked(modifier, check) ;
}

template<typename N>
Vect<N> & operator+=(Vect<N> & rhs, const Vect<N> & lhs) {
	rhs.setX(rhs.getX() + lhs.getX()) ;
	rhs.setY(rhs.getY() + lhs.getY()) ;
	return rhs ;
}

template<typename N>
Vect<N> & operator-=(Vect<N> & lhs, const Vect<N> & rhs) {
	lhs.setX(lhs.getX() - rhs.getX()) ;
	lhs.setY(lhs.getY() - rhs.getY()) ;
	return lhs ;
}

/*
template<typename N>
VectorAndVelocity<N> & operator+=(VectorAndVelocity<N> & rhs, Vect<N> & lhs) {
    rhs.setX(rhs.getX() + lhs.getX()) ;
    rhs.setY(rhs.getY() + lhs.getY()) ;
    return rhs ;
}

template<typename N>
VectorAndVelocity<N> & operator-=(VectorAndVelocity<N> & rhs, Vect<N> & lhs) {
    rhs.setX(rhs.getX() - lhs.getX()) ;
    rhs.setY(rhs.getY() - lhs.getY()) ;
    return rhs ;
}

*/


/**
 * Translates a Vect from within the World to a Vect
 * equivelent within the boundaries of the current screen.
 * Gets the current GLOBAL_max_Position (and min) from World
 * to calculate the ratio
 *
 * @param inSpriteFight The Vect from within the World
 */
template<typename T>
extern Vect<T> transPosition(const Vect<T> & inSpriteFight) {

	auto worldXSize = globalMaxX() /*- GameObject::GLOBAL_min_X*/ ;
	auto worldYSize = globalMaxY() /*- GameObject::GLOBAL_min_Y*/ ;

	T tempX = inSpriteFight.getX() /*+ (worldXSize - GameObject::GLOBAL_max_X)*/ ;
	T tempY = inSpriteFight.getY() /*+ (worldYSize - GameObject::GLOBAL_max_Y)*/ ;

	unsigned tw = termWidth() ;
	unsigned th = termHeight() ;

	T x = (tw * tempX)/worldXSize ;
	T y = (th * tempY)/worldYSize ;

	return Vect<T>(x, y) ;
}

template<typename N>
struct Resolution : public Vect<N> {

private:

	N getZ() const { return 0 ; }

	virtual void setZ(const N z) {}

public:

	Resolution() :
		Vect<N>(0, 0) {}

	Resolution(N x, N y) :
		Vect<N>(x, y) {}

	Resolution(const Resolution & other) :
		Vect<N>(other) {}

	Resolution(Resolution && other) :
		Vect<N>(std::move(other)) {}

	Resolution & operator=(const Resolution & rhs) {
		if (this != &rhs) {
			this->Vect<N>::operator=(rhs) ;
		}
		return *this ;
	}

	~Resolution() {}

	Resolution & operator=(Resolution && rhs) {
		if (this != &rhs) {
			this->Vect<N>::operator=(std::move(rhs)) ;
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
