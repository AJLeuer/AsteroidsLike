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

#include <SDL2/SDL_surface.h>

#include "../Util/Debug.h"
#include "../Util/Time.h"
#include "../Util/Util.hpp"
#include "../Util/AssetFileIO.h"

#include "../Input/Input.h"

#include "ForwardDecl.h"
#include "Position.hpp"
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
	
	friend class WorldController ;
	
protected:
	

	unsigned ID ;
	

	/**
	 * The surface that will represent this GameObject when rendered
	 */
	SDL_Surface * surface ;
	
	bool markedForDeletion = false ;
	
    Position<long> * loc ;
	
	vectorHeading<long> vectDir ;
	
	GameObject * ally = nullptr ;
	
	static const long MAX_X ;
	static const long MIN_X ;
	static const long MAX_Y ;
	static const long MIN_Y ;
	
	/**
	 * Holds pointers to GameObjects like allGameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its Position. Is synced with WorldController's map.
	 */
	static GameMap<GameObject> * map ;
	
	
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
	static vector<GameObject*> * getAllGameObjects() { return GameObject::allGameObjects ; }
	
	//static const vector<GameObject*>::iterator start() { return GameObject::allGameObjects->begin() ; }
	//static const vector<GameObject*>::iterator end() { return GameObject::allGameObjects->end() ; }
	
	static const GameMap<GameObject> * getMap() { return GameObject::map ; }
	
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
	 * @param type The type of image file associated with this GameObject (character, scenery, etc)
	 * @param imageFilename The name of the file to be used as the SDL_Surface for this GameObject
     * @param loc This GameObject's Position<long>
	 */
	GameObject(ImageType type, const string & imageFilename, Position<long> * loc) ;
    
    /**
	 * Constructs a randomized GameObject. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param randSeed A seed to initialize the random number generator
	 */
	GameObject(fastRand<long> rand, ImageType assetType = ImageType(fastRand<unsigned>(0, 1)())) ; //increase fastRand limit (currently 1) to maximum number
																								   //of values represented by enum class FileType
	
	
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
	 * Moves this GameObject to the Position<long> moveTo
	 *
	 * @param moveTO The Position<long> where this GameObject is to move
	 */
	void move(const Position<long> & moveTo) ;
	
	/**
	 * Moves this GameObject by changing its Position<long> x and y coordinates according to the
	 * vectorHeading of its last move
	 */
	void moveSameDirection() ;
	
	/**
	 * Moves this GameObject by changing its Position<long> x and y coordinates according to the given
	 * vectorHeading
	 *
	 * @param newDirection The new vectorDirection specifying the direction of travel
	 */
	void moveNewDirection(vectorHeading<long> & newDirection) ;
	
	
	/**
	 * Runs this GameObject's defaultBehaviors on its own thread ;
	 */
	void runOnThread() ;
	
	/**
	 * Each GameObject can implement this to enable its default behaviors to run 
	 * on a loop on a separate thread
	 */
	virtual void defaultBehaviors() ;
	
	virtual void attack(GameObject * enemy) ;
	
	virtual void allyWith(GameObject *) ;
	
	/**
	 * Moves this GameObject randomly around the World (calls move() with an RNG) for time in microseconds
	 *
	 * @param xyOffset The max distance (in both the X and Y directions) between each move()
	 * @param time How long (in microseconds) this GameObject should wander
	 */
	virtual void wander(long xyOffset, unsigned timeInterval, long time) ;
	
	/**
	 * Moves this GameObject randomly around the World (calls move() with an RNG) until run is false
	 *
	 * @param xyOffset The max distance (in both the X and Y directions) between each move()
	 * @param run Flag to continue or end execution
	 */
	virtual void wander(long xyOffset, unsigned timeInterval, bool * run) ;
	
	/**
	 * Moves this GameObject randomly around the World (calls move() with an RNG) until run is false
	 *
	 * @param xyOffset The max distance (in both the X and Y directions) between each move()
	 * @param run Flag to continue or end execution
	 */
	virtual void wander(long xyOffset, unsigned timeInterval, int loops, int ignored) ;
	
	/**
	 * @return This GameObject's Position<long>
	 */
	const Position<long> * getPosition() const {return this->loc ; }
	
	/**
	 * @return This GameObject's vector in 3-D space
	 */
	const vectorHeading<long> getVector() const {return this->vectDir ; }
	
	/**
	 * Sets this GameObject's icon to the icon argument
	 *
	 * @param icon This GameObject's new icon
	 */
	void setSurface(const char * icon) ;
	
	/** 
	 * Returns this GameObject's icon
	 */
	SDL_Surface * getSurface() const  ;
	
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
