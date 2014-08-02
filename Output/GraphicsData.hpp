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
#include <chrono>
#include <initializer_list>

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_rect.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "../Util/GameRandom.hpp"
#include "../Util/Debug.h"
#include "../Util/Util.hpp"
#include "../Util/Util2.h"
#include "../Util/Position.hpp"
#include "../Util/Size.hpp"
#include "../Util/AssetFileIO.h"

#include "../Control/Configuration.h"

enum class PositionType {
	worldPosition,
	screenPosition,
    null
};

extern vector<Texture *> texturesToDestroy ;

template<typename POSUTYPE, typename SIZEUTYPE>
struct GraphicsData {
	
protected:
	
	static vector<GraphicsData *> allOutputData ;
    
    static BasicMutex gdMutex ;
	
	bool initFlag = true ;
	
	bool visible = true ;
    
	bool visibility_was_updated = false ;
	
	PositionType positionType ;
	
	AssetFile textureImageFile ;
	
	/**
	 * @brief A texture
	 *
	 * @note In most cases, the class owning this GraphicsData object should never need to deal with texture directly
	 */
	Texture * texture ;
    
	bool texture_was_updated ;
	
	/**
	 * @brief A pointer to a Position object, which in most cases is owned by the class that owns
	 *        this GraphicsData object
	 */
	Position<POSUTYPE> * position ;
    
	Vectr<POSUTYPE> vectr = Vectr<POSUTYPE>(SafeBoolean::f) ;
	
	/**
	 * @brief A Size object, which unlike position is not a pointer and is owned by the GraphicsData object
	 *
	 */
	Size<SIZEUTYPE> size ;
	
	/**
	 * @brief A copy of position from the last time this GraphicsData was updated
	 */
	Position<POSUTYPE> position_lastRecordedValue ;
	
	/**
	 * @brief A copy of size from the last time this GraphicsData was updated
	 */
	Size<SIZEUTYPE> size_lastRecordedValue ;
	
	vector<bool *> updateFlags = { & visibility_was_updated, & texture_was_updated } ; /* add more here */
	
	/**
	 * @brief Does the remaining initialization that could not be done in the constructor (because it does not
	 * know with certainty that it is on the main thread)
	 *
	 * @note Can ONLY be run on the main thread
	 */
	virtual void completeInitialization() ;
	
	virtual void update() ;
	
	friend class GameObject ;
	
	
public:
	
	static vector<GraphicsData *> * getOutputData() ;
	
	/**
	 * @note Should only be called from the main thread
	 */
	static void updateAll() ;
    
    GraphicsData() :
        textureImageFile(),
        texture(nullptr),
        position(nullptr),
        size(), /* can't be initialized yet */
        positionType(PositionType::null)
    {
        allOutputData.push_back(this) ;
    }
	
	GraphicsData(Position<POSUTYPE> * pos, Angle orientation, const float sizeModifier, PositionType type, bool visible = true) :
        textureImageFile(),
        texture(nullptr),
        position(pos),
        size(), /* can't be initialized yet */
		positionType(type),
		visible(visible)
	{
		/* init flag is true */
		size.setModifier(sizeModifier) ;
		allOutputData.push_back(this) ;
	}
	
	GraphicsData(const AssetFile & file, Position<POSUTYPE> * pos, Angle orientation,  const float sizeModifier, PositionType type, bool visible = true) :
		textureImageFile(file),
        texture(nullptr),
        position(pos),
        size(),
		positionType(type),
		visible(visible)
    {
        /* init flag is true */
		size.setModifier(sizeModifier) ;
		allOutputData.push_back(this) ;
    }
    
    GraphicsData(FastRand<int> & randm, Position<POSUTYPE> * pos, AssetType assetType, PositionType posType, bool visible = true) :
        textureImageFile(AssetFile(randm, assetType)),
        texture(nullptr),
        position(pos),
        size(),
		positionType(posType),
		visible(visible)
    {
        /* init flag is true */
		FastRand<float> sizeInit(0.75, 1.5) ;
		size.setModifier(sizeInit()) ;
		allOutputData.push_back(this) ;
    }
	
    GraphicsData(const GraphicsData & other) :
        textureImageFile(other.textureImageFile),
        texture(nullptr),
        position(other.position),
        vectr(other.vectr),
        size(other.size),
		position_lastRecordedValue(other.position_lastRecordedValue),
		size_lastRecordedValue(other.size_lastRecordedValue),
		positionType(other.positionType),
		visible(other.visible)
    {
		//init flag is true
		allOutputData.push_back(this) ;
    }
	
    GraphicsData(GraphicsData && other) :
		initFlag(other.initFlag),
        textureImageFile(other.textureImageFile),
        texture(other.texture),
        position(other.position),
        vectr(std::move(other.vectr)),
		size(std::move(other.size)),
		position_lastRecordedValue(std::move(other.position_lastRecordedValue)),
		size_lastRecordedValue(std::move(other.size_lastRecordedValue)),
		positionType(other.positionType),
		visible(other.visible)
    {
        other.texture = nullptr ;
        other.position = nullptr ;
	
		allOutputData.push_back(this) ;
    }
    
	
	~GraphicsData() {
        gdMutex.lock() ;
        if (texture != nullptr) {
			texturesToDestroy.push_back(texture) ;
        }
        gdMutex.unlock() ;
    }
	
