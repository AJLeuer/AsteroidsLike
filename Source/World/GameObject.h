//
//  GameObject.h
//  World
//
//  Created by Adam James Leuer on 3/11/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef __SpriteFight__GameObject__
#define __SpriteFight__GameObject__

#include <iostream>
#include <sstream>
#include <vector>
#include <stack>
#include <thread>
#include <functional>
#include <cmath>

#include <SDL2/SDL_surface.h>

#include "../Util/Debug.h"
#include "../Util/Util.hpp"
#include "../Util/Util2.h"
#include "../Util/Size.hpp"
#include "../Util/Random.hpp"
#include "../Util/Timer.hpp"
#include "../Util/AssetFileIO.h"

#include "../Output/GraphicsData.hpp"

#include "ForwardDecl.h"
#include "Vect.hpp"
#include "BoundsCheck.hpp"
#include "GameMap.hpp"
#include "GameState.hpp"
#include "GameInterface.h"

#include "../Control/Configuration.h"
#include "../Control/Input.hpp"

using namespace::std ;

/**
 * @brief The base class from which all other classes in the world
 *		  will inherit. This class will handle the assignment of a unique ID to each GameObject.
 *
 * @see Character
 */
class GameObject : public GameInterface {
	
	
private:
	
	
	static unsigned IDs ;
	
	static bool map_is_init ;
	
	/**
	 * Pointers to all extant GameObjects. WorldControl will actually inialize this during its init(), by simply syncing
	 * allGameObjects to the same vector pointed by WorldControl::gameObjects. In practice the two should almost always be the same
	 */
	static vector<GameObject*> * allGameObjects ;
	
	
	int ID ;
	
	bool wasMoved = false ;
	
	GraphicsData<float, int> * graphicsData ;
	
	bool onMap = false ;
	
	BasicMutex mtx ;

	/**
	 * Stores closures, each of which will undo an earlier action taken by this game object. Undo functions for the most recent actions
	 * are pushed to the top of the stack, while the undo functions for the earliest actions are at the bottom.
	 */
	stack<function<void()>> * previousActionUndoFunctions = new stack<function<void()>>() ;
	
	/**
	 * @brief Handles thread starting duties. Should always be called by the function that calls
	 *        the threaded function. Ended by calling endThreading()
	 *
	 * @param wait Whether to wait on this thread to finish
	 * @param functionPointer Pointer to the instance member function to run on a thread.
	 */
	void startThreading(void (GameObject::*functionPointer)(), bool wait) ;
	
	/**
	 * @brief Handles thread duties. In some case will be called by the threaded function once it has completed,
	 *        don't call if joinThreads() will be called.
	 *
	 * @param join whether to call join() on this thread first
	 */
	void endThreading(bool join) ;
	
	friend class WorldControl ;
	friend class GameState ;
	
protected:
	
    void update() ;
	
	/**
	 * Holds pointers to GameObjects like allGameObjects, but is 2D and the placement of each GameObject in map
	 * corresponds to the x and y coordinate of its Vect. Is synced with WorldController's map.
	 */
	static GameMap<GameObject> * map ;
	
	static Randm<int> goRand ;
	
	/**
	 * Erases the GameObject pointer matching the given ID from the allGameObjects container.
	 */
	static void eraseByID(int ID) ;
	
	static void eraseByReference(GameObject & reference) ;
    
    friend class Weapon ;
	
public:
	
	/**
	 * @note Pointers to all extant GameObjects. WorldControl will actually inialize this during its init(), by simply syncing
	 * allGameObjects to the same vector pointed by WorldControl::gameObjects. In practice the two should almost always be the same.
	 * Only classes that *absolutely* must have write access to allGameObjects should it access via this method. All others should call
	 * GameState::getGameObjects().
	 */
	static vector<GameObject*> * & accessAllGameObjects() { return GameObject::allGameObjects ; }
	
	
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
	 * @param imageFile The file to be used as the Texture for this GameObject
     * @param pos This GameObject's Vect<float>
	 */
	GameObject(const AssetFile & imageFile, float sizeModifier, const Vect<float> & pos, const Angle rotation, bool visible, SafeBoolean monitorVelocity, SafeBoolean boundsChecking, SafeBoolean collisionDetection) ;
    
    /**
	 * Constructs a randomized GameObject. The client has to option to simply leave the argument randSeed as
	 * 0, in which case the constructor will generate its own random number.
	 *
	 * @param rand A seed to initialize the random number generator
	 */
	GameObject(Randm<int> & rand, AssetType type, bool visible) ; //increase fastRand limit (currently 1) to maximum number
																								   //of values represented by enum class FileType
	
	/**
	 * Destructor for GameObject
	 */
	virtual ~GameObject() ;
	
	/**
	 *
	 */
	GameObject & operator=(const GameObject & rhs) = delete ;
	
	/**
	 *
	 */
	GameObject & operator=(GameObject && rhs) = delete ;
	
	
	/**
	 * Overloads operator() for GameObject. Possibly will be used to
	 * call notify(). TBD.
	 */
	virtual void operator()() ;
	
	
	/**
	 * Overloads the overload of operator(). For the most part the details of
	 * this function will be handled by inheriting classes.
	 *
	 * @param other A reference to another GameObject
	 */
	virtual void operator()(GameObject * other) ;
	
	/**
	 * Overloads the overload of operator(). For the most part the details of
	 * this function will be handled by inheriting classes.
	 *
	 * @param other A reference to another GameObject
     * @return whether this GameObject ID is equal to ID of other
	 */
	bool operator==(GameObject & other) const ;
	
