//
//  GraphicsData.h
//  SpriteFight
//
//  Created by aleuer2 on 7/5/14.
//  Copyright (c) 2014 Adam James Leuer. All rights reserved.
//

#ifndef SpriteFight_OutputData_h
#define SpriteFight_OutputData_h

#include <unistd.h>

#include <iostream>
#include <thread>
#include <array>
#include <tuple>
#include <chrono>
#include <initializer_list>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>

#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_image/SDL_image.h>

#include "../Util/Random.hpp"
#include "../Util/Debug.h"
#include "../Util/Util.hpp"
#include "../Util/Util2.h"
#include "../Util/Vect.hpp"
#include "../Util/Size.hpp"
#include "../Util/Rectangle.hpp"
#include "../Util/Rectangle.hpp"
#include "../Util/AssetFileIO.h"

#include "../Control/Configuration.h"

typedef array<Util::byte, 3> RGBColor ;

enum class PositionType {
	worldPosition,
	screenPosition,
    null
};


extern vector<Texture *> texturesToDestroy ;

/**
 * Simple storage class for grouping together types used by rendering functions.
 *
 * @note This class manages it's own memory usage. GraphicsData objects cannot be
 * allocated on the stack. They must be heap allocated (i.e. using operator new).
 * There's is no need to call delete on a GraphicsData object (indeed the destructor
 * is not accessible to client classes) - the class manages this automatically.
 */
template<typename POSUTYPE, typename SIZEUTYPE>
struct GraphicsData {
	
protected:
	
    static vector<GraphicsData *> allGraphicsData ;
	
	static RGBColor getColor(Texture * texture) ;
	
	/**
	 * True when the GraphicsData still needs to be initialized, else false
	 */
	bool needsInit = true ;
	
	bool visible = true ;
    
	bool visibility_was_updated = false ;
	
	bool colorFlashingActive = false ;
    
    bool boundsChecking = true ;
	
	/**
	 * True enables collision detection for this GraphicsData object, false skips it
	 */
	bool collisionDetection ;
    
    bool markedForDeletion = false ;
    
    /**
     * A BoundsCheck object that can serve to perform bounds checking when
     * an object is moved around the screen or in the world
     */
    const BoundsCheck<float> * bc = &(BoundsCheck<float>::defaultCheck) ;
	
	PositionType positionType ;
	
	AssetFile textureImageFile ;
	
	/**
	 * @brief A texture
	 *
	 * @note In most cases, the class owning this GraphicsData object should never need to deal with texture directly
	 */
	Texture * texture ;
	
	RGBColor initialColor ;
	
	/*
	 * When color flashing is enabled (signalled by colorFlashingActive equaling true), this will contain the color
	 * and duration of the flashing, as well as a timer to keep track of how long the texture has been flashing. When
	 * the time elapsed as counted by the timer equals the specified duration to flash, the flashing will end
	 */
	struct ColorFlashingInfo {
		bool colorFlashingApplied = false ;
		RGBColor color ;
		chrono::milliseconds duration = chrono::milliseconds(0) ;
		Timer timer ;
	} colorFlashingInfo;
    
	bool texture_was_updated ;
	
	/**
	 * @brief A pointer to a Vect object, which in most cases is owned by the class that owns
	 *        this GraphicsData object
	 */
    Vect<POSUTYPE> * position ;
	
	VectorAndVelocity<POSUTYPE> vectr = VectorAndVelocity<POSUTYPE>(0.0, Util::SafeBoolean::f) ;
	
	/**
	 * @brief A Size object, which unlike position is not a pointer and is owned by the GraphicsData object
	 *
	 */
	Size<SIZEUTYPE> size ;
	
	/**
	 * @brief A copy of position from the last time this GraphicsData was updated
	 */
	Vect<POSUTYPE> position_lastRecordedValue ;
	
