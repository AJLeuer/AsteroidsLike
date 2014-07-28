//
//  OutputData.h
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

template<typename POSUTYPE, typename SIZEUTYPE>
struct OutputData {
	
protected:
	
	static vector<OutputData *> allOutputData ;
	
	bool initFlag = true ;
	
	/**
	 * @brief Indicates whether the OutputData has been recently updated. For every member that
	 *		  changes, it's corresponding boolean within updateFlags should be set true. Don't refer to this
	 *		  directly, instead call checkIfUpdated()
	 */

	
	bool visible = true ;
    
	bool visibility_was_updated = false ;
	
	PositionType positionType ;
	
	AssetFile textureImageFile ;
	
	/**
	 * @brief A texture
	 *
	 * @note In most cases, the class owning this OutputData object should never need to deal with texture directly
	 */
	Texture * texture ;
	bool texture_was_updated ;
	
	/**
	 * @brief A pointer to a Position object, which in most cases is owned by the class that owns
	 *        this OutputData object
	 */
	const Position<POSUTYPE> * position ;
    
    /**
     * @brief This object's orientation in 2 dimensions
     */
    Angle orientation ;
	
	/**
	 * @brief A Size object, which unlike position is not a pointer and is owned by the OutputData object
	 *
	 */
	Size<SIZEUTYPE> size ;
	
	/**
	 * @brief A copy of position from the last time this OutputData was updated
	 */
	Position<POSUTYPE> position_lastRecordedValue ;
	
	/**
	 * @brief A copy of size from the last time this OutputData was updated
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
	
	static vector<OutputData *> * getOutputData() ;
	
	/**
	 * @note Should only be called from the main thread
	 */
	static void updateAll() ;
    
    OutputData() :
        textureImageFile(),
        texture(nullptr),
        position(nullptr),
		orientation(0),
        size(), /* can't be initialized yet */
        positionType(PositionType::null)
    {
        allOutputData.push_back(this) ;
    }
	
	OutputData(Position<POSUTYPE> * pos, Angle orientation, const float sizeModifier, PositionType type, bool visible = true) :
        textureImageFile(),
        texture(nullptr),
        position(pos),
        orientation(orientation),
        size(), /* can't be initialized yet */
		positionType(type),
		visible(visible)
	{
		/* init flag is true */
		size.setModifier(sizeModifier) ;
		allOutputData.push_back(this) ;
	}
	
	OutputData(const AssetFile & file, const Position<POSUTYPE> * pos, Angle orientation,  const float sizeModifier, PositionType type, bool visible = true) :
		textureImageFile(file),
        texture(nullptr),
        position(pos),
        orientation(orientation),
        size(),
		positionType(type),
		visible(visible)
    {
        /* init flag is true */
		size.setModifier(sizeModifier) ;
		allOutputData.push_back(this) ;
    }
    
    OutputData(FastRand<int> & randm, const Position<POSUTYPE> * pos, AssetType assetType, PositionType posType, bool visible = true) :
        textureImageFile(AssetFile(randm, assetType)),
        texture(nullptr),
        position(pos),
        orientation(randm(0, 365)),
        size(),
		positionType(posType),
		visible(visible)
    {
        /* init flag is true */
		FastRand<float> sizeInit(0.75, 1.5) ;
		size.setModifier(sizeInit()) ;
		allOutputData.push_back(this) ;
    }
	
    OutputData(const OutputData & other) :
        textureImageFile(other.textureImageFile),
        texture(nullptr),
        position(other.position),
        orientation(other.orientation),
        size(other.size),
		position_lastRecordedValue(other.position_lastRecordedValue),
		size_lastRecordedValue(other.size_lastRecordedValue),
		positionType(other.positionType),
		visible(other.visible)
    {
		//init flag is true
		allOutputData.push_back(this) ;
    }
	
