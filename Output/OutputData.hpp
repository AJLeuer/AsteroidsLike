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
#include "../Util/Position.hpp"
#include "../Util/Size.hpp"
#include "../Util/AssetFileIO.h"

#include "../Control/Configuration.h"


template<typename POSUTYPE, typename SIZEUTYPE>
struct OutputData {
	
public:
    
    bool updateFlag = false ;
    
    AssetFile textureImageFile ;
    
    /**
     * @brief A texture
     *
     * @note In most cases, the class owning this OutputData object should never need to deal with texture directly
     */
    Texture * texture ;
    
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
	
	OutputData(const Position<POSUTYPE> * pos, const Size<SIZEUTYPE> * sz) :
        textureImageFile(),
        texture(nullptr),
        position(pos),
        size(sz) {}
	
	OutputData(const AssetFile & file, const Position<POSUTYPE> * pos, const Size<SIZEUTYPE> * sz) :
		textureImageFile(file),
        texture(nullptr),
        position(pos),
        size(sz)
    {
        initTexture() ;
    }
    
    OutputData(FastRand<int> & randm, const Position<POSUTYPE> * pos, const Size<SIZEUTYPE> * sz) :
        textureImageFile(AssetFile(randm)),
        texture(nullptr),
        position(pos),
        size(sz)
    {
        initTexture() ;
    }
    
    OutputData(const OutputData & other) :
        textureImageFile(other.textureImageFile),
        texture(nullptr),
        position(other.position),
        size(other.size)
    {
        initTexture() ;
    }
    
    OutputData(OutputData && other) :
        textureImageFile(other.textureImageFile),
        texture(other.texture),
        position(other.position),
        size(other.size)
    {
        other.texture = nullptr ;
        other.position = nullptr ;
        other.size = nullptr ;
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
    void setTexture(Texture * texture) {
        SDL_DestroyTexture(this->texture) ;
        this->texture = texture ;
    }
    
    Texture * getTexture() const { return texture ; }
	
} ;

#endif
