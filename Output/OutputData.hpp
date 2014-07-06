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


struct OutputData {
	
    Texture * texture;
    const Position<float> * position ;
    const Size<int> * size ;
	
	OutputData() {}
	
	OutputData(Texture * tex, const Position<float> * pos, const Size<int> * sz) :
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
	
} ;

#endif
