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
#include <thread>
#include <cmath>

#include "Debug.h"
#include "../Util/Time.h"
#include "../Util/Util.hpp"
#include "ForwardDecl.h"
#include "Location.hpp"
#include "GameMap.hpp"
#include "GameInterface.h"

using namespace::std ;

/**
 * The base class from which all other classes in the world
 *  will inherit. This class will handle the assignment of a unique ID to each GameObject.
 */
class GameObject : public GameInterface {
	
	
private:
	
	
	static unsigned IDs ;
	static vector<string> * icons ;
	
	static bool map_is_init ;
	
	/**
	 * Pointers to all extant GameObjects. WorldController will actually inialize this during its init(), by simply syncing
	 * allGameObjects to the same vector pointed by WorldController::gameObjects. In practice the two should almost always be the same
	 */
	static vector<GameObject*> * allGameObjects ;
	
	
	/**
	 * Handles thread starting duties. Should always be called by the function that calls
	 * the threaded function.
	 *
	 * @param goThr The thread to manage
	 * @param wait Whether to wait for the thread to finish (by calling join()) or continue execution
	 */
	void startThreading(std::thread * goThr, bool wait) ;
	
	/**
	 * Handles thread duties. In some case will be called by the threaded function once it has completed,
	 * don't call if joinThreads() will be called
	 *
	 * @param join whether to call join() on this thread first
	 */
	void endThreading(bool join) ;
	
	/**
	 * Private internal implementation of wander(), allows GameObject to wander() on its own thread.
	 * This overload runs the given period of time (in microseconds).
	 *
	 * @param pos The position of this thread in the allThreads list
	 */
	void wander_threaded(double xyOffset, long time) ;
	
	/**
	 * Private internal implementation of wander(), allows GameObject to wander() on its own thread
	 * This overload takes a pointer to a bool and runs until it is false.
	 *
	 * @param pos The position of this thread in the allThreads list
	 */
	void wander_threaded(double xyOffset, bool * run) ;
	
	friend class WorldController ;
	
protected:
	

	unsigned ID ;
	string icon ;
    Location<long> * loc ;
	
	static const long MAX_X ;
	static const long MIN_X ;
	static const long MAX_Y ;
	static const long MIN_Y ;
	
	
	/**
	 * This holds references to all the new threads spawned by instances of GameObject
	 * allowing us to join and delete them as needed. Using a list allows us to insert and erase
	 * elements without invalidating other iterators
	 */
	static vector< pair<thread *, GameObject*> > * allThreads ;
	
	/**
	 * For more processor intensive or repetitive operations, a GameObject member function will be put into its own thread.
	 * goThread is provided for that purpose. When the function is handed off to the thread, that member's goThread pointer should
	 * be pushed back onto GameObject::allThreads.
	 */
	std::thread * goThread ;
	bool * currentlyThreading ;
	
	static fastRand<int> goRand ;
	
	
public:
	
	/**
	 * Pointers to all extant GameObjects. WorldController will actually inialize this during its init(), by simply syncing
	 * allGameObjects to the same vector pointed by WorldController::gameObjects. In practice the two should almost always be the same
	 */
	static const vector<GameObject*> * getAllGameObjects() { return GameObject::allGameObjects ; }
	
	static const vector<GameObject*>::iterator start() { return GameObject::allGameObjects->begin() ; }
	static const vector<GameObject*>::iterator end() { return GameObject::allGameObjects->end() ; }
	
	/**
	 * Holds pointers to GameObjects like allGameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its location. Is synced with WorldController's map.
	 */
	static GameMap<GameObject> * map ;
	
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
	 * Move constructor for GameObject. The new instance the same ID.
	 *
	 * @param other The GameObject to be moved
	 */
	GameObject(GameObject && other) ;
	
	/**
	 * Creates an object with the given UTF-8 symbol (preferably just
	 * one character) as its icon
	 *
	 * @param symbol The icon to be used by this GameObject
     * @param loc This GameObject's Location<long>
	 */
	GameObject(string symbol, Location<long> * loc) ;
    
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
	 * Assignment operator overload (copy) for GameObject. The object copied
	 * to will have its own unique ID.
	 *
	 * @param rhs The right hand side argument (which will be copied)
	 */
	virtual GameObject & operator=(const GameObject & rhs) ;
	
	/**
	 * Assignment operator overload (move) for GameObject. The object copied
	 * to will have the same ID.
	 *
	 * @param rhs The right hand side argument (which will be moved)
	 */
	virtual GameObject & operator=(GameObject && rhs) ;
	
	
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
	 * Overloads the overload of operator(). For the most part the details of
	 * this function will be handled by inheriting classes.
	 *
	 * @param sentObject A reference to another GameObject
	 */
	bool operator==(GameObject & other) const ;
	
	/**
	 * Returns this ID
	 */
	unsigned getID() { return this->ID ; }
	
	/**
	 * Erases the GameObject in allGameObjects matching ID.
	 */
	static void eraseByID(unsigned ID) ;
	
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
	virtual void textDescription(ostream * writeTo) const ;

	/**
	 * Moves this GameObject by changing its Location<long> x and y coordinates by the given offsets
	 *
	 * @param xoffset The change in this GameObject's Location<long>.x
	 * @param yoffset The change in this GameObject's Location<long>.y
	 */
	void move(long xoffset, long yoffset) ;
	
	/**
	 * Moves this GameObject to the Location<long> moveTo
	 *
	 * @param moveTO The Location<long> where this GameObject is to move
	 */
	void move(const Location<long> & moveTo) ;
	
	/**
	 * Moves this GameObject randomly around the World (calls move() with an RNG) for time in microseconds
	 *
	 * @param xyOffset The max distance (in both the X and Y directions) between each move()
	 * @param time How long (in microseconds) this GameObject should wander
	 */
	void wander(double xyOffset, long time) ;
	
	/**
	 * Moves this GameObject randomly around the World (calls move() with an RNG) until run is false
	 *
	 * @param xyOffset The max distance (in both the X and Y directions) between each move()
	 * @param run Flag to continue or end execution
	 */
	void wander(double xyOffset, bool * run) ;
	
	/**
	 * @return This GameObject's Location<long>
	 */
	const Location<long> * getLocation() const {return this->loc ; }
	
	/**
	 * Sets this GameObject's icon to the icon argument
	 *
	 * @param icon This GameObject's new icon
	 */
	void setIcon(const string & icon) ;
	
	/** 
	 * Returns this GameObject's icon
	 */
	const string & getIcon() const  ;
	
	/**
	 * Override the << output stream operator
	 */
	friend ostream & operator<<(std::ostream & os, const GameObject & gameObj) ;
	
	/**
	 * Similar to textDescription(), but returns a new string instead of writing to one passed to it
	 */
	string toString() const ;
	
	
	/**
	 * Waits for this GameObjects thread (goThread) to finish execution, then joins the threads
	 */
	static void joinThreads() ;
	
} ;



#endif /* defined(__GameWorld__GameObject__) */
