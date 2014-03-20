//
//  GameObject.h
//  GameWorld
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __GameWorld__GameObject__
#define __GameWorld__GameObject__

#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

#include "Util.h"
#include "Location.h"
#include "GameInterface.h"

using namespace::std ;

/*
 * The base class from which all other classes in the world (thought not the world
 * itself) will inherit. This class will handle the assignment of a unique ID to each GameObject.
 */
class GameObject : public GameInterface {
	
	
private:
	
	static unsigned IDs ;
	static vector<string> icons ;
	static char nameLetters[] ;
	
protected:

	unsigned ID ;
	string icon ;
    Location * loc ;
	
	
public:
	
	static const double GLOBAL_MAX_X ;
	static const double GLOBAL_MIN_X ;
	static const double GLOBAL_MAX_Y ;
	static const double GLOBAL_MIN_Y ;
	
	/**
	 * Creates a new GameObject
	 */
	GameObject() ;
	
	/**
	 * Copy constructor for GameObject. The new instance has its own unique ID.
	 *
	 * @param other The GameObject to be copied
	 */
	GameObject(const GameObject & other) ;
	
	/**
	 * Creates an object with the given UTF-8 symbol (preferably just
	 * one character) as its icon
	 *
	 * @param symbol The icon to be used by this GameObject
     * @param loc This GameObject's location
	 */
	GameObject(string symbol, Location * loc) ;
    
    /**
	 * Constructs a randomized GameObject. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param randSeed A seed to initialize the random number generator
	 */
	GameObject(int randSeed) ;
	
	
	/**
	 * Destructor for GameObject
	 */
	virtual ~GameObject() ;
	
	
	/**
	 * Assignment operator overload for GameObject. The object copied
	 * to will have its own unique ID.
	 *
	 * @param rhs The right hand side argument (which will be copied)
	 */
	virtual GameObject & operator=(const GameObject & rhs) ;
	
	
	/**
	 * Overloads operator() for GameObject. Possibly will be used to
	 * call notify(). TBD.
	 */
	virtual void operator()() ;
	
	
	/**
	 * Overloads the overload of operator(). For the most part the details of
	 * this function will be handled by inheriting classes.
	 *
	 * @param sentObject A reference to another GameObject
	 */
	virtual void operator()(GameObject & other) ;
	
	
	/**
	 * Every sub-type of GameObject should implement this to perform some
	 * function of their choosing. Will typically be called by other classes with a 
	 * reference to this GameObject.
	 */
	virtual void notify() ;
	
	
	/**
	 * A GameObject or any other class can implement this function to pass messages to another.
	 *
	 * @param message The Message sent by this
	 * @param recipient The object receiving the Message
	 */
	virtual void passMessage(Message * message, GameObject & recipient) ;
	
	/**
	 * Writes a formatted text description of this GameObject into the desired output stream
	 */
	virtual void textDescription(ostream * writeTO) ;

	/**
	 * Moves this GameObject by changing its Location x and y coordinates by the given offsets
	 *
	 * @param xoffset The change in this GameObject's Location.x
	 * @param yoffset The change in this GameObject's Location.y
	 */
	void move(int xoffset, int yoffset) ;
	
	/**
	 * Moves this GameObject to the Location moveTo
	 *
	 * @param moveTO The Location where this GameObject is to move
	 */
	void move(const Location moveTo) ;
	
	/**
	 * @return This GameObject's Location
	 */
	Location * getLocation() {return this->loc ; }
	
	/**
	 * Sets this GameObject's icon to the icon argument
	 *
	 * @param icon This GameObject's new icon
	 */
	void setIcon(const string & icon) ;
	
	/** 
	 * Returns this GameObject's icon
	 */
	string getIcon() ;
	
	/**
	 * Override the << output stream operator
	 */
	friend ostream & operator<<(std::ostream & os, GameObject & gameObj) ;
	
	/**
	 * Returns a string describing the properties of this GameObject
	 */
	const string * textDescription() ;
	
	/*
	 * See textDescription()
	 */
	const string * toString() ;
	
	static const string generateName(unsigned length) ;
} ;



#endif /* defined(__GameWorld__GameObject__) */