	/**
	 * Overloads the overload of operator(). For the most part the details of
	 * this function will be handled by inheriting classes.
	 *
	 * @param other A reference to another GameObject
	 * @return whether this GameObject ID is equal to ID of other
	 */
	bool operator==(const GameObject & other) const ;
	
	/**
	 * Undoes the most recent action performed by this game object
	 */
	void reversePreviousAction() ;
	
	/**
	 * @return this ID
	 */
	unsigned getID() { return this->ID ; }
	
	/**
	 * @return This GameObject's mutex
	 */
	BasicMutex & accessMutex() { return this->mtx ; }
	
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
    
    void placeOnMap() ;
    
    void moveOnMap(const Vect<float> * toNewLoc) ;
    
    void eraseFromMap() ;
	
	/**
	 * Moves this GameObject to the Vect<float> moveTo. All other movement
	 * functions should call this.
	 *
	 * @param to The Vect<float> where this GameObject is to move
	 */
	void moveTo(Vect<float> * to) ;
	
	/**
	 * Moves this GameObject to the Vect<float> moveTo. All other movement
	 * functions should call this.
	 *
	 * @param to The Vect<float> where this GameObject is to move
	 */
	void moveTo(Vect<float> to) ;

	//void moveTo(float x, float y, float z) { moveTo(Vect<float>(x, y)) ; }

    void moveX(float x) ;
    void moveY(float y) ;
	
	virtual void moveUp() ;
	virtual void moveDown() ;
	virtual void moveRight();
	virtual void moveLeft();
    
    void orientationDependentLeftRightMove() ;
    void orientationDependentRightLeftMove() ;
    
	virtual void rotateClockwise() override ;
	virtual void rotateCounterClockwise() override ;
	
    virtual void moveRandomDirection() ;
	
	void jump() ;
    
    /**
	 * Moves this GameObject by changing its Vect<float> x and y coordinates according to the
	 * VectorAndVelocity of its last move
	 *
	 */
    void move() ;

	/**
	 * Moves this GameObject by changing its Vect<float> x and y coordinates according to the
	 * VectorAndVelocity of its last move
	 *
	 * @note If graphicsData.getBoundsCheck() == nullptr, move() will skip bounds checking
	 */
	void move(float distanceModifier) ;
	
	/**
	 * Moves this GameObject by changing its Vect<float> x and y coordinates according to the given
	 * VectorAndVelocity
	 *
	 * @note If bc = nullptr, move() will skip bounds checking
	 *
	 * @param newDirection The new vector specifying the direction of travel
	 */
	virtual void move(VectorAndVelocity<float> & direction, float distanceModifier = defaultMoveDistance<float>) ;
    
    void rotateDiff(const Angle & orientation) { graphicsData->setVectorAndOrientation(orientation) ; }
	
	/**
	 * Similar to move(), but instead of stopping when reaching the bounds of the gamespace,
	 * the GameObject reverses course
	 */
	void wander() ;
	
	/**
	 * Each GameObject can implement this to enable its default behaviors to run 
	 * on a loop on a separate thread
	 */
	virtual void doDefaultBehavior(bool initialCall = false) ;
	
	virtual void aiBehaviors() ;
	
	virtual void attack(GameObject * enemy) ;

	/**
	 * @return This GameObject's Colors
	 */
	Colors getColor() const { return graphicsData->getAssetFile()->color ; }
	
	/**
	 * @return This GameObject's Vect<float>
	 */
	const Vect<float> * getPosition() const { return this->graphicsData->getRawMutablePosition() ; }

	const Vect<float> copyPosition() const { return graphicsData->getPosition() ; }
	
	const Angle * getOrientation() const { return graphicsData->getOrientation() ; }
	
	/**
	 * @return This GameObject's vector in 3-D space
	 */
	const VectorAndVelocity<float> * getVector() { return graphicsData->getVector() ; }
    
    /**
     * @note Use only when no other options are available
     */
    Vect<float> * getRawMutablePosition() { return this->graphicsData->getRawMutablePosition() ; }
	
	/**
	 * @note Use only when no other options are available
	 */
	VectorAndVelocity<float> * getRawMutableVector() { return graphicsData->getRawMutableVector() ; }
	
	/**
	 * Sets this GameObject's sprite to the specified file
	 *
	 * @param imageFileName The filename and path of the sprite image
	 */
	void setImageFile(string imageFileName) ;
	
	/**
	 * @return This GameObject's textureImageFile, i.e. the file path of its texture (usually in png format)
	 */
	const AssetFile * getImageFile() const ;
	
	/**
	 * @return This GameObject's texture, or nullptr if this isVisible() is false
	 */
	Texture * getTexture() const ;
	
	const Size<int> * getSize() const { return graphicsData->getSizePtr() ; }
	
	/**
	 * @return This GameObject's asset type
	 */
	AssetType getType() const { return graphicsData->getAssetFile()->type ; }
	
	void setVisibility(bool visible) { this->graphicsData->setVisibility(visible) ; }
	bool isVisible() const { return this->graphicsData->isVisible() ; }
    
    bool overBounds() ;
	
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
	
	/**
	 * @note Provides no functionality. Implemented only to fullfill the requirements of GameInterface
	 * interface. See PlayerCharacter::fire() for a functional implementation
	 */
	virtual void fire() ;
	
	
} ;



#endif /* defined(__SpriteFight__GameObject__) */