	GraphicsData & operator=(const GraphicsData & rhs) = delete ;
	
	/*
	GraphicsData & operator=(const GraphicsData & rhs) {
		if (this != &rhs) {
            SDL_DestroyTexture(texture) ;
            
            this->textureImageFile = rhs.textureImageFile ;
			this->texture = nullptr ;
			this->position = rhs.position ;
			this->size = rhs.size ;
			this->position_lastRecordedValue = rhs.position_lastRecordedValue ;
			this->size_lastRecordedValue = rhs.size_lastRecordedValue ;
			this->positionType = rhs.positionType ;
			this->visible = rhs.visible ;
            
            initFlag = true ;
            
            update() ;
		}
		return *this ;
	} */
	
	GraphicsData & operator=(GraphicsData && rhs) = delete ;
	
	/*
    GraphicsData & operator=(GraphicsData && rhs) {
        if (this != &rhs) {
            this->textureImageFile = rhs.textureImageFile ;
            this->texture = rhs.texture ;
            this->position = rhs.position ;
			this->size = std::move(rhs.size) ;
			this->position_lastRecordedValue = std::move(rhs.position_lastRecordedValue) ;
			this->size_lastRecordedValue = std::move(rhs.size_lastRecordedValue) ;
			this->positionType = rhs.positionType ;
			this->visible = rhs.visible ;
			this->initFlag = rhs.initFlag ;
			
            rhs.texture = nullptr ;
            rhs.position = nullptr ;
            
            update() ;
        }
        return *this ;
    } */
	
	/**
	 * @note Useful when the client class's constructor can't properly initialize this in it's initializer
	 */
	void reinitializeMembers(const AssetFile & file, Position<POSUTYPE> * pos, SafeBoolean tf, const Angle rotation, const float sizeModifier, PositionType type, bool visible) ;
	
	/**
	 * @note Useful when the client class's constructor can't properly initialize this in it's initializer
	 */
	void reinitializeMembers(FastRand<int> & randm, Position<POSUTYPE> * pos, SafeBoolean tf, AssetType assetType, PositionType posType, bool visible) ;
	
	GraphicsData & copy(const GraphicsData & other) ;
	
	GraphicsData & moveCopy(GraphicsData && other) ;
	
	/**
	 * @brief Check whether this GraphicsData has changed since the last time it was rendered
	 *
	 * @return Whether this GraphicsData has changed since the last time it was rendered
	 */
	bool checkIfUpdated() ;
    
    void setAssetFile(string imageFileName) { textureImageFile = imageFileName ; }
    const AssetFile * getAssetFile() const { return & textureImageFile ; }
    
    /**
     * @note Use only when absolutely neccessary
     */
	void setTexture(Texture * texture) ;
	
    Texture * getTexture() const { return texture ; }
	
	void setPosition(Position<POSUTYPE> * pos) { this->pos = pos ; }
	
	const Position<POSUTYPE> getPosition() const ;
    
    const Vectr<POSUTYPE> * getVector() const { return & vectr ; }
    
    /**
     * @note Use only when no other options are available
     */
    Position<POSUTYPE> * getRawMutablePosition() { return position ; }
	
	/**
	 * @note Use only when no other options are available
	 */
	Vectr<POSUTYPE> * getRawMutableVector() { return & vectr ; }
	
	/**
	 * @note Only use for making a copy of this OutputData's position,
	 * not for rendering operations
	 */
	const Position<POSUTYPE> * getPosition_raw() const { return position ; }
	
	PositionType getPositionType() const { return positionType ; }
	
	void modifyOrientation(const Angle & angleOffset) { vectr.setOrientation(angleOffset) ; }
	
	void overrideCurrentOrientation(const Angle & newAngle) { vectr.overrideCurrentOrientation(newAngle) ; }
	
	const Angle * getOrientation() const { return vectr.getOrientation() ; }
	
	Angle copyOrientation() const { return vectr.copyOrientation() ; }

	//const Angle * getOrientation(short ignored) const { return & orientation ; }
	
	const Size<SIZEUTYPE> getSize() const { return size ; }
	
	void setVisibility(bool visible) { this->visible = visible ; }
	bool isVisible() const { return visible ; }
	
	void updateAndNormalizeVector() { vectr.updateAndNormalize() ; }
    
	void rotateClockwise() ;
    void rotateCounterClockwise() ;
    
    void rotateDiff(const Angle & orientation) { vectr.rotateDiff(orientation) ; }
	
} ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector<GraphicsData<POSUTYPE, SIZEUTYPE> *> GraphicsData<POSUTYPE, SIZEUTYPE>::allOutputData ;

