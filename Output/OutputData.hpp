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
    const Position<float> pos ;
    const Size<int> * size ;
	
	OutputData(Texture * tex, const Position<float> & pos, const Size<int> & sz) :
		texture(tex), pos(pos), size(new Size<int>(sz)) {}
	
	OutputData(const OutputData & other) :
		texture(other.texture), pos(Position<float>(other.pos)), size(new Size<int>(*other.size)) {}
	
	~OutputData() {
		delete size ;
	}
	
	OutputData & operator=(const OutputData & rhs) = delete ;
	
} ;

#endif
