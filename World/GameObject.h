//
//  GameObject.h
//  World
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
#include "../Util/Time.hpp"
#include "../Util/Util.hpp"
#include "../Util/Util2.h"
#include "../Util/AssetFileIO.h"

#include "ForwardDecl.h"
#include "Position.hpp"
#include "GameMap.hpp"
#include "GameState.h"
#include "GameInterface.h"

#include "../Control/Configuration.h"
#include "../Control/Input.hpp"

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
	 * This holds references to all the new threads spawned by instances of GameObject
	 * allowing us to join and delete them as needed. Using a list allows us to insert and erase
	 * elements without invalidating other iterators.
	 */
	static vector< pair<thread *, GameObject*> > * allThreads ;
	
	/**
	 * For more processor intensive or repetitive operations, a GameObject member function will be put into its own thread.
	 * gthread is provided for that purpose. When the function is handed off to the thread, that member's gthread pointer should
	 * be pushed back onto GameObject::allThreads.
	 */
	std::thread * gthread = nullptr ;
	
	bool * hasThread = new bool(false) ;
	
	bool markedForDeletion = false ;

	/**
	 * Initializes texture and size information for this GameObject
	 */
	void initGraphicsData(bool overrideCurrentTexture, float sizeModifier) ;
	
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
	friend class GameState ;
	
protected:
	
	unsigned ID ;

	/**
	 * String containing path to and filename of file that holds the sprite image that will represent this GameObject
	 */
	string textureImageFile ;
	
	SDL_Texture * texture = nullptr ;

	Size<int> size ;
	Pos2<float> * loc ;
	DirectionVector<float> vectr ;

	AssetType type ;
	bool visible ;
	
	const GameObject * ally = nullptr ;
	
	/**
	 * Holds pointers to GameObjects like allGameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its Position. Is synced with WorldController's map.
	 */
	static GameMap<GameObject> * map ;
	
	static FastRand<int> goRand ;
	
	void markForDeletion() { markedForDeletion = true ; }
	
	static void checkForMarkedDeletions() ;
	
	/**
	 * Erases the GameObject pointer matching the given ID from the allGameObjects container.
	 */
	static void eraseByID(unsigned ID) ;
	
	/**
	 * Waits for this GameObjects thread (gthread) to finish execution, then joins the threads
	 */
	static void joinThreads() ;
	
