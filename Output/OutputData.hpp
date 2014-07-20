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
	
	static vector<const OutputData *> allOutputData ;
    
    bool updateFlag = false ;
	
	PositionType positionType ;
    
    AssetFile textureImageFile ;
	
	bool visible = true ;
	
	/**
	 * @brief A texture
	 *
	 * @note In most cases, the class owning this OutputData object should never need to deal with texture directly
	 */
	Texture * texture ;
	
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
	
    
    void initTexture () {
        if ((texture == nullptr) || (updateFlag == true)) {
            Texture * tex = nullptr ;
            tex = AssetFileIO::getTextureFromFilename(GameState::getMainRenderer(), this->textureImageFile, textureImageFile.type) ;
            
            if (tex == nullptr) {
                stringstream ss ;
                ss << "Load texture failed." << '\n' ;
                ss << SDL_GetError() << '\n' ;
                cerr << ss.rdbuf() ;
                throw exception() ;
            }
            
            this->setTexture(tex) ;
        }
        
    }
    
public:
	
	static vector<const OutputData *> * getOutputData() ;
	
	OutputData(const Position<POSUTYPE> * pos, const Size<SIZEUTYPE> * sz, PositionType type) :
        textureImageFile(),
        texture(nullptr),
        position(pos),
        size(sz),
		positionType(type)
	{
		allOutputData.push_back(this) ;
	}
	
	OutputData(const AssetFile & file, const Position<POSUTYPE> * pos, const Size<SIZEUTYPE> * sz, PositionType type) :
		textureImageFile(file),
        texture(nullptr),
        position(pos),
        size(sz),
		positionType(type)
    {
        initTexture() ;
		allOutputData.push_back(this) ;
    }
    
    OutputData(FastRand<int> & randm, const Position<POSUTYPE> * pos, const Size<SIZEUTYPE> * sz, PositionType type) :
        textureImageFile(AssetFile(randm)),
        texture(nullptr),
        position(pos),
        size(sz),
		positionType(type)
    {
        initTexture() ;
		allOutputData.push_back(this) ;
    }
    
    OutputData(const OutputData & other) :
        textureImageFile(other.textureImageFile),
        texture(nullptr),
        position(other.position),
        size(other.size),
		positionType(other.positionType),
		visible(other.visible)
    {
        initTexture() ;
		allOutputData.push_back(this) ;
    }
    
    OutputData(OutputData && other) :
        textureImageFile(other.textureImageFile),
        texture(other.texture),
        position(other.position),
        size(other.size),
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
			this->positionType = rhs.positionType ;
			this->visible = rhs.visible ;
            
            initTexture() ;
		}
		return *this ;
	}
    
    OutputData & operator=(OutputData && rhs) {
        if (this != &rhs) {
            this->textureImageFile = rhs.textureImageFile ;
            this->texture = rhs.texture ;
            this->position = rhs.position ;
			this->size = rhs.size ;
			this->positionType = rhs.positionType ;
			this->visible = rhs.visible ;
            
            rhs.texture = nullptr ;
            rhs.position = nullptr ;
            rhs.size = nullptr ;
        }
        return *this ;
    }
    
    void setAssetFile(string imageFileName) { textureImageFile = imageFileName ; }
    const AssetFile * getAssetFile() const { return & textureImageFile ; }
    
    /**
     * @note Use only when absolutely neccessary
     */
    void setTexture(Texture * texture) { SDL_DestroyTexture(this->texture) ; this->texture = texture ; }
    Texture * getTexture() const { return texture ; }
	
	const Position<POSUTYPE> getPosition() const ;
	
	const Size<SIZEUTYPE> getSize() const { return *size ; }
	
	void setVisibility(bool visible) { this->visible = visible ; }
	bool isVisible() const { return visible ; }
	
} ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector<const OutputData<POSUTYPE, SIZEUTYPE> *> OutputData<POSUTYPE, SIZEUTYPE>::allOutputData ;

template<typename POSUTYPE, typename SIZEUTYPE>
vector<const OutputData<POSUTYPE, SIZEUTYPE> *> * OutputData<POSUTYPE, SIZEUTYPE>::getOutputData() {
	return & allOutputData ;
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
