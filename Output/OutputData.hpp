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
	
	
	
	bool textureInitFlag = true ;
	
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
	const Position<POSUTYPE> * position ;
	
	/**
	 * @brief A pointer to a Size object, which in most cases is owned by the class that owns
	 *        this OutputData object
	 */
	const Size<SIZEUTYPE> * size ;
	
	
	void initTexture() ;
	
	void updateLastRecordedValues() ;
	
	
public:
	
	static vector<OutputData *> * getOutputData() ;
	
	/**
	 * @note Should only be called from the main thread
	 */
	static void updateAll() ;
	
	OutputData(const Position<POSUTYPE> * pos, const Size<SIZEUTYPE> * sz, PositionType type) :
        textureImageFile(),
        texture(nullptr),
        position(pos),
        size(sz),
		positionType(type)
	{
		/* texinit flag is true */
		updateLastRecordedValues() ;
		allOutputData.push_back(this) ;
	}
	
	OutputData(const AssetFile & file, const Position<POSUTYPE> * pos, const Size<SIZEUTYPE> * sz, PositionType type) :
		textureImageFile(file),
        texture(nullptr),
        position(pos),
        size(sz),
		positionType(type)
    {
        /* texinit flag is true */
		allOutputData.push_back(this) ;
    }
    
    OutputData(FastRand<int> & randm, const Position<POSUTYPE> * pos, const Size<SIZEUTYPE> * sz, PositionType type) :
        textureImageFile(AssetFile(randm)),
        texture(nullptr),
        position(pos),
        size(sz),
		positionType(type)
    {
        /* texinit flag is true */
		updateLastRecordedValues() ;
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
        /* texinit flag is true */
		allOutputData.push_back(this) ;
    }
    
    OutputData(OutputData && other) :
		textureInitFlag(false), //no need to init
        textureImageFile(other.textureImageFile),
        texture(other.texture),
        position(other.position),
        size(other.size),
		position_lastRecordedValue(std::move(other.position_lastRecordedValue)),
		size_lastRecordedValue(std::move(other.size_lastRecordedValue)),
		positionType(other.positionType),
		visible(other.visible)
    {
        other.texture = nullptr ;
        other.position = nullptr ;
        other.size = nullptr ;
		
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
            
            textureInitFlag = true ;
		}
		return *this ;
	}
    
    OutputData & operator=(OutputData && rhs) {
        if (this != &rhs) {
            this->textureImageFile = rhs.textureImageFile ;
            this->texture = rhs.texture ;
            this->position = rhs.position ;
			this->size = rhs.size ;
			this->position_lastRecordedValue = std::move(rhs.position_lastRecordedValue) ;
			this->size_lastRecordedValue = std::move(rhs.size_lastRecordedValue) ;
			this->positionType = rhs.positionType ;
			this->visible = rhs.visible ;
			
			textureInitFlag = false ;
			
            rhs.texture = nullptr ;
            rhs.position = nullptr ;
            rhs.size = nullptr ;
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
	void setTexture(Texture * texture) { SDL_DestroyTexture(this->texture) ; this->texture = texture ; updateFlag = true ; }
    Texture * getTexture() const { return texture ; }
	
	const Position<POSUTYPE> getPosition() const ;
	
	const Size<SIZEUTYPE> getSize() const { return *size ; }
	
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
		
		if (out->textureInitFlag) {
			out->initTexture() ;
		}

		out->updateLastRecordedValues() ;
	}
}


template<typename POSUTYPE, typename SIZEUTYPE>
void OutputData<POSUTYPE, SIZEUTYPE>::initTexture() {
	
	if (textureInitFlag) {
		
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
		textureInitFlag = false ;
	}
}

template<typename POSUTYPE, typename SIZEUTYPE>
void OutputData<POSUTYPE, SIZEUTYPE>::updateLastRecordedValues() {
	position_lastRecordedValue = *position ;
	size_lastRecordedValue = *size ;
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
		else if (*this->size != this->size_lastRecordedValue) {
			changed = true ;
		}
		return changed ;
	}
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
