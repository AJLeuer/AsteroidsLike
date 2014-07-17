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
#include "../Util/Util.hpp"
#include "../Util/Util2.h"
#include "../Util/Size.hpp"
#include "../Util/GameRandom.hpp"
#include "../Util/Timer.hpp"
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
 *  The base class from which all other classes in the world
 *  will inherit. This class will handle the assignment of a unique ID to each GameObject.
 *
 * @sa Character
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
	
	int ID ;

	/**
	 * String containing path to and filename of file that holds the sprite image that will represent this GameObject
	 */
	AssetFile textureImageFile ;
	
	Texture * texture = nullptr ;

	Size<int> size ;
	Pos2<float> * loc ;
	Vectr<float> vectr ;

	bool visible ;
	
	const GameObject * ally = nullptr ;
	
	/**
	 * Holds pointers to GameObjects like allGameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its Position. Is synced with WorldController's map.
	 */
	static GameMap<GameObject> * map ;
	
	static FastRand<int> goRand ;
	
	void update() ;
	
	void markForDeletion() { markedForDeletion = true ; }
	
	static void checkForMarkedDeletions() ;
	
	/**
	 * Erases the GameObject pointer matching the given ID from the allGameObjects container.
	 */
	static void eraseByID(unsigned ID) ;
	

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
	
    /**
     * @return The current map of GameObjects 
     */
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
	 * @param imageFilen The file to be used as the Texture for this GameObject
     * @param loc This GameObject's Position<float>
	 */
	GameObject(const AssetFile & imageFile, float sizeModifier, const Position<float> & loc, bool visible, bool monitorVelocity) ;
    
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
	virtual void operator()(GameObject * other) ;
	
	/**
	 * Overloads the overload of operator(). For the most part the details of
	 * this function will be handled by inheriting classes.
	 *
	 * @param sentObject A reference to another GameObject
     * @return whether this GameObject ID is equal to ID of other
	 */
	bool operator==(GameObject & other) const ;
	
	/**
	 * @return this ID
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
	
	virtual void moveUp() ;
	virtual void moveDown() ;
	virtual void moveRight();
	virtual void moveLeft();
	
    void moveRandomDirection() ;
	
	void jump() ;

	/**
	 * Moves this GameObject by changing its Position<float> x and y coordinates according to the
	 * Vectr of its last move
	 */
	void move(float distanceModifier = defaultMoveDistance<float>) ;
	
	/**
	 * Moves this GameObject by changing its Position<float> x and y coordinates according to the given
	 * Vectr
	 *
	 * @param newDirection The new vector specifying the direction of travel
	 */
	virtual void moveNewDirection(Vectr<float> & newDirection) ;
	
	/**
	 * Similar to move(), but instead of stopping when reaching the bounds of the gamespace,
	 * the GameObject reverses course
	 */
	void wander() ;
	
	/**
	 * Each GameObject can implement this to enable its default behaviors to run 
	 * on a loop on a separate thread
	 */
	virtual void defaultBehaviors() ;
	
	virtual void aiBehaviors() ;
	
	virtual void attack(GameObject * enemy) ;
	
	virtual void findNearbyAlly(int searchDistanceX, int searchDistanceY) ;
	
	virtual void allyWith(const GameObject *) ;
	
	/**
	 * @return This GameObject's Colors
	 */
	Colors getColor() const { return textureImageFile.color ; }
	
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
	Vectr<float> * getVector() { return &(this->vectr) ; }
	
	/**
	 * Sets this GameObject's sprite to the specified file
	 *
	 * @param imageFile The filename and path of the sprite image
	 */
	void setImageFile(string imageFileName) ;
	
	void setTexture(Texture * texture) { this->texture = texture ; }
	
	//void setSize(int w, int h) { size.setWidth(w) ; size.setHeight(h) ; }
	
	/**
	 * @return This GameObject's textureImageFile, i.e. the file path of its texture (usually in png format)
	 */
	const AssetFile * getImageFile() const ;
	
	/**
	 * @return This GameObject's texture, or nullptr if this isVisible() is false
	 */
	Texture * getTexture() const ;
	
	const Size<int> * getSize() const { const Size<int> * rtnSize = & size ; return rtnSize ; }
	
	/**
	 * @return This GameObject's asset type
	 */
	AssetType getType() const { return textureImageFile.type ; }
	
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