public:
	
	/**
	 * Pointers to all extant GameObjects. WorldController will actually inialize this during its init(), by simply syncing
	 * allGameObjects to the same vector pointed by WorldController::gameObjects. In practice the two should almost always be the same.
	 * Only classes that *absolutely* must have write access to allGameObjects should it access via this method. All others should call
	 * GameState::getGameObjects().
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
     * @param loc This GameObject's Position<float>
	 */
	GameObject(AssetType type, const string & imageFilename, float sizeModifier, const Pos2<float> & loc) ;
    
    /**
	 * Constructs a randomized GameObject. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param randSeed A seed to initialize the random number generator
	 */
	GameObject(FastRand<int> rand) ; //increase fastRand limit (currently 1) to maximum number
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
	 * Moves this GameObject to the Position<float> moveTo. All other movement
	 * functions should call this.
	 *
	 * @param moveTO The Position<float> where this GameObject is to move
	 */
	void moveTo(Position<float> * to) ;

	/**
	 * Moves this GameObject to the Position<float> moveTo.
	 *
	 * @param to The Position<float> where this GameObject is to move
	 */
	void moveTo(Position<float> to) ;

	void moveTo(float x, float y, float z) { moveTo(Position<float>(x, y, z)) ; }

	void moveX(float x) { moveTo(x, loc->getY(), loc->getZ()) ; }
	void moveY(float y) { moveTo(loc->getX(), y, loc->getZ()) ; }
	
	void moveUp() { moveTo(loc->getX(), (loc->getY()-defaultOffset<float>), loc->getZ()) ; }
	void moveDown() { moveTo(loc->getX(), (loc->getY()+defaultOffset<float>), loc->getZ()) ; }
	void moveRight() { moveTo((loc->getX()+defaultOffset<float>), loc->getY(), loc->getZ()) ; }
	void moveLeft() { moveTo((loc->getX()-defaultOffset<float>), loc->getY(), loc->getZ()) ; }
	
	void moveUp(float offset) { moveTo(loc->getX(), (loc->getY()-offset), loc->getZ()) ; }
	void moveDown(float offset) { moveTo(loc->getX(), (loc->getY()+offset), loc->getZ()) ; }
	void moveRight(float offset) { moveTo((loc->getX()+offset), loc->getY(), loc->getZ()) ; }
	void moveLeft(float offset) { moveTo((loc->getX()-offset), loc->getY(), loc->getZ()) ; }

	void moveUpRight(float offset = defaultOffset<float>) { moveTo((loc->getX()+offset), (loc->getY()-offset), loc->getZ()) ; }
	void moveUpLeft(float offset = defaultOffset<float>) { moveTo((loc->getX()-offset), (loc->getY()-offset), loc->getZ()) ; }
	void moveDownRight(float offset = defaultOffset<float>) { moveTo((loc->getX()+offset), (loc->getY()+offset), loc->getZ()) ; }
	void moveDownLeft(float offset = defaultOffset<float>) { moveTo((loc->getX()-offset), (loc->getY()+offset), loc->getZ()) ; }
	
	void jump() ;

	/**
	 * Moves this GameObject by changing its Position<float> x and y coordinates according to the
	 * DirectionVector of its last move
	 */
	void moveSameDirection() ;
	
	/**
	 * Moves this GameObject by changing its Position<float> x and y coordinates according to the given
	 * DirectionVector
	 *
	 * @param newDirection The new vectorDirection specifying the direction of travel
	 */
	void moveNewDirection(DirectionVector<float> & newDirection) ;
	
	
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
	
	virtual void findNearbyAlly(int searchDistanceX, int searchDistanceY) ;
	
	virtual void allyWith(const GameObject *) ;
	
	/**
	 * Moves this GameObject randomly around the World (calls move() with an RNG) for time in microseconds
	 *
	 */
	virtual void wander() ;
	
	/**
	 * @return This GameObject's Position<float>
	 */
	const Position<float> * getPosition() const { return this->loc ; }

	/**
	 * @return This GameObject's Position history (Pos2)
	 */
	const Pos2<float> * getPositionHistory() const { return this->loc ; }
	
	/**
	 * @return This GameObject's vector in 3-D space
	 */
	const DirectionVector<float> getVector() const { return this->vectr ; }
	
	/**
	 * Sets this GameObject's sprite to the specified file
	 *
	 * @param imageFile The filename and path of the sprite image
	 */
	void setImageFile(string imageFileName) ;
	
	void setTexture(SDL_Texture * texture) { this->texture = texture ; }
	
	//void setSize(int w, int h) { size.setWidth(w) ; size.setHeight(h) ; }
	
	/**
	 * @return This GameObject's textureImageFile, i.e. the file path of its texture (usually in png format)
	 */
	string getImageFile() const ;
	
	/**
	 * @return This GameObject's texture, or nullptr if this isVisible() is false
	 */
	SDL_Texture * getTexture() const ;
	
	const Size<int> * getSize() const { const Size<int> * rtnSize = & size ; return rtnSize ; }
	
	/**
	 * @return This GameObject's asset type
	 */
	AssetType getType() { return type ; }
	
	void setVisible() { visible = true ; }
	bool isVisible() const { return visible ; }
	
	/**
	 * Turns this GameObject invisible for nano nanoseconds
	 *
	 * @param nano The length of time to remain invisible
	 */
	void timedTurnInvisible(std::chrono::nanoseconds nano) ;
	
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
