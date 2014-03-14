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

#include "GameInterface.h"
#include "Message.h"


/*
 * The base class from which all other classes in the world (thought not the world
 * itself) will inherit. This class will handle the assignment of a unique ID to each GameObject.
 */
class GameObject : public BaseInterface {
	
protected:
	static unsigned IDs ;
	unsigned ID ;
	
public:
	/**
	 * Creates a new GameObject
	 */
	GameObject() ;
	
	/**
	 * Copy constructor for GameObject
	 *
	 * @param other The GameObject to be copied
	 */
	GameObject(const GameObject & other) ;
	
	/**
	 * Destructor for GameObject
	 */
	virtual ~GameObject() ;
	
	/**
	 * Assignment operator overload for GameObject
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
	 * Overloads the overload of operator().
	 
	 */
	virtual void operator()(GameObject & sentObject) ;
	
	/**
	 * Every sub-type of GameObject should implement this to perform some
	 * function of their choosing. Will typically be called by other classes with a 
	 * reference to this GameObject.
	 */
	virtual void notify() ;
	
	/**
	 * A GameObject or any other class can implement this function to pass messages to another.
	 */
	virtual void passMessage(Message *, GameObject & recipient)  = 0 ;
	
	/**
	 * Draws a representation of this GameObject
	 */
	virtual stringstream * draw() ;
	
} ;

#endif /* defined(__GameWorld__GameObject__) */
