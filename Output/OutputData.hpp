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
	screenPosition
};

template<typename POSUTYPE, typename SIZEUTYPE>
struct OutputData {
	
protected:
	
	static vector<OutputData *> allOutputData ;
	
	
	
	bool initFlag = true ;
	
	/**
	 * @brief Whether the OutputData has been recently updated. Don't refer to this value
	 *		  directly, instead call checkIfUpdated()
	 */
    bool updateFlag = false ;
	
	bool visible = true ;
	
	
	PositionType positionType ;
	
	AssetFile textureImageFile ;
	
	/**
	 * @brief A texture
	 *
	 * @note In most cases, the class owning this OutputData object should never need to deal with texture directly
	 */
	Texture * texture ;
	
	/**
	 * @brief A copy of position from the last time this OutputData was updated
	 */
	Position<POSUTYPE> position_lastRecordedValue ;
	
	/**
	 * @brief A copy of size from the last time this OutputData was updated
	 */
	Size<SIZEUTYPE> size_lastRecordedValue ;
	
	/**
	 * @brief A pointer to a Position object, which in most cases is owned by the class that owns
	 *        this OutputData object
	 */
	Position<POSUTYPE> * position ;
	
	/**
	 * @brief A Size object, which unlike position is not a pointer and is owned by the OutputData object
	 *
	 */
	Size<SIZEUTYPE> size ;
	
	/**
	 * @note This function's functionality cannot be part of the constructor, nor can it even be called from 
	 * the constructor itself, the reason being that we can never be sure if the constructor is run on the main
	 * thread (SDL requires graphics operations to be on the main thread). Therefore, this function is only called
	 * by updateAll() (which is always run on the main thread), and updateAll() only calls this on objects that have their
	 * initFlag set (each object should only have it's init flag set once in it's lifetime, unless it was the recipient of a 
	 * copy operation). What the constructor can and should do, then, is to set the initFlag.
	 */
	virtual void initGraphicsData() ;
	
	virtual void update() ;
	
	
public:
	
	static vector<OutputData *> * getOutputData() ;
	
	/**
	 * @note Should only be called from the main thread
	 */
	static void updateAll() ;
	
	OutputData(Position<POSUTYPE> * pos, const float sizeModifier, PositionType type) :
        textureImageFile(),
        texture(nullptr),
        position(pos),
        size(), /* can't be initialized yet */
		positionType(type)
	{
		/* init flag is true */
		size.setModifier(sizeModifier) ;
		update() ;
		allOutputData.push_back(this) ;
	}
	
	OutputData(const AssetFile & file, Position<POSUTYPE> * pos, const float sizeModifier, PositionType type) :
		textureImageFile(file),
        texture(nullptr),
        position(pos),
        size(),
		positionType(type)
    {
        /* init flag is true */
		size.setModifier(sizeModifier) ;
		allOutputData.push_back(this) ;
    }
    
    OutputData(FastRand<int> & randm, Position<POSUTYPE> * pos, const float sizeModifier, PositionType type) :
        textureImageFile(AssetFile(randm)),
        texture(nullptr),
        position(pos),
        size(),
		positionType(type)
    {
        /* init flag is true */
		size.setModifier(sizeModifier) ;
		update() ;
		allOutputData.push_back(this) ;
    }
    
    OutputData(const OutputData & other) :
        textureImageFile(other.textureImageFile),
        texture(nullptr),
        position(other.position),
        size(other.size),
		position_lastRecordedValue(other.position_lastRecordedValue),
		size_lastRecordedValue(other.size_lastRecordedValue),
		positionType(other.positionType),
		visible(other.visible)
    {
        /* init flag is true */
		allOutputData.push_back(this) ;
    }
    
    OutputData(OutputData && other) :
		initFlag(false),
        textureImageFile(other.textureImageFile),
        texture(other.texture),
        position(other.position),
		size(std::move(other.size)),
		position_lastRecordedValue(std::move(other.position_lastRecordedValue)),
		size_lastRecordedValue(std::move(other.size_lastRecordedValue)),
		positionType(other.positionType),
		visible(other.visible)
    {
		//no need to init
        other.texture = nullptr ;
        other.position = nullptr ;
		
		allOutputData.push_back(this) ;
    }
    
	
	~OutputData() {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture) ;
        }
    }
	
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
		}
		return *this ;
	}
    
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
			
			initFlag = false ;
			
            rhs.texture = nullptr ;
            rhs.position = nullptr ;
        }
        return *this ;
    }
	
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
	
	const Size<SIZEUTYPE> getSize() const { return size ; }
	
	void setVisibility(bool visible) { this->visible = visible ; }
	bool isVisible() const { return visible ; }
	
} ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector<OutputData<POSUTYPE, SIZEUTYPE> *> OutputData<POSUTYPE, SIZEUTYPE>::allOutputData ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector<OutputData<POSUTYPE, SIZEUTYPE> *> * OutputData<POSUTYPE, SIZEUTYPE>::getOutputData() {
	return & allOutputData ;
}

template<typename POSUTYPE, typename SIZEUTYPE>
void OutputData<POSUTYPE, SIZEUTYPE>::updateAll() {
	
	for (auto i = 0 ; i < OutputData::allOutputData.size() ; i++) {
		
		OutputData * out = allOutputData.at(i) ;
		
		if (out->initFlag) {
			out->initGraphicsData() ;
		}

		out->update() ;
	}
}


template<typename POSUTYPE, typename SIZEUTYPE>
void OutputData<POSUTYPE, SIZEUTYPE>::initGraphicsData() {
	
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
		initFlag = false ;
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void OutputData<POSUTYPE, SIZEUTYPE>::update() {
    if (checkIfUpdated()) {
        position_lastRecordedValue = *position ;
        size_lastRecordedValue = size ;
        
        updateFlag = false ;
    }
}

template<typename POSUTYPE, typename SIZEUTYPE>
bool OutputData<POSUTYPE, SIZEUTYPE>::checkIfUpdated() {
	
	/* if the updateFlag was set directly, this overrides any checking. Just return true immediately */
	if (updateFlag == true) {
		updateFlag = false ; //reset updateFlag
		return true ;
	}
	else {
		bool changed = false ;
		if (*this->position != this->position_lastRecordedValue ) {
			changed = true ;
		}
		else if (this->size != this->size_lastRecordedValue) {
			changed = true ;
		}
		return changed ;
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void OutputData<POSUTYPE, SIZEUTYPE>::setTexture(Texture * texture) {
	SDL_DestroyTexture(this->texture) ;
	this->texture = texture ;
	updateFlag = true ;
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
