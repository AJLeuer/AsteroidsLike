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

#include "../Util/Debug.h"
#include "../Util/Util.hpp"
#include "../Util/Position.hpp"
#include "../Util/Size.hpp"

#include "../Control/Configuration.h"


template<typename POSUTYPE, typename SIZEUTYPE>
struct OutputData {
	
    /**
     * @brief A texture
     *
     * @note In most cases, the class owning this OutputData object should never need to deal with texture directly
     */
    Texture ** texture ;
    
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
	
	OutputData() : texture(nullptr), position(nullptr), size(nullptr) {}
	
	OutputData(Texture ** tex, const Position<POSUTYPE> * pos, const Size<SIZEUTYPE> * sz) :
		texture(tex), position(pos), size(sz) {}
	
	~OutputData() {}
	
	OutputData & operator=(const OutputData & rhs) {
		if (this != &rhs) {
			this->texture = rhs.texture ;
			this->position = rhs.position ;
			this->size = rhs.size ;
		}
		return *this ;
	}
	
	void setAll(Texture ** texture, const Position<POSUTYPE> * position, const Size<SIZEUTYPE> * size) {
		this->texture = texture ;
		this->position = position ;
		this->size = size ;
	}
	
} ;

#endif