    OutputData(OutputData && other) :
		initFlag(other.initFlag),
        textureImageFile(other.textureImageFile),
        texture(other.texture),
        position(other.position),
        orientation(std::move(other.orientation)),
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
    
	
	~OutputData() {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture) ;
        }
    }
	
	OutputData & operator=(const OutputData & rhs) = delete ;
	
	/*
	OutputData & operator=(const OutputData & rhs) {
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
	
	OutputData & operator=(OutputData && rhs) = delete ;
	
	/*
    OutputData & operator=(OutputData && rhs) {
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
	void reinitializeMembers(const AssetFile & file, const Position<POSUTYPE> * pos, const Angle rotation, const float sizeModifier, PositionType type) ;
	
	/**
	 * @note Useful when the client class's constructor can't properly initialize this in it's initializer
	 */
	void reinitializeMembers(FastRand<int> & randm, const Position<POSUTYPE> * pos, AssetType assetType, PositionType posType) ;
	
	/**
	 * @brief Check whether this OutputData has changed since the last time it was rendered
	 *
	 * @return Whether this OutputData has changed since the last time it was rendered
	 */
	bool checkIfUpdated() ;
    
    void setAssetFile(string imageFileName) { textureImageFile = imageFileName ; }
    const AssetFile * getAssetFile() const { return & textureImageFile ; }
    
    /**
     * @note Use only when absolutely neccessary
     */
	void setTexture(Texture * texture) ;
	
    Texture * getTexture() const { return texture ; }
	
	const Position<POSUTYPE> getPosition() const ;
	
	/**
	 * @note Only use for making a copy of this OutputData's position,
	 * not for rendering operations
	 */
	const Position<POSUTYPE> * getPosition_raw() const { return position ; }
	
	PositionType getPositionType() const { return positionType ; }
	
	void setOrientation(const Angle angle) { this->orientation = angle ; }
	
	Angle * getOrientation() { return & orientation ; }

	const Angle * getOrientation(short ignored) const { return & orientation ; }
	
	const Size<SIZEUTYPE> getSize() const { return size ; }
	
	void setVisibility(bool visible) { this->visible = visible ; }
	bool isVisible() const { return visible ; }
    
    void rotateClockwise() { orientation += Angle(1) ; }
    void rotateCounterClockwise() { orientation -= Angle(1) ; }
	
} ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector<OutputData<POSUTYPE, SIZEUTYPE> *> OutputData<POSUTYPE, SIZEUTYPE>::allOutputData ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector<OutputData<POSUTYPE, SIZEUTYPE> *> * OutputData<POSUTYPE, SIZEUTYPE>::getOutputData() {
	return & allOutputData ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void OutputData<POSUTYPE, SIZEUTYPE>::updateAll() {
	
	{
	/* Debug code */
	#ifdef DEBUG_MODE
	if (this_thread::get_id() != mainThreadID) {
		DebugOutput << "OutputData::updateAll() can only be called on the main thread \n" ;
		throw exception() ;
	}
	#endif
	/* End Debug code */
	}
	
	auto * od =  & allOutputData ; /* temp debug var */
	
	for (auto i = 0 ; i < OutputData::allOutputData.size() ; i++) {
		
		OutputData * out = allOutputData.at(i) ; /* temp debug var */
		
		if (allOutputData.at(i)->initFlag) {
			allOutputData.at(i)->completeInitialization() ;
		}
		
		allOutputData.at(i)->update() ;
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void OutputData<POSUTYPE, SIZEUTYPE>::reinitializeMembers(const AssetFile & file, const Position<POSUTYPE> * pos, const Angle rotation, const float sizeModifier, PositionType type) {
	
	textureImageFile = file ;
	texture = nullptr ;
	position = pos ;
	orientation = rotation ;
	positionType = type ;
	size.setModifier(sizeModifier) ;
	
	update() ;
	
	initFlag = true ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void OutputData<POSUTYPE, SIZEUTYPE>::reinitializeMembers(FastRand<int> & randm, const Position<POSUTYPE> * pos, AssetType assetType, PositionType posType) {
	
	FastRand<float> realRand(0.0, 0.0) ; /* ignore the initialization max and mins, each individual use of this FastRand will have different max/min parameters */
	
	textureImageFile = AssetFile(randm, assetType) ;
	texture = nullptr ;
	position = pos ;
	orientation = realRand(0, 365) ;
	positionType = posType ;
	
	
	size.setModifier(realRand.nextValue(0.75f, 1.5f)) ;
	
	update() ;
	
	initFlag = true ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void OutputData<POSUTYPE, SIZEUTYPE>::completeInitialization() {
	
	{
	/* Debug code */
	#ifdef DEBUG_MODE
	if (this_thread::get_id() != mainThreadID) {
		DebugOutput << "OutputData::initGraphicsData() can only be called on the main thread \n" ;
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
void OutputData<POSUTYPE, SIZEUTYPE>::update() {
    if (checkIfUpdated()) {
        position_lastRecordedValue = *position ;
        size_lastRecordedValue = size ;

    }
}

template<typename POSUTYPE, typename SIZEUTYPE>
bool OutputData<POSUTYPE, SIZEUTYPE>::checkIfUpdated() {
	
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
void OutputData<POSUTYPE, SIZEUTYPE>::setTexture(Texture * texture) {
	SDL_DestroyTexture(this->texture) ;
	this->texture = texture ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
const Position<POSUTYPE> OutputData<POSUTYPE, SIZEUTYPE>::getPosition() const {
	if (this->positionType == PositionType::worldPosition) {
		return translateToWindowCoords(*this->position) ;
	}
	else if (this->positionType == PositionType::screenPosition) {
		return *this->position ;
	}
	return *this->position ;
}















#endif
