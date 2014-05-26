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
#include "../Util/Size.hpp"
#include "../Util/Time.h"
#include "../Util/Util.hpp"
#include "../Util/Util2.h"
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
	 * Initializes texture and size information for this GameObject
	 */
	void initGraphicsData(bool overrideCurrentTexture) ;
	
	/**
	 * Handles thread starting duties. Should always be called by the function that calls
	 * the threaded function.
	 *
	 * @param goThr The thread to manage
	 * @param functionPointer Pointer to the instance member function to run on a thread
	 */
	void startThreading(void (GameObject::*functionPointer)(), bool wait) ;
	
	/**
	 * Handles thread duties. In some case will be called by the threaded function once it has completed,
	 * don't call if joinThreads() will be called
	 *
	 * @param join whether to call join() on this thread first
	 */
	void endThreading(bool join) ;
	
	friend class WorldController ;
	friend class SharedGameData ;
	
protected:
	
	unsigned ID ;

	/**
	 * String containing path to and filename of file that holds the sprite image that will represent this GameObject
	 */
	string textureImageFile ;
	
	SDL_Texture * texture = nullptr ; //It won't be possible to initialize texture or size in the constructor

	Size<int> size ;
	Pos2<long> * loc ;
	VectorHeading<long> vectDir ;

	AssetType type ;
	
	bool markedForDeletion = false ;
	

	
	const GameObject * ally = nullptr ;
	
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
	
	bool * currentlyThreading = new bool(false) ;
	
	static FastRand<int> goRand ;
	
	static void checkForMarkedDeletions() ;
	
	/**
	 * Erases the GameObject in allGameObjects matching ID.
	 */
	static void eraseByID(unsigned ID) ;
	
	/**
	 * Waits for this GameObjects thread (goThread) to finish execution, then joins the threads
	 */
	static void joinThreads() ;
	
public:
	
	/**
	 * Pointers to all extant GameObjects. WorldController will actually inialize this during its init(), by simply syncing
	 * allGameObjects to the same vector pointed by WorldController::gameObjects. In practice the two should almost always be the same.
	 * Only classes that *absolutely* must have write access to allGameObjects should it access via this method. All others should call
	 * SharedGameData::getGameObjects().
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
	GameObject(AssetType type, const string & imageFileName, float sizeModifier, const Pos2<long> & loc) ;
    
    /**
	 * Constructs a randomized GameObject. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param randSeed A seed to initialize the random number generator
	 */
	GameObject(FastRand<long> rand) ; //increase fastRand limit (currently 1) to maximum number
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
	void moveTo(Position<long> * to) ;

	/**
	 * Moves this GameObject to the Position<long> moveTo
	 *
	 * @param moveTO The Position<long> where this GameObject is to move
	 */
	void moveTo(Position<long> to) ;

	void moveTo(long x, long y, long z) { moveTo(Position<long>(x, y, z)) ; }

	void moveRight() { moveTo((loc->getX()+1), loc->getY(), loc->getZ()) ; }
	void moveLeft() { moveTo((loc->getX()-1), loc->getY(), loc->getZ()) ; }
	void moveUp() { moveTo(loc->getX(), (loc->getY()+1), loc->getZ()) ; }
	void moveDown() { moveTo(loc->getX(), (loc->getY()-1), loc->getZ()) ; }

	void moveUpRight() { moveTo((loc->getX()+1), (loc->getY()+1), loc->getZ()) ; }
	void moveUpLeft() { moveTo((loc->getX()-1), (loc->getY()+1), loc->getZ()) ; }
	void moveDownRight() { moveTo((loc->getX()+1), (loc->getY()-1), loc->getZ()) ; }
	void moveDownLeft() { moveTo((loc->getX()-1), (loc->getY()-1), loc->getZ()) ; }

	/**
	 * Moves this GameObject by changing its Position<long> x and y coordinates according to the
	 * VectorHeading of its last move
	 */
	void moveSameDirection() ;
	
	/**
	 * Moves this GameObject by changing its Position<long> x and y coordinates according to the given
	 * VectorHeading
	 *
	 * @param newDirection The new vectorDirection specifying the direction of travel
	 */
	void moveNewDirection(VectorHeading<long> & newDirection) ;
	
	
	/**
	 * Runs this GameObject's defaultBehaviors on its own thread ;
	 */
	void defaultBehaviors_threaded() ;
	
	/**
	 * Each GameObject can implement this to enable its default behaviors to run 
	 * on a loop on a separate thread
	 */
	virtual void defaultBehaviors() ;
	
	virtual void wanderVariedSpeed(FastRand<unsigned> speedVariance = FastRand<unsigned>(8, 40)) ;
	
	virtual void attack(GameObject * enemy) ;
	
	virtual void findNearbyAlly(long searchDistanceX, long searchDistanceY) ;
	
	virtual void allyWith(const GameObject *) ;
	
	/**
	 * Moves this GameObject randomly around the World (calls move() with an RNG) for time in microseconds
	 *
	 */
	virtual void wander() ;
	
	/**
	 * @return This GameObject's Position<long>
	 */
	const Position<long> * getPosition() const { return this->loc ; }

	/**
	 * @return This GameObject's Position history (Pos2)
	 */
	const Pos2<long> * getPositionHistory() const { return this->loc ; }
	
	/**
	 * @return This GameObject's vector in 3-D space
	 */
	const VectorHeading<long> getVector() const { return this->vectDir ; }
	
	/**
	 * Sets this GameObject's sprite to the specified file
	 *
	 * @param imageFile The filename and path of the sprite image
	 */
	void setImageFile(string imageFileName) ;
	
	void setTexture(SDL_Texture * texture) { this->texture = texture ; }
	
	void setSize(int w, int h) { size.setWidth(w) ; size.setHeight(h) ; }
	
	/** 
	 * @return This GameObject's textureImageFile
	 */
	string getImageFile() const ;
	
	SDL_Texture * getTexture() const { return this->texture ; }
	
	Size<int> getSize() const { return size ; }
	
	/**
	 * @return This GameObject's asset type
	 */
	AssetType getType() { return this->type ; }
	
	/**
	 * Override the << output stream operator
	 */
	friend ostream & operator<<(std::ostream & os, const GameObject & gameObj) ;
	
	/**
	 * Similar to textDescription(), but returns a new string instead of writing to one passed to it
	 */
	string toString() const ;
	
	
} ;



#endif /* defined(__GameWorld__GameObject__) */