	/**
	 * @brief A copy of size from the last time this GraphicsData was updated
	 */
	Size<SIZEUTYPE> size_lastRecordedValue ;
	
	vector<bool *> updateFlags = { & visibility_was_updated, & texture_was_updated } ; /* add more here */
    
    /**
     * @note Declared protected, thus can't be called directly. Protected destructors prevent this class from
     * being allocated on the stack
     *
     */
    virtual ~GraphicsData() {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture) ;
        }
    }
    
    /**
     * @brief Check whether this GraphicsData has changed since the last time it was rendered
     *
     * @return Whether this GraphicsData has changed since the last time it was rendered
     */
    bool checkIfUpdated() ;
    
	/**
	 * @brief Does the remaining initialization that could not be done in the constructor (because it does not
	 * know with certainty that it is on the main thread)
	 *
	 * @note Can ONLY be run on the main thread
	 */
	virtual void completeInitialization() ;
	
	virtual void update() ;
	
	//friend class GameObject ;
    
    friend class GraphicalOutput ;
	
	
public:
	
	static vector<GraphicsData *> * getOutputData() ;
	
	/**
	 * @note Should only be called from the main thread
	 */
	static void updateAll() ;
	
	static void checkForCollisions() ;
    
    /*
    GraphicsData() :
        textureImageFile(),
        texture(nullptr), //impossible to init here
        position(nullptr),
        vectr(position, Angle(0), Util::SafeBoolean::f),
        size(), //can't be initialized yet
        positionType(PositionType::null)
    {
        allGraphicsData.push_back(this) ;
    } */
	
	GraphicsData(Vect<POSUTYPE> * pos, Angle orientation, const float sizeModifier, PositionType type, Util::SafeBoolean visible = Util::SafeBoolean::t, Util::SafeBoolean monitorVelocity = Util::SafeBoolean::f, Util::SafeBoolean boundsChecking = Util::SafeBoolean::t, Util::SafeBoolean collisionDetection = Util::SafeBoolean::t) :
	
        textureImageFile(),
        texture(nullptr),
        position(pos),
        vectr(position, orientation, monitorVelocity),
        size(), /* can't be initialized yet */
		positionType(type),
		visible((bool)visible),
        boundsChecking((bool)boundsChecking),
		collisionDetection((bool)collisionDetection)
	{
		/* init flag is true */
		size.setModifier(sizeModifier) ;
        
		allGraphicsData.push_back(this) ;
        
        update() ;
	}
	
    GraphicsData(const AssetFile & file, Vect<POSUTYPE> * pos, Angle orientation, const float sizeModifier, PositionType type, Util::SafeBoolean visible = Util::SafeBoolean::t, Util::SafeBoolean monitorVelocity = Util::SafeBoolean::f, Util::SafeBoolean boundsChecking = Util::SafeBoolean::t, Util::SafeBoolean collisionDetection = Util::SafeBoolean::t) :
	
		textureImageFile(file),
        texture(nullptr),
        position(pos),
        vectr(position, orientation, monitorVelocity),
        size(),
		positionType(type),
		visible((bool)visible),
		boundsChecking((bool)boundsChecking),
		collisionDetection((bool)collisionDetection)
    {
        /* init flag is true */
		size.setModifier(sizeModifier) ;
        
        allGraphicsData.push_back(this) ;
        
        update() ;
    }
    
    GraphicsData(Randm<int> & randm, Vect<POSUTYPE> * pos, AssetType assetType, PositionType posType, Util::SafeBoolean visible = Util::SafeBoolean::t,
		Util::SafeBoolean monitorVelocity = Util::SafeBoolean::f, Util::SafeBoolean boundsChecking = Util::SafeBoolean::t, Util::SafeBoolean collisionDetection = Util::SafeBoolean::t) :
	
        textureImageFile(AssetFile(randm, assetType)),
        texture(nullptr),
        position(pos),
        vectr(position, Angle(randm), monitorVelocity),
        size(),
		positionType(posType),
		visible((bool)visible),
		boundsChecking((bool)boundsChecking),
		collisionDetection((bool)collisionDetection)
    {
        /* init flag is true */
		Randm<float> sizeInit(0.75, 1.5) ;
		size.setModifier(sizeInit()) ;
        
		allGraphicsData.push_back(this) ;
        
        update() ;
    }
	
    GraphicsData(const GraphicsData & other) :
        textureImageFile(other.textureImageFile),
        texture(nullptr),
        vectr(other.vectr),
        size(other.size),
        position(new auto(*other.position)),
		position_lastRecordedValue(other.position_lastRecordedValue),
		size_lastRecordedValue(other.size_lastRecordedValue),
		positionType(other.positionType),
		visible(other.visible),
        boundsChecking(other.boundsChecking),
        bc(new BoundsCheck<float>(*other.bc)),
        markedForDeletion(other.markedForDeletion)
    {
		//init flag is true
		allGraphicsData.push_back(this) ;
        
        update() ;
    }
	
    GraphicsData(GraphicsData && other) :
		needsInit(other.needsInit),
        textureImageFile(other.textureImageFile),
        texture(other.texture),
        vectr(std::move(other.vectr)),
		size(std::move(other.size)),
        position(other.position),
		position_lastRecordedValue(std::move(other.position_lastRecordedValue)),
		size_lastRecordedValue(std::move(other.size_lastRecordedValue)),
		positionType(other.positionType),
		visible(other.visible),
        boundsChecking(other.boundsChecking),
        bc(other.bc),
        markedForDeletion(other.markedForDeletion)
    {
        other.texture = nullptr ;
        other.position = nullptr ;
        other.bc = nullptr ;
	
		allGraphicsData.push_back(this) ;
        
        update() ;
    }
	
	GraphicsData & operator=(const GraphicsData & rhs) = delete ;
	
	GraphicsData & operator=(GraphicsData && rhs) = delete ;
	
	
	GraphicsData & copy(const GraphicsData & other) ;
	
	GraphicsData & moveCopy(GraphicsData && other) ;
	
	void modifyColor(Util::byte r, Util::byte g, Util::byte b) { SDL_SetTextureColorMod(this->texture, r, g, b) ; }
	
	void flashColor(Util::byte r, Util::byte g, Util::byte b, const chrono::milliseconds & colorChangeDuration) ;
    
    bool isMarkedForDeletion() const { return markedForDeletion ; }
    
    void markForDeletion(bool mark = true) { markedForDeletion = mark ; }
    
    void setAssetFile(string imageFileName) { textureImageFile = imageFileName ; }
    const AssetFile * getAssetFile() const { return & textureImageFile ; }
    
    /**
     * @note Use only when absolutely neccessary
     */
	void setTexture(Texture * texture) ;
	
    Texture * getTexture() const { return texture ; }
	
	void setPosition(Vect<POSUTYPE> * pos) { this->pos = pos ; }
	
	const Vect<POSUTYPE> getPosition() const ;
    
    const VectorAndVelocity<POSUTYPE> * getVector() const { return & vectr ; }
    
    /**
     * @note Use only when absolutely necessary
     */
    Vect<POSUTYPE> * getRawMutablePosition() { return position ; }
	
	/**
	 * @note Use only when no other options are available
	 */
	VectorAndVelocity<POSUTYPE> * getRawMutableVector() { return & vectr ; }
	
	/**
	 * @note Only use for making a copy of this OutputData's position,
	 * not for rendering operations
	 */
	const Vect<POSUTYPE> * getPosition_raw() const { return position ; }
	
	PositionType getPositionType() const { return positionType ; }
    
	void modifyOrientation(const Angle & angleOffset) { vectr.modifyOrientation(angleOffset) ; }
	
	void overrideCurrentOrientation(const Angle & newAngle) { vectr.overrideCurrentOrientation(newAngle) ; }
	
	const Angle * getOrientation() const { return vectr.getOrientation() ; }
	
	Angle copyOrientation() const { return vectr.copyOrientation() ; }

	//const Angle * getOrientation(short ignored) const { return & orientation ; }
	
	const Size<SIZEUTYPE> getSize() const { return size ; }
    
    const Size<SIZEUTYPE> * getSizePtr() const { return & size ; }
	
	Rectangle<POSUTYPE, SIZEUTYPE> rectangle() const ;
	
	void setVisibility(bool visible) { this->visible = visible ; }
	bool isVisible() const { return visible ; }
    
    void enableBoundsChecking() { boundsChecking = true ; }
	void disableBoundsChecking() { boundsChecking = false ; }
    bool isBoundsChecked() const { return ((boundsChecking) && (bc != nullptr)) ; }
    
    bool overBounds() ;
    
    const BoundsCheck<float> * getBoundsCheck() const { return bc ; }
    void setBoundsCheck(BoundsCheck<float> * b) { bc = b ; }
	
	void enableCollisionDetection() { collisionDetection = true ; }
	void disableCollisionDetection() { collisionDetection = false ; }
	bool collisionDetectionIsEnabled() const { return collisionDetection ; }

	void updateAndNormalizeVector() { vectr.updateAndNormalize() ; }
    
	void rotateClockwise() ;
    void rotateCounterClockwise() ;
    
    void setVectorAndOrientation(const Angle & orientation) { vectr.setVectorAndOrientation(orientation) ; }
	
} ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector< GraphicsData<POSUTYPE, SIZEUTYPE> *> GraphicsData<POSUTYPE, SIZEUTYPE>::allGraphicsData ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector< GraphicsData<POSUTYPE, SIZEUTYPE> *> * GraphicsData<POSUTYPE, SIZEUTYPE>::getOutputData() {
	return & allGraphicsData ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
RGBColor GraphicsData<POSUTYPE, SIZEUTYPE>::getColor(Texture * texture) {
	Util::byte r, g, b ;
	SDL_GetTextureColorMod(texture, & r, & g, & b) ;
	RGBColor color {r, g, b} ;
	return color ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::updateAll() {
	
	{
	/* Debug code */
	#ifdef DEBUG_MODE
	if (this_thread::get_id() != mainThreadID) {
		DebugOutput << "GraphicsData::updateAll() can only be called on the main thread \n" ;
		throw exception() ;
	}
	#endif
	/* End Debug code */
	}
	
	auto * agd =  & allGraphicsData ; /* temp debug var */
	
	for (auto i = 0 ; i < GraphicsData::allGraphicsData.size() ; i++) {
		
		auto current = allGraphicsData.at(i) ; /* temp debug var */
        
        /* Try to lock each GraphicsData's mutex before updating. When locking fails,
         that typically means that graphics data was just destroyed, so skip it. Also, check
         for null pointers stored in allGraphicsData
         */

        if ((allGraphicsData.at(i) != nullptr) && (allGraphicsData.at(i)->isMarkedForDeletion())) {
            delete allGraphicsData.at(i) ;
            allGraphicsData.at(i) = nullptr ;
        }
        else if ((allGraphicsData.at(i) != nullptr) && (allGraphicsData.at(i)->needsInit)) {
            allGraphicsData.at(i)->completeInitialization() ;
        }
        else if (allGraphicsData.at(i) != nullptr) {
            allGraphicsData.at(i)->update() ;
        }
	}
}


template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::checkForCollisions() {
	
	auto graphicsData = allGraphicsData ; //copy, so we can make modify the container to speed up iteration
	
	for (long i = (graphicsData.size() - 1) ; i >= 0 ; i--) {
		
		if ((graphicsData[i] == nullptr) || (graphicsData[i]->collisionDetection == false) || (graphicsData[i]->needsInit)) {
			graphicsData.pop_back() ;
			continue ;
		}
		
		for (auto j = 0 ; j < i ; j++) {
			
			if ((graphicsData[j] == nullptr) || (graphicsData[j]->collisionDetection == false) || (graphicsData[j]->needsInit)) {
				continue ;
			}
			
			auto rectangle_i = graphicsData[i]->rectangle() ;
			auto rectangle_j = graphicsData[j]->rectangle() ;
			
			bool collision = Rectangle<POSUTYPE, SIZEUTYPE>::detectCollision(rectangle_i, rectangle_j) ;
			
			if (collision) {
				//temporary debug code
				graphicsData[i]->flashColor(255, 0, 255, chrono::milliseconds(500)) ; //flash bright magenta when collision detected
				graphicsData[j]->flashColor(255, 0, 255, chrono::milliseconds(500)) ;
				//end debug code
			}
			
		}
		graphicsData.pop_back() ;
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
GraphicsData<POSUTYPE, SIZEUTYPE> & GraphicsData<POSUTYPE, SIZEUTYPE>::copy(const GraphicsData<POSUTYPE, SIZEUTYPE> & other) {
	needsInit = other.needsInit ;
	visible = other.visible ;
	texture_was_updated = other.texture_was_updated ;
	visibility_was_updated = other.visibility_was_updated ;
	updateFlags = other.updateFlags ;
	
	textureImageFile = other.textureImageFile ;
	texture = AssetFileIO::getTextureFromFilename(GameState::getMainRenderer(), other.textureImageFile, textureImageFile.type) ;
	size = other.size ;
    position = new auto(*other.position) ;
	position_lastRecordedValue = other.position_lastRecordedValue ;
	vectr.copy(other.vectr, position) ; //we can assume the class owning this GraphicsOutput (the same one owning the Vect object
    positionType = other.positionType ; //pointed to by position, will have updated it's position to the copy argument's position's values,
										//so we don't need to worry about that
    bc = new auto(*other.bc) ;
    markedForDeletion = other.markedForDeletion ;

	return *this ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
GraphicsData<POSUTYPE, SIZEUTYPE> & GraphicsData<POSUTYPE, SIZEUTYPE>::moveCopy(GraphicsData<POSUTYPE, SIZEUTYPE> && other) {
	needsInit = other.needsInit ;
	visible = other.visible ;
	texture_was_updated = other.texture_was_updated ;
	visibility_was_updated = other.visibility_was_updated ;
	updateFlags = std::move(other.updateFlags) ;
	
	textureImageFile = std::move(other.textureImageFile) ;
	texture = other.texture ;
	position = other.position ;
	size = std::move(other.size) ;
	position_lastRecordedValue = std::move(other.position_lastRecordedValue) ;
	vectr = std::move(other.vectr) ;
	positionType = other.positionType ;
    bc = other.bc ;
    markedForDeletion = other.markedForDeletion ;
	
	other.position = nullptr ;
    other.bc = nullptr ;
	
	return *this ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::flashColor(Util::byte r, Util::byte g, Util::byte b, const chrono::milliseconds & colorChangeDuration) {
	RGBColor rgb {r, g, b} ;
	Timer t ;
	t.startTimer() ;
	colorFlashingInfo = {false, rgb, colorChangeDuration, std::move(t) } ;
	this->colorFlashingActive = true ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
Rectangle<POSUTYPE, SIZEUTYPE> GraphicsData<POSUTYPE, SIZEUTYPE>::rectangle() const {
	
	static constexpr InitializeFromCenterCoordinates initFromCenterCoords ;
	
	Rectangle<POSUTYPE, SIZEUTYPE> rectangle(* this->position, this->size, initFromCenterCoords) ;
	
	rectangle.rotate(this->copyOrientation()) ;
	
	return rectangle ;
}


template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::completeInitialization() {
	
	{
	/* Debug code */
	#ifdef DEBUG_MODE
	if (this_thread::get_id() != mainThreadID) {
		DebugOutput << "GraphicsData::initGraphicsData() can only be called on the main thread \n" ;
		throw exception() ;
	}
	#endif
	/* End Debug code */
	}
	
	if (needsInit) {
		
		Texture * tex = nullptr ;
		tex = AssetFileIO::getTextureFromFilename(GameState::getMainRenderer(), this->textureImageFile, textureImageFile.type) ;
		
		if (tex == nullptr) {
			stringstream ss ;
			ss << "Load texture failed." << '\n' ;
			ss << SDL_GetError() << '\n' ;
			DebugOutput << ss.rdbuf() ;
			throw exception() ;
		}
		
		this->texture = tex ;
		this->initialColor = getColor(this->texture) ;
		
		/* texture must be initialized before we can set Size */
		
		//set size
		int tempW = 0  ;
		int tempH = 0  ;
		
		SDL_QueryTexture(texture, NULL, NULL, &tempW, &tempH) ; //init local size with size of texture
		
		size.setSize(tempW, tempH) ; //assign new size to this GameObject
		
		/* reset the needsInit */
		needsInit = false ;
	}
}

/* Can only be called after we complete initialization */
template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::update() {
    if (boundsChecking) {
        position->checkBounds(bc) ;
        updateAndNormalizeVector() ;
    }
    if (checkIfUpdated()) {
        position_lastRecordedValue = *position ;
        size_lastRecordedValue = size ;
    }
	if (colorFlashingActive) {
		if (colorFlashingInfo.timer.checkTimeElapsed() >= chrono::duration_cast<chrono::nanoseconds>(colorFlashingInfo.duration)) {
			colorFlashingActive = false ;
			colorFlashingInfo.timer.stopTimer() ;
			colorFlashingInfo.duration = chrono::milliseconds(0) ;
			this->modifyColor(initialColor[0], initialColor[1], initialColor[2]) ; //restore the original color
		}
		else if (colorFlashingInfo.colorFlashingApplied == false) { //there's still time left on the flashing timer, but the color change was not yet applied
			this->modifyColor(colorFlashingInfo.color[0], colorFlashingInfo.color[1], colorFlashingInfo.color[2]) ;
			colorFlashingInfo.colorFlashingApplied = true ;
		}
		//else there's still time left on the flashing timer, but the color change was already applied, so no need to do anything
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
bool GraphicsData<POSUTYPE, SIZEUTYPE>::checkIfUpdated() {
	/* if the updateFlag was set directly, this overrides any checking. Just return true immediately */
	for (auto i = 0 ; i < updateFlags.size(); i++) {
		if (*updateFlags.at(i) == true) {
			*updateFlags.at(i) = false ;
			return true ;
		}
	}

	/* otherwise do any other checking for possible changes... */
	bool changed = false ;
	if ((position != nullptr) && (*this->position != this->position_lastRecordedValue)) {
		changed = true ;
	}
	else if (this->size != this->size_lastRecordedValue) {
		changed = true ;
	}
	return changed ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::setTexture(Texture * texture) {
	texturesToDestroy.push_back(this->texture) ;
	this->texture = texture ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
const Vect<POSUTYPE> GraphicsData<POSUTYPE, SIZEUTYPE>::getPosition() const {
	if (this->positionType == PositionType::worldPosition) {
		return translateToWindowCoords(*this->position) ;
	}
	else if (this->positionType == PositionType::screenPosition) {
		return *this->position ;
	}
	return *this->position ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
bool GraphicsData<POSUTYPE, SIZEUTYPE>::overBounds() {
    return size.overBounds(*bc, *position, *vectr.getOrientation()) ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::rotateClockwise() {
	vectr.rotateVectorAndOrientation(Angle(defaultMoveDistance<float>)) ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::rotateCounterClockwise() {
	vectr.rotateVectorAndOrientation(Angle(-defaultMoveDistance<float>)) ;
}



#endif
