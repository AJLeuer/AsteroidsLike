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
#include <list>
#include <thread>
#include <cmath>

#include "Debug.h"
#include "../Util/BasicTime.h"
#include "../Util/Util.hpp"
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
	static char * nameLetters ;
	static bool map_is_init ;
	
	/*this holds references to all the new threads spawned by instances of GameObject
	 allowing us to join and delete them as needed. Using a list allows us to insert and erase
	 elements without invalidating other iterators*/
	static list<thread *>* allThreads ;
	
	//the last thread added to allThreads
	static list<thread *>::iterator lastAddedThread ;
	
	/**
	 * Static member that holds the iterator of the last GameObject added to
	 * allGameObjects 
	 */
	static list<GameObject *>::iterator next_goIterator ;
	
	/**
	 * Iterator member pointing to the position of this in
	 * allGameObjects 
	 */
	list<GameObject *>::iterator goIterator ;
	
	/*
	 * Flag which sets to false when this GameObject is running a threaded
	 * function, and true once the GameObject has completed its threaded task 
	 */
	bool threadFinished = false ;
	
	
	/**
	 * Handles thread starting duties. Should always be called by the function that calls
	 * the threaded function.
	 *
	 * @param goThr The thread to manage
	 * @param wait Whether to wait for the thread to finish (by calling join()) or continue execution
	 */
	list<thread *>::iterator startThreading(std::thread * goThr, bool wait) ;
	
	/**
	 * Handles thread duties. In some case will be called by the threaded function once it has completed,
	 * though in many cases another class may finish the thread first by calling the public member function
	 * joinThread() or the static joinThreads(), which ends all GameObject threads
	 *
	 * @param pos The position of this thread in the allThreads list
	 */
	void endThreading(list<thread *>::iterator pos, bool wait) ;
	
	/**
	 * Private internal implementation of wander(), allows GameObject to wander() on its own thread.
	 * This overload runs the given period of time (in microseconds).
	 *
	 * @param pos The position of this thread in the allThreads list
	 */
	void wander_threaded(double xyOffset, long time, list<thread *>::iterator pos) ;
	
	/**
	 * Private internal implementation of wander(), allows GameObject to wander() on its own thread
	 * This overload takes a pointer to a bool and runs until it is false.
	 *
	 * @param pos The position of this thread in the allThreads list
	 */
	void wander_threaded(double xyOffset, bool * run, list<thread *>::iterator pos) ;
	
	
protected:
	

	unsigned ID ;
	string icon ;
    Location<long> * loc ;
	
	/**
	 * For more processor intensive or repetitive operations, a GameObject member function will be put into its own thread.
	 * goThread is provided for that purpose. When the function is handed off to the thread, that member's goThread pointer should
	 * be pushed back onto GameObject::allThreads.
	 */
	std::thread * goThread ;
	
	static fastRand<int> goRand ;
	
	
public:
	
	/**
	 * Pointers to all extant GameObjects. WorldController will actually inialize this during its init(), by simply syncing
	 * allGameObjects to the same vector pointed by WorldController::gameObjects. In practice the two should almost always be the same
	 */
	static list<GameObject*> * allGameObjects ;
	
	/**
	 * Holds pointers to GameObjects like allGameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its location. Is synced with WorldController's map.
	 */
	static GameMap<GameObject> * map ;
	
	static const long MAX_X ;
	static const long MIN_X ; //easier without these, but could bring them back if we decide - coords make sense
	static const long MAX_Y ;
	static const long MIN_Y ;
	
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
	bool operator==(GameObject & other) ;
	
	
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
	virtual void textDescription(ostream * writeTO) const ;

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
	Location<long> * getLocation() {return this->loc ; }
	
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
	friend ostream & operator<<(std::ostream & os, GameObject & gameObj) ;
	
	/**
	 * Similar to textDescription(), but returns a new string instead of writing to one passed to it
	 */
	string toString() const ;
	
	/**
	 * Generates a random string that can be used as a name
	 *
	 * @param length The length of the string to be generated
	 */
	static const string generateName(unsigned length) ;
	
	/**
	 * Waits for all threads spawned by GameObject instances to finish, joins
	 * the threads, then deletes the threads
	 */
	static void joinThreads() ;
	
	/**
	 * Waits for this GameObjects thread (goThread) to finish execution, then joins the threads
	 */
	void joinThread() ;
	
} ;



#endif /* defined(__GameWorld__GameObject__) */