template<typename POSUTYPE, typename SIZEUTYPE>
BasicMutex GraphicsData<POSUTYPE, SIZEUTYPE>::gdMutex ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector<GraphicsData<POSUTYPE, SIZEUTYPE> *> * GraphicsData<POSUTYPE, SIZEUTYPE>::getOutputData() {
	return & allOutputData ;
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
	
	auto * od =  & allOutputData ; /* temp debug var */
	
	for (auto i = 0 ; i < GraphicsData::allOutputData.size() ; i++) {
		gdMutex.lock() ;
		GraphicsData * out = allOutputData.at(i) ; /* temp debug var */
		
		if ((allOutputData.at(i) != nullptr) && (allOutputData.at(i)->initFlag)) {
			allOutputData.at(i)->completeInitialization() ;
		}
        if (allOutputData.at(i) != nullptr) {
            allOutputData.at(i)->update() ;
        }
        gdMutex.unlock() ;
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::reinitializeMembers(const AssetFile & file, Position<POSUTYPE> * pos, SafeBoolean monitorVelocity, const Angle rotation, const float sizeModifier, PositionType type, bool visible) {
	
	textureImageFile = file ;
	texture = nullptr ;
	position = pos ;
	vectr = Vectr<POSUTYPE>(pos, monitorVelocity) ;
	positionType = type ;
    this->visible = visible ;
	size.setModifier(sizeModifier) ;
	
	update() ;
	
	initFlag = true ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::reinitializeMembers(FastRand<int> & randm, Position<POSUTYPE> * pos, SafeBoolean monitorVelocity, AssetType assetType, PositionType posType, bool visible) {
	
	FastRand<float> realRand(0.0, 0.0) ; /* ignore the initialization max and mins, each individual use of this FastRand will have different max/min parameters */
	
	textureImageFile = AssetFile(randm, assetType) ;
	texture = nullptr ;
	position = pos ;
	vectr = Vectr<POSUTYPE>(pos, monitorVelocity) ;
	positionType = posType ;
    this->visible = visible ;
	size.setModifier(realRand.nextValue(0.75f, 1.5f)) ;
	
	update() ;
	
	initFlag = true ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
GraphicsData<POSUTYPE, SIZEUTYPE> & GraphicsData<POSUTYPE, SIZEUTYPE>::copy(const GraphicsData<POSUTYPE, SIZEUTYPE> & other) {
	initFlag = other.initFlag ;
	visible = other.visible ;
	texture_was_updated = other.texture_was_updated ;
	visibility_was_updated = other.visibility_was_updated ;
	updateFlags = other.updateFlags ;
	
	textureImageFile = other.textureImageFile ;
	texture = AssetFileIO::getTextureFromFilename(GameState::getMainRenderer(), other.textureImageFile, textureImageFile.type) ;
	size = other.size ;
	position_lastRecordedValue = other.position_lastRecordedValue ;
	vectr.copy(other.vectr, position) ; //we can assume the class owning this GraphicsOutput (the same one owning the Position object
    positionType = other.positionType ; //pointed to by position, will have updated it's position to the copy argument's position's values,
										//so we don't need to worry about that
	return *this ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
GraphicsData<POSUTYPE, SIZEUTYPE> & GraphicsData<POSUTYPE, SIZEUTYPE>::moveCopy(GraphicsData<POSUTYPE, SIZEUTYPE> && other) {
	initFlag = other.initFlag ;
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
	
	other.position = nullptr ;
	
	return *this ;
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
	
	if (initFlag) {
		
		Texture * tex = nullptr ;
		tex = AssetFileIO::getTextureFromFilename(GameState::getMainRenderer(), this->textureImageFile, textureImageFile.type) ;
		
		if (tex == nullptr) {
			stringstream ss ;
			ss << "Load texture failed." << '\n' ;
			ss << SDL_GetError() << '\n' ;
			cerr << ss.rdbuf() ;
			throw exception() ;
		}
		
		this->texture = tex ;
		
		/* texture must be initialized before we can set Size */
		
		//set size
		int tempW  ;
		int tempH  ;
		
		SDL_QueryTexture(texture, NULL, NULL, &tempW, &tempH) ; //init local size with size of texture
		
		size.setSize(tempW, tempH) ; //assign new size to this GameObject
		
		/* reset the initFlag */
		initFlag = false ;
	}
}

/* Can only be called after we complete initialization */
template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::update() {
    if (checkIfUpdated()) {
        position_lastRecordedValue = *position ;
        size_lastRecordedValue = size ;

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
	if (*this->position != this->position_lastRecordedValue ) {
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
const Position<POSUTYPE> GraphicsData<POSUTYPE, SIZEUTYPE>::getPosition() const {
	if (this->positionType == PositionType::worldPosition) {
		return translateToWindowCoords(*this->position) ;
	}
	else if (this->positionType == PositionType::screenPosition) {
		return *this->position ;
	}
	return *this->position ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::rotateClockwise() {
	vectr.rotateAbs(Angle(defaultMoveDistance<float>)) ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void GraphicsData<POSUTYPE, SIZEUTYPE>::rotateCounterClockwise() {
	vectr.rotateAbs(Angle(-defaultMoveDistance<float>)) ;
}











#endif
